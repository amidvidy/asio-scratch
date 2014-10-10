#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::udp;

namespace {

class HelloWorldServer {
public:
    HelloWorldServer(boost::asio::io_service& io_service)
        : _socket(io_service, udp::endpoint(udp::v4(), 1111))
    {
        std::cout << "starting HelloWorldServer" << std::endl;
        startReceive();
    }
private:
    void startReceive() {
        std::cout << "startReceive" << std::endl;
        _socket.async_receive_from(
            boost::asio::buffer(_recvBuffer), _remoteEndpoint,
            boost::bind(&HelloWorldServer::sendWelcome, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void sendWelcome(const boost::system::error_code& error,
                     std::size_t bytes_transferred) {
        std::cout << "sendWelcome" << std::endl;
        std::string message{"Welcome. What is your name?\n"};
        _socket.async_send_to(boost::asio::buffer(message), _remoteEndpoint,
                              boost::bind(&HelloWorldServer::getName, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
    }

    void getName(const boost::system::error_code& error,
                 std::size_t bytes_transferred) {
        std::cout << "getName" << std::endl;
        if (!error || error == boost::asio::error::message_size) {
            _socket.async_receive_from(boost::asio::buffer(_recvBuffer), _remoteEndpoint,
                                       boost::bind(&HelloWorldServer::sendHello, this,
                                                   boost::asio::placeholders::error,
                                                   boost::asio::placeholders::bytes_transferred));
        }
    }

    void sendHello(const boost::system::error_code& error,
                   std::size_t bytes_transferred) {
        std::cout << "sendHello" << std::endl;
        if (!error || error == boost::asio::error::message_size) {
            std::string name{_recvBuffer.data(), bytes_transferred};
            boost::algorithm::trim(name);
            auto message = "Hello, " + name + "\n";
            _socket.async_send_to(boost::asio::buffer(message), _remoteEndpoint,
                                  boost::bind(&HelloWorldServer::handleSend, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
        }
    }

    void handleSend(const boost::system::error_code& ec,
                    std::size_t bytes_transferred) {
        std::cout << "handleSend" << std::endl;
        startReceive();
    }

    udp::socket _socket;
    udp::endpoint _remoteEndpoint;
    std::array<char, 1024> _recvBuffer;
};

}  // namespace

int main() {
    try {
        boost::asio::io_service io_service;
        HelloWorldServer server{io_service};
        io_service.run();
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}
