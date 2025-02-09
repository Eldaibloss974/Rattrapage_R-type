/*
** EPITECH PROJECT, 2024
** Game
** File description:
** Server.cpp
*/

#include "Server.hpp"
#include <iostream>

static std::string endpoint_to_string(const asio::ip::udp::endpoint& ep)
{
    return ep.address().to_string() + ":" + std::to_string(ep.port());
}

Server::Server() : socket_(io_context_), retransmit_timer_(io_context_), disconnect_timer_(io_context_) {}

Server::~Server()
{
    stop();
}

void Server::start(const std::string &address, unsigned short port)
{
    asio::ip::udp::endpoint endpoint(asio::ip::udp::v4(), port);
    socket_.open(endpoint.protocol());
    socket_.bind(endpoint);

    work_ = std::make_unique<asio::io_context::work>(io_context_);
    io_thread_ = std::thread([this](){ run_io_context(); });

    do_receive();
    start_retransmit_timer();
    start_check_disconnect_timer();

    std::cout << "Server started on port " << port << std::endl;
}

void Server::stop()
{
    if (work_) {
        work_.reset();
    }
    io_context_.stop();
    if (io_thread_.joinable()) {
        io_thread_.join();
    }
    if (socket_.is_open()) {
        socket_.close();
    }
    std::cout << "Server stopped" << std::endl;
}

void Server::run_io_context()
{
    io_context_.run();
}

void Server::do_receive()
{
    socket_.async_receive_from(
            asio::buffer(recv_buffer_, sizeof(recv_buffer_)),
            remote_endpoint_,
            [this](std::error_code ec, std::size_t bytes_recvd) {
                handle_receive(ec, bytes_recvd);
            });
}

void Server::handle_receive(const std::error_code& ec, std::size_t bytes_recvd)
{
    if (!ec && bytes_recvd >= HEADER_SIZE) {
        process_packet(remote_endpoint_, recv_buffer_, bytes_recvd);
    } else {
        if (ec) {
            std::cerr << "Receive error: " << ec.message() << std::endl;
        }
    }
    if (socket_.is_open()) {
        do_receive();
    }
}

void Server::process_packet(const asio::ip::udp::endpoint& sender, const char* data, std::size_t length)
{
    auto header = read_header(data);

    std::lock_guard<std::mutex> lock(sessions_mutex_);
    ClientSession& session = get_or_create_session(sender);

    update_client_ack(session, header.ack);

    session.last_activity = std::chrono::steady_clock::now();

    if (header.seq == 0 && length == HEADER_SIZE) {
        return;
    }

    if (header.seq <= session.last_received_seq && header.seq != 0) {
        return;
    }

    if (header.seq != 0) {
        session.last_received_seq = header.seq;
        std::string payload(data + HEADER_SIZE, length - HEADER_SIZE);
        if (on_message_) {
            on_message_(sender, payload);
        }
    }
}

void Server::send_to_client(const asio::ip::udp::endpoint endpoint, const std::string payload)
{
    auto &session = get_or_create_session(endpoint);

    uint16_t seq = session.next_send_seq++;
    uint16_t ack = session.last_received_seq;

    auto packet_ptr = std::make_shared<std::string>();
    packet_ptr->resize(HEADER_SIZE + payload.size());
    write_header(packet_ptr->data(), seq, ack, 0);
    std::memcpy(packet_ptr->data() + HEADER_SIZE, payload.data(), payload.size());

    PendingMessage pm;
    pm.data = packet_ptr;
    pm.seq = seq;
    pm.send_time = std::chrono::steady_clock::now();

    session.pending.push_back(pm);

    socket_.async_send_to(asio::buffer(*packet_ptr), endpoint,
                          [seq, endpoint, packet_ptr](std::error_code ec, std::size_t) {
                              if (ec) {
                                  std::cerr << "Failed to send to " << endpoint_to_string(endpoint)
                                            << " seq=" << seq
                                            << " error=" << ec.message() << std::endl;
                              }
                          }
    );
}

void Server::broadcast(const std::string &message)
{
    for (const auto &kv : sessions_) {
        send_to_client(kv.second.endpoint, message);
    }
}

std::vector<asio::ip::udp::endpoint> Server::get_connected_clients()
{
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    std::vector<asio::ip::udp::endpoint> clients;
    for (const auto& kv : sessions_) {
        clients.push_back(kv.second.endpoint);
    }
    return clients;
}

void Server::set_on_message(const std::function<void(const asio::ip::udp::endpoint&, const std::string&)> &callback)
{
    on_message_ = callback;
}

void Server::set_on_connect(const std::function<void(const asio::ip::udp::endpoint&)> &callback)
{
    on_connect_ = callback;
}

void Server::set_on_disconnect(const std::function<void(const asio::ip::udp::endpoint&)> &callback)
{
    on_disconnect_ = callback;
}

ClientSession& Server::get_or_create_session(const asio::ip::udp::endpoint &endpoint)
{
    std::string key = endpoint_to_string(endpoint);
    auto it = sessions_.find(key);
    if (it == sessions_.end()) {
        ClientSession session;
        session.endpoint = endpoint;
        session.last_activity = std::chrono::steady_clock::now();
        sessions_[key] = session;

        if (on_connect_) {
            on_connect_(endpoint);
        }

        return sessions_[key];
    }
    return it->second;
}

void Server::update_client_ack(ClientSession& session, uint16_t ack)
{
    while (!session.pending.empty() && session.pending.front().seq <= ack) {
        session.pending.pop_front();
    }
    session.last_acked_by_client = ack;
}

void Server::start_retransmit_timer()
{
    retransmit_timer_.expires_after(std::chrono::milliseconds(200));
    retransmit_timer_.async_wait([this](std::error_code ec) {
        if (!ec) {
            retransmit_check();
            start_retransmit_timer();
        }
    });
}

void Server::retransmit_check()
{
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    auto now = std::chrono::steady_clock::now();

    for (auto& kv : sessions_) {
        auto& session = kv.second;
        for (auto& pm : session.pending) {
            if (!pm.acked && (now - pm.send_time) > std::chrono::milliseconds(500)) {
                pm.send_time = now;
                socket_.async_send_to(asio::buffer(*pm.data), session.endpoint,
                                      [data = pm.data](std::error_code ec, std::size_t) {
                                          if (ec) {
                                              std::cerr << "Retransmit error: " << ec.message() << std::endl;
                                          }
                                      });
            }
        }
    }
}

void Server::start_check_disconnect_timer()
{
    disconnect_timer_.expires_after(std::chrono::seconds(5));
    disconnect_timer_.async_wait([this](std::error_code ec) {
        if (!ec) {
            check_disconnect();
            start_check_disconnect_timer();
        }
    });
}

void Server::check_disconnect()
{
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    auto now = std::chrono::steady_clock::now();
    std::vector<std::string> disconnected_keys;

    for (auto& kv : sessions_) {
        auto& session = kv.second;
        if ((now - session.last_activity) > std::chrono::seconds(10)) {
            disconnected_keys.push_back(kv.first);
            if (on_disconnect_) {
                on_disconnect_(session.endpoint);
            }
        }
    }

    for (const auto& key : disconnected_keys) {
        sessions_.erase(key);
    }
}
