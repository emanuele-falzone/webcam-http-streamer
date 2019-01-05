#ifndef STREAMER_H
#define STREAMER_H

#include "camera.h"
#include "server.h"

//---------------------------------- Streamer class -----------------------------------------

/**
 * @brief The Streamer class
 * Streamer is the "server". It receive HTTP requests and send responses
 */
class Streamer : public Server {

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
                               "Content-Type: multipart/x-mixed-replace;boundary=--boundary\r\n\r\n";

    /**
     * @brief HEADER
     * HTTP response BODY. To be completed before sending
     */
    const std::string BODY = "--boundary\r\n"
                                    "Content-Type: image/jpeg\r\n"
                                    "Content-Length: ";
    Camera* cam;

public:

    Streamer(uint16_t port, Camera* camera) : Server(port) {
        cam = camera;
    }

    void send_datas(int socket, size_t pos) override {
        std::string image, buffer = HEADER;
        while ((send(socket, buffer.c_str(), buffer.size(), MSG_NOSIGNAL)) > 0) {
            image = cam->jpg_encode();
            buffer = BODY + std::to_string(image.size()) + "\r\n\r\n" + image;
        }
        Server::send_datas(socket, pos);
    }
};

#endif // STREAMER_H
