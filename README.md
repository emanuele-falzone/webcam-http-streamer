# webcam-http-streamer
A simple C++ MJPEG streamer with C socket and OpenCV

## How to compile on Linux
In this project std::thread and OpenCV has been used so you must add lib references to the compiler (and install necessary dependences).

```
g++ main.cpp -o <output_file_name> `pkg-config --cflags --libs opencv` -std=c++17 -pthread
```

## Usage

### Camera

cam_id is the number of the device connected to USB (ID are sequential: 0, 1, 2,...)

```
Camera cam(cam_id);
```

### Streamer/Shooter

```
size_t port = 1234;

Streamer streamer(port, &cam);
Shooter shooter(port + 1, &cam);
```

### To start and stop the server

Start() is asynchronous, when you call it, the server launch a thread to receive the request and send response

```
s.start();
...
s.stop();
```
