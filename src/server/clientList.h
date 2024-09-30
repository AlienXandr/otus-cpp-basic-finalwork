#pragma once

#include "../device_emulator/interfaces/IJsonMsg.h"
#include <vector>

class websocket_session;

struct IClient {
  IChartState chartState;
  websocket_session *ws;
};

extern std::vector<IClient> clientList;