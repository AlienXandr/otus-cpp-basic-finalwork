#pragma once

#include "interfaces/IJsonMsg.h"
#include <chrono>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

using json = nlohmann::json;

class websocket_session;

class DevEmulator {
public:
  DevEmulator();
  void run();
  void setDeviceState(const json &data);
  void setChartState(const json &data, websocket_session *ws);
  const IDeviceState &getDeviceState();
  const std::vector<IDeviceLogMsg> &getDeviceLog();
  const IChartAxisLimits &getChartAxisLimits();

private:
  void sendSamplesChan() const;
  void sendSpectrumChan() const;
  void sendCrossSpectrum() const;
  void sendPhaseSpectrum() const;
  void sendBearing() const;
  void sendDeviceLog();

private:
  std::mutex mtx_;

  float freqCenter_;
  float sampleRate_;
  float dF_;
  float fMin_;
  float fMax_;
  int fftSize_;
  float fStep_;
  float timeStep_;
  float timeMin_;
  float timeMax_;

  size_t frame_;

  IDeviceState deviceState_;
  IChartAxisLimits chartAxisLimits_;
  std::vector<IDeviceLogMsg> deviceLog_;
};

extern DevEmulator emulator;