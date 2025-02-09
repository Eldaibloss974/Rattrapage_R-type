/*
** EPITECH PROJECT, 2024
** Game
** File description:
** Client.cpp
*/

#include "Client.hpp"
#include <iostream>

Client::Client() : socket_(io_context_), retransmit_timer_(io_context_), disconnect_timer_(io_context_) {}

Client::~Client() {}

void Client::start(const std::string &server_addr, unsigned short server_port)
{
    asio::ip::udp::endpoint local_endpoint(asio::ip::udp::v4(), 0);
    socket_.open(local_endpoint.protocol());
    socket_.bind(local_endpoint);

    server_endpoint_ = asio::ip::udp::endpoint(asio::ip::address::from_string(server_addr), server_port);

    work_ = std::make_unique<asio::io_context::work>(io_context_);
    io_thread_ = std::thread([this](){ run_io_context(); });

    do_receive();
    start_retransmit_timer();
    start_check_disconnect_timer();
    std::cout << "Client started. Bound to " << socket_.local_endpoint().port() << std::endl;
}

void Client::stop()
{
    try {
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
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Client stopped" << std::endl;
}

void Client::run_io_context()
{
    io_context_.run();
}

void Client::do_receive()
{
    socket_.async_receive_from(
            asio::buffer(recv_buffer_, sizeof(recv_buffer_)),
            server_endpoint_,
            [this](std::error_code ec, std::size_t bytes_recvd) {
                handle_receive(ec, bytes_recvd);
            });
}

void Client::handle_receive(const std::error_code& ec, std::size_t bytes_recvd)
{
    if (!ec && bytes_recvd >= HEADER_SIZE) {
        process_packet(recv_buffer_, bytes_recvd);
    } else {
        if (ec) {
            std::cerr << "Receive error: " << ec.message() << std::endl;
        }
    }
    if (socket_.is_open()) {
        do_receive();
    }
}

void Client::process_packet(const char *data, std::size_t length)
{
    auto header = read_header(data);
    {
        std::lock_guard<std::mutex> lock(pending_mutex_);
        update_server_ack(header.ack);
    }

    auto now = std::chrono::steady_clock::now();
    disconnect_timer_.expires_after(std::chrono::seconds(10));

    if (header.seq != 0) {
        if (header.seq <= last_received_seq_) {
            return;
        }
        last_received_seq_ = header.seq;

        std::string payload(data + HEADER_SIZE, length - HEADER_SIZE);

        if (on_message_) {
            on_message_(payload);
        }

        auto ack_packet = std::make_shared<std::string>();
        ack_packet->resize(HEADER_SIZE);
        write_header(ack_packet->data(), 0, last_received_seq_, 1);

        socket_.async_send_to(asio::buffer(*ack_packet), server_endpoint_, [ack_packet](std::error_code, std::size_t) {});
    }
}

void Client::update_server_ack(uint16_t ack)
{
    while (!pending_.empty() && pending_.front().seq <= ack) {
        pending_.pop_front();
    }
    last_acked_by_server_ = ack;
}

void Client::send_message(const std::string &payload)
{
    uint16_t seq = next_send_seq_++;
    uint16_t ack = last_received_seq_;

    auto packet_ptr = std::make_shared<std::string>();
    packet_ptr->resize(HEADER_SIZE + payload.size());
    write_header(&(*packet_ptr)[0], seq, ack, 0);
    std::memcpy(&(*packet_ptr)[0] + HEADER_SIZE, payload.data(), payload.size());

    ClientPendingMessage pm;
    pm.data = packet_ptr;
    pm.seq = seq;
    pm.send_time = std::chrono::steady_clock::now();

    {
        std::lock_guard<std::mutex> lock(pending_mutex_);
        pending_.push_back(pm);
    }

    socket_.async_send_to(asio::buffer(*packet_ptr), server_endpoint_,
                          [seq, packet_ptr](std::error_code ec, std::size_t) {
                              if (ec) {
                                  std::cerr << "Failed to send seq=" << seq
                                            << " error=" << ec.message() << std::endl;
                              }
                          }
    );
}

void Client::set_on_message(const std::function<void(const std::string&)> &callback)
{
    on_message_ = callback;
}

void Client::set_on_disconnect(const std::function<void()> &callback)
{
    on_disconnect_ = callback;
}

void Client::start_retransmit_timer()
{
    retransmit_timer_.expires_after(std::chrono::milliseconds(200));
    retransmit_timer_.async_wait([this](std::error_code ec) {
        if (!ec) {
            retransmit_check();
            start_retransmit_timer();
        }
    });
}

void Client::retransmit_check()
{
    std::lock_guard<std::mutex> lock(pending_mutex_);
    auto now = std::chrono::steady_clock::now();
    for (auto &pm : pending_) {
        if (!pm.acked && (now - pm.send_time) > std::chrono::milliseconds(500)) {
            pm.send_time = now;
            socket_.async_send_to(asio::buffer(*pm.data), server_endpoint_, [](std::error_code, std::size_t){});
        }
    }
}

void Client::start_check_disconnect_timer()
{
    // disconnect_timer_.expires_after(std::chrono::seconds(10));
    // disconnect_timer_.async_wait([this](std::error_code ec) {
    //     if (!ec) {
    //         check_disconnect();
    //         start_check_disconnect_timer();
    //     }
    // });
}

void Client::check_disconnect()
{
    if (on_disconnect_) {
        on_disconnect_();
    }
    is_stopped_ = true;
}

bool Client::is_stopped()
{
    return is_stopped_;
}
