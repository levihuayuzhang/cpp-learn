#include <iostream>
#include <zmq.hpp>

auto main() -> int {
    // request & reply
    // zmq::context_t context(1);
    // zmq::socket_t socket(context, zmq::socket_type::req);

    // socket.connect("tcp://localhost:5555");

    // zmq::message_t request(5);
    // memcpy(request.data(), "Hello", 5);
    // socket.send(request, zmq::send_flags::none);

    // zmq::message_t response;
    // socket.recv(response, zmq::recv_flags::none);

    // std::cout << "Received respose: "
    //           << std::string(static_cast<const char *>(response.data()),
    //                          response.size())
    //           << '\n';

    // return 0;

    // subscribe & public
    // zmq::context_t context(1);
    // zmq::socket_t socket(context, zmq::socket_type::sub);

    // socket.connect("tcp://localhost:5555");

    // socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    // while (true) {
    //     zmq::message_t topicMsg;
    //     socket.recv(topicMsg, zmq::recv_flags::none);
    //     std::string topic(static_cast<const char *>(topicMsg.data()),
    //                       topicMsg.size());

    //     zmq::message_t messageMsg;
    //     socket.recv(messageMsg, zmq::recv_flags::none);
    //     std::string message(static_cast<const char *>(messageMsg.data()),
    //                         messageMsg.size());

    //     std::cout << "Received: " << topic << " - " << message << '\n';
    // }

    // return 0;

    // push & pull
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::pull);

    socket.connect("tcp://localhost:5555");

    while (true) {
        zmq::message_t messageMsg;
        socket.recv(messageMsg, zmq::recv_flags::none);
        std::string message(static_cast<const char *>(messageMsg.data()),
                            messageMsg.size());

        std::cout << "Received: " << message << '\n';
    }

    return 0;
}