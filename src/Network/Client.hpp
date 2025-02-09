/*
** EPITECH PROJECT, 2024
** Game
** File description:
** Client.hpp
*/

#pragma once

#include "asio.hpp"
#include <thread>
#include <string>
#include <deque>
#include <mutex>
#include <chrono>
#include <memory>
#include <functional>

#include "common.hpp"

struct ClientPendingMessage {
    std::shared_ptr<std::string> data;
    uint16_t seq;
    std::chrono::steady_clock::time_point send_time;
    bool acked = false;
};

class Client {
    public:
        Client();
        ~Client();

        void start(const std::string &server_addr, unsigned short server_port);
        // Arrêter le client
        void stop();

        void send_message(const std::string &payload);

        void set_on_message(const std::function<void(const std::string&)>& callback);
        void set_on_disconnect(const std::function<void()>& callback);

        bool is_stopped();

        // GAME
        std::string playerId_;
        bool returnToMenu_ = false;

    private:
        void run_io_context();
        void do_receive();
        void handle_receive(const std::error_code &ec, std::size_t bytes_recvd);
        void process_packet(const char *data, std::size_t length);

        void update_server_ack(uint16_t ack);

        void start_retransmit_timer();
        void retransmit_check();

        void start_check_disconnect_timer();
        void check_disconnect();

        asio::io_context io_context_;
        std::unique_ptr<asio::io_context::work> work_;
        std::thread io_thread_;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint server_endpoint_;
        char recv_buffer_[2048];

        uint16_t last_received_seq_ = 0;
        uint16_t next_send_seq_ = 1;
        uint16_t last_acked_by_server_ = 0;
        std::deque<ClientPendingMessage> pending_;

        std::mutex pending_mutex_;
        asio::steady_timer retransmit_timer_;
        asio::steady_timer disconnect_timer_;

        std::function<void(const std::string&)> on_message_;
        std::function<void()> on_disconnect_;

        std::atomic<bool> is_stopped_ = false;

};
