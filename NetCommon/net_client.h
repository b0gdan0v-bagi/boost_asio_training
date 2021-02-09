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
		protected:
			boost::asio::io_context m_context;
			// thread of its own to execute context work commands
			std::thread thrContext;
			// hardware socket
			boost::asio::ip::tcp::socket m_socket;

		private:
			//This is the thread safe queue of incoming messages
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}
