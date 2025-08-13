#include <iostream>
#include <zmq.hpp>

auto main() -> int {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);

    socket.bind("tcp://*:5555");

    while (true) {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);

        std::cout << "Received request: "
                  << std::string(static_cast<const char *>(request.data()),
                                 request.size())
                  << "\n";

        zmq::message_t response(5);
        memcpy(response.data(), "World", 5);
        socket.send(response, zmq::send_flags::none);
    }

    return 0;
}