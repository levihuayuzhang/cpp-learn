#include <iostream>
#include <zmq.hpp>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);

    socket.connect("tcp://localhost:5555");

    zmq::message_t request(5);
    memcpy(request.data(), "Hello", 5);
    socket.send(request, zmq::send_flags::none);

    zmq::message_t response;
    socket.recv(response, zmq::recv_flags::none);

    std::cout << "Received respose: "
              << std::string(static_cast<const char *>(response.data()),
                             response.size())
              << '\n';

    return 0;
}