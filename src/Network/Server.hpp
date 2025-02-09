#pragma once
#include "asio.hpp"
#include <thread>
#include <string>
#include <unordered_map>
#include <deque>
#include <chrono>
#include <mutex>
#include <memory>
#include <functional>
#include <vector>

#include "common.hpp"

struct PendingMessage {
    std::shared_ptr<std::string> data;
    uint16_t seq;
    std::chrono::steady_clock::time_point send_time;
    bool acked = false;
};

struct ClientSession {
    asio::ip::udp::endpoint endpoint;
    uint16_t last_received_seq = 0;
    uint16_t next_send_seq = 1;
    uint16_t last_acked_by_client = 0;
    std::deque<PendingMessage> pending;
    std::chrono::steady_clock::time_point last_activity;
};

class Server {
    public:
        Server();
        ~Server();

        void start(const std::string &address, unsigned short port);
        void stop();

        void send_to_client(const asio::ip::udp::endpoint endpoint, const std::string payload);
        void broadcast(const std::string& message);
        std::vector<asio::ip::udp::endpoint> get_connected_clients();

        void set_on_message(const std::function<void(const asio::ip::udp::endpoint&, const std::string&)>& callback);
        void set_on_connect(const std::function<void(const asio::ip::udp::endpoint&)>& callback);
        void set_on_disconnect(const std::function<void(const asio::ip::udp::endpoint&)>& callback);

    private:
        void run_io_context();
        void do_receive();
        void handle_receive(const std::error_code& ec, std::size_t bytes_recvd);
        void process_packet(const asio::ip::udp::endpoint& sender, const char* data, std::size_t length);

        void start_retransmit_timer();
        void retransmit_check();

        void update_client_ack(ClientSession& session, uint16_t ack);

        ClientSession& get_or_create_session(const asio::ip::udp::endpoint& endpoint);

        void start_check_disconnect_timer();
        void check_disconnect();

        asio::io_context io_context_;
        std::unique_ptr<asio::io_context::work> work_;
        std::thread io_thread_;
        asio::ip::udp::socket socket_;
        char recv_buffer_[2048];

        asio::ip::udp::endpoint remote_endpoint_;

        std::mutex sessions_mutex_;
        std::unordered_map<std::string, ClientSession> sessions_;

        asio::steady_timer retransmit_timer_;
        asio::steady_timer disconnect_timer_;

        // Callbacks pour les événements
        std::function<void(const asio::ip::udp::endpoint&, const std::string&)> on_message_;
        std::function<void(const asio::ip::udp::endpoint&)> on_connect_;
        std::function<void(const asio::ip::udp::endpoint&)> on_disconnect_;
};
