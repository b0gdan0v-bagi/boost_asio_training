#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
		public:
			connection()
			{}

			virtual ~connection()
			{}

		public:
			bool ConnectToServer();
			bool Disconnect();
			bool IsConnected() const;

		public:
			bool Send(const message<T>& msg);

		protected:
			boost::asio::ip::tcp::socket m_socket;

			boost::asio::io_context& m_asioContext;

			// this queue holds all messages to be sent out
			tsqueue<message<T>> m_qMessagesOut;

			// this queue hold message that have been recieved
			// note it is a reference as the owner of this connection
			tsqueue<owned_message>& m_qMessagesIn;

		};


	}
}