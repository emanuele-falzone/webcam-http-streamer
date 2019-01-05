#include <iostream>

#include "streamer.h"
#include "shooter.h"
#include "camera.h"


int main(int argc, char *argv[])
{
    uint16_t port, cam_id;
    if (argc > 2) {
        port = static_cast<uint16_t>(std::atoi(argv[1]));
        cam_id = static_cast<uint16_t>(std::atoi(argv[2]));

        Camera cam(cam_id);

        std::cout << "Binding camera stream on port: " << port << std::endl;
        std::cout << "Binding camera snapshot on port: " << port + 1 << std::endl;

        Streamer streamer(port, &cam);
        Shooter shooter(port + 1 , &cam);

        streamer.start();
        shooter.start();

        std::cout << "Write something to exit" << std::endl;
        std::cin.get();

        streamer.stop();
        shooter.stop();

    } else
        std::cout << "Missed arguments. Insert port and number of camera" << std::endl;
    return 0;
}
