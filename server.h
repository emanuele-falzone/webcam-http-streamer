#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>

//---------------------------------- Server class -----------------------------------------

/**
 * @brief The Server class
 * Server is the "server". It receive HTTP requests and send responses
 */
class Server {
private:
    int server, client;
    sockaddr_in s_addr, c_addr;
    std::thread t;
    std::vector<std::thread*> tasks;
    std::vector<std::atomic<bool>*> ended_t;

    void bind_channel() { 
        if (bind(server, reinterpret_cast<sockaddr*>(&s_addr), sizeof(s_addr)) < 0) {
            perror("binding_failed");
            exit(1);
        }
    }

    size_t get_pos() {
        for (size_t i = 0; i < tasks.size(); ++i)
            if (*ended_t[i]) {
                if (tasks[i]->joinable())
                    tasks[i]->join();
                *ended_t[i] = false;
                return i;
            }
        tasks.push_back(nullptr);
        ended_t.push_back(new std::atomic_bool(false));
        return tasks.size() - 1;
    }

    void run() {
        while(true) {
            socklen_t c_length = sizeof(c_addr);
            if ((client = accept(server, reinterpret_cast<sockaddr*>(&c_addr), &c_length)) > 0) {
                size_t p = get_pos();
                tasks[p] = new std::thread(&Server::send_datas, this, client, p);
            } else
                break;
        }
        close(server);
    }

public:

    void start() { 
        bind_channel(); 
        listen(server, 5); 
        t = std::thread(&Server::run, this); 
    }

    void stop() { 
        shutdown(server, SHUT_RDWR);
        t.join(); 
    }

    virtual void send_datas(int socket, size_t pos) {
        *ended_t[pos] = true;
        close(socket);
    }

    Server(uint16_t port) {
        server = socket(PF_INET, SOCK_STREAM, 0);
        sockaddr_in s;
        bzero(reinterpret_cast<char*>(&s), sizeof(s));
        s.sin_family = PF_INET;
        s.sin_addr.s_addr = inet_addr("0.0.0.0");
        s.sin_port = htons(port);
        s_addr = s;
    }

    ~Server() {
        for (size_t i = 0; i < tasks.size(); ++i) { 
            if (tasks[i]->joinable()) {
                tasks[i]->join(); 
            }
            delete tasks[i]; 
            delete ended_t[i];
        }
    }
};

#endif // SERVER_H
