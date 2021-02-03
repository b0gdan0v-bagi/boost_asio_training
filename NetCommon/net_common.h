#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif // _WIN32

#include "boost/asio.hpp"
#include <boost/asio/error.hpp>
#include <boost/system/error_code.hpp>
#include "boost/asio/ts/buffer.hpp"
#include "boost/asio/ts/internet.hpp"