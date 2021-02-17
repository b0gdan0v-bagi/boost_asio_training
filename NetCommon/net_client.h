#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace olc
{
	namespace net
	{
		template <typename T>
		class client_interface
		{
			client_interface() : m_socket(m_context)
			{

			}
			virtual ~client_interface()
			{
				Disconnect();
			}
		public:
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					// create connection
					m_connection = std::make_unique<connection<T>>();
					boost::asio::ip::tcp::resolver resolver(m_context);
					m_endpoints = resolver.resolve(host, std::to_string(port));

					// tell connection object to connect to server
					m_connection->ConnectToServer(m_endpoints);
					// context thread
					thrContext = std::thread([this]() {m_context.run(); });

				}
				catch (std::exception& e)
				{
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}
				return true;
			}
			void Disconnect()
			{
				if (IsConnected())
				{
					m_connection->Disconnect();
				}
				m_context.stop();
				if (thrContext.joinable())
					thrContext.join();

				//destroy
				m_connection.release();
			}
			bool IsConnected()
			{
				if (m_connection)
					return m_connection->IsConnected();
				else
					return false;
			}

		public:
			// Send message to server
			void Send(const message<T>& msg)
			{
				if (IsConnected())
					m_connection->Send(msg);
			}

			tsqueue<owned_message<T>>& Incoming()
			{
				return m_qMessagesIn;
			}
		protected:
			boost::asio::io_context m_context;
			// thread of its own to execute context work commands
			std::thread thrContext;
			// hardware socket
			boost::asio::ip::tcp::socket m_socket;
			// The client has single instance of connection
			std::unique_ptr<connection<T>> m_connection;

		private:
			//This is the thread safe queue of incoming messages
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}
