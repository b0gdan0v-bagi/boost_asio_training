#include <iostream>
#include <chrono>
#include "boost/asio.hpp"
#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>
#include "boost/asio/ts/buffer.hpp"
#include "boost/asio/ts/internet.hpp"

using namespace boost;

std::vector<char> vBuffer(1 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for (int i = 0; i < length; i++)
                {
                    std::cout << vBuffer[i];
                }
                GrabSomeData(socket);
            }
        }
    );
}

int main()
{
    boost::system::error_code ec;

    asio::io_context context;

    asio::io_context::work idleWork(context);

    std::thread thrContext = std::thread([&]() {context.run(); });


    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49", ec), 80);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if (!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else std::cout << "FAIL to connect:\n" << ec.message() << "\n";

    if (socket.is_open())
    {
        GrabSomeData(socket);

        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20000ms);

        context.stop();
        if (thrContext.joinable()) thrContext.join();
       
    }

    return 0;
}

