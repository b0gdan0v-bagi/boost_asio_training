#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class server_interface
		{
		public:
			server_interface(uint16_t port)
				: m_asioAcceptor(m_asioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
			{
				
			}
			virtual ~server_interface()
			{
				Stop();
			}

			bool Start()
			{
				try
				{
					WaitForClientConnection();

					m_threadContext = std::thread([this]() {m_asioContext.run(); });

				}
				catch (std::exception& e)
				{
					std::cerr << "[SERVER] Exception " << e.what() << "\n";
					return false;
				}

				std::cout << "[SERVER] Started\n";
				return true;
			}

			void Stop()
			{
				m_asioContext.stop();

				// Tidy up the context thread
				if (m_threadContext.joinable()) m_threadContext.join();

				std::cout << "[SERVER] stopped\n";
			}

			// ASYNC - asio wait for connection
			void WaitForClientConnection()
			{
				m_asioAcceptor.async_accept(
					[this](std::error_code ec, boost::asio::ip::tcp::socket socket)
					{
						if (!ec)
						{
							std::cout << "[SERVER] New connection: " << socket.remote_endpoint() << "\n";

							 

						}
						else
						{
							std::cout << "[SERVER] new connection error: " << ec.message() << "\n";
						}

						// prime the asiot context more work - again simply wait for another connection
						WaitForClientConnection();
					});
				)
			}

			void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
			{

			}
			void MessageAllClients(const message<T>& msh, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
			{

			}
		protected:
			virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
			{
				return false;
			}
			virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
			{

			}

			virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
			{

			}
		protected:
			tsqueue<owned_message<T>> m_qMessagesIn;

			boost::asio::io_context m_asioContext;
			std::thread m_threadContext;

			// for asion context
			boost::asio::ip::tcp::acceptor m_asioAcceptor;
			// clients will be represented by ID
			uint32_t nIDCounter = 10000;
		};

	}
}