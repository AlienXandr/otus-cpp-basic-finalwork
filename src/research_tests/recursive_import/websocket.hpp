#pragma once

#include "interfaces.h"
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <memory>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class WebSocket : public std::enable_shared_from_this<WebSocket> {
public:
  struct IClient {
    int param_1 = 0;
    float param_2 = 0.0;
    bool param_3 = false;
    std::shared_ptr<WebSocket> ws;
  };

private:
  websocket::stream<beast::tcp_stream> ws_;
  beast::flat_buffer buffer_;
  std::vector<IClient> &clientsList_;

public:
  // Take ownership of the socket
  WebSocket(tcp::socket &&socket, std::vector<IClient> &clientsList)
      : ws_(std::move(socket)), clientsList_(clientsList) {
    IClient client;
    client.ws = shared_from_this();
    clientsList_.push_back(client);
    std::cout << "Create websocket_session \n";
  }

  ~WebSocket() {
    clientsList_.erase(std::remove_if(clientsList_.begin(), clientsList_.end(),
                                      [this](IClient &client) -> bool {
                                        return client.ws == shared_from_this();
                                      }),
                       clientsList_.end());
    std::cout << "Delete websocket_session \n";
  };
};
