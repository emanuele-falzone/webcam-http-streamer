#ifndef SHOOTER_H
#define SHOOTER_H

#include "camera.h"
#include "server.h"

//---------------------------------- Shooter class -----------------------------------------

/**
 * @brief The Shooter class
 * Shooter is the "server". It receive HTTP requests and send responses
 */
class Shooter : public Server {

private:

    /**
     * @brief HEADER
     * HTTP response header
     */
    const std::string HEADER = "HTTP/1.1 200 OK\r\n"
                               "Connection: close\r\n"
                               "Max-Age: 0\r\n"
                               "Expires: 0\r\n"
                               "Cache-Control: no-cache, private\r\n"
                               "Pragma: no-cache\r\n"
                               "Content-Type: multipart/x-mixed-replace";

    /**
     * @brief HEADER
     * HTTP response BODY. To be completed before sending
     */
    const std::string BODY = "Content-Type: image/jpeg\r\n"
                             "Content-Length: ";

    Camera* cam;

public:

    Shooter(uint16_t port, Camera* camera) : Server(port){
        cam = camera;
    }

    void send_datas(int socket, size_t pos) override {
        std::string image, buffer;
        image = cam->jpg_encode();
        buffer = HEADER + BODY + std::to_string(image.size()) + "\r\n\r\n" + image;
        send(socket, buffer.c_str(), buffer.size(), MSG_NOSIGNAL);
        Server::send_datas(socket, pos);
    }
};

#endif // SHOOTER_H
