#pragma once

#include "net_common.h"

namespace olc
{
	namespace net
	{
		// message header is sent at start of all messages.
		template <typename T>
		struct  message_header
		{
			T id{};
			uint32_t size = 0;
		};

		template <typename T>
		struct message
		{
			message_header<T> header{};
			std::vector<uint8_t> body;

			size_t size() const
			{
				return sizeof(message_header<T>) + body.size();
			}

			// Overried for std::cout compatibility
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
			{
				os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
				return os;
			}

			// Pushes any POD-like data into the message buffer
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data)
			{
				//Check that the type of the data being pushed is trivially copyable
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");
				//Cache current size of vector, as this will be the point we insert data
				size_t i = msg.body.size();

				// copy the data into the newly allocated vector space
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));
				//resize the vector by the size of the data allocated vector space

				msg.body.resize(msg.body.size() + sizeof(DataType));
				// Recalculate message size
				msg.header.size = msg.size();
				// Return the target message so it can be chained
				return msg;
			}
			template<typename DataType>
			friend message<T>& operator >> (message<T>& msg, DataType& data)
			{
				// check that the type of data is trivial
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed");

				// Cache the location twards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - sizeof(DataType);
				// copy the data from vector into the user variable
				std::memcpy(&data, msg.body.data() + i,  sizeof(DataType));
				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate message size
				msg.header.size = msg.size();
				// Return the target message so it can be chained
				return msg;

			}



		};
		template <typename T>
		class connection;

		template <typename T>
		struct owned_message
		{
			message<T> msg;
			std::shared_ptr<connection<T>> remote = nullptr;

			friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
			{
				os << msg.msg;
				return os;
			}

		};
	}
}