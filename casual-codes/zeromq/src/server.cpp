#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <zmq.hpp>

auto main() -> int {
    // request & reply
    // zmq::context_t context(1);
    // zmq::socket_t socket(context, zmq::socket_type::rep);

    // socket.bind("tcp://*:5555");

    // while (true) {
    //     zmq::message_t request;
    //     socket.recv(request, zmq::recv_flags::none);

    //     std::cout << "Received request: "
    //               << std::string(static_cast<const char *>(request.data()),
    //                              request.size())
    //               << "\n";

    //     zmq::message_t response(5);
    //     memcpy(response.data(), "World", 5);
    //     socket.send(response, zmq::send_flags::none);
    // }

    // return 0;

    // subscribe & public
    // zmq::context_t context(1);
    // zmq::socket_t socket(context, zmq::socket_type::pub);

    // socket.bind("tcp://*:5555");

    // int count = 0;
    // while (true) {
    //     std::string topic = "Topic";
    //     std::string message = "Message" + std::to_string(count);

    //     zmq::message_t topicMsg(topic.size());
    //     memcpy(topicMsg.data(), topic.data(), topic.size());
    //     socket.send(topicMsg, zmq::send_flags::sndmore);

    //     zmq::message_t messageMsg(message.size());
    //     memcpy(messageMsg.data(), message.data(), message.size());
    //     socket.send(messageMsg, zmq::send_flags::none);

    //     std::cout << "Published: " << topic << " - " << message << '\n';

    //     count++;
    //     sleep(1);
    // }

    // return 0;

    // push & pull
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::push);

    socket.bind("tcp://*:5555");

    int count = 0;
    while (true) {
        std::string message = "Message " + std::to_string(count);

        zmq::message_t messageMsg(message.size());
        memcpy(messageMsg.data(), message.data(), message.size());
        socket.send(messageMsg, zmq::send_flags::none);

        std::cout << "Pushed: " << message << '\n';

        count++;
        sleep(1);
    }

    return 0;
}
