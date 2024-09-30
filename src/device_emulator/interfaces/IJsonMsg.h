#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;

// ================= DeviceMode ==============
enum class DeviceMode { on, off };
void to_json(json &j, const DeviceMode &s);
void from_json(const json &j, DeviceMode &s);

// ================= IDeviceState ==============
// GUI read|write
struct IDeviceState {
  DeviceMode mode;
  bool shutdown;
  float freqCenter;
  uint8_t sampleRateIdx;
  uint8_t fftSizeIdx;
  uint8_t gainChan0;
  uint8_t gainChan1;
};
void to_json(json &j, const IDeviceState &s);
void from_json(const json &j, IDeviceState &s);

// ================= LogMsgType ==============
enum class LogMsgType { info, warning, error };
void to_json(json &j, const LogMsgType &s);

// ================= IDeviceLogMsg ==============
// GUI only read
struct IDeviceLogMsg {
  LogMsgType type;
  uint64_t date;
  std::string message;
};
void to_json(json &j, const IDeviceLogMsg &s);
// void from_json(const json &j, IDeviceLogMsg &s);

// ================= IChartAxisLimits ==============
// GUI only read
struct IChartAxisLimits {
  float samplesChan0[2] = {0.0, 0.0};
  float samplesChan1[2] = {0.0, 0.0};
  float spectrumChan0[2] = {0.0, 0.0};
  float spectrumChan1[2] = {0.0, 0.0};
  float crossSpectrum[2] = {0.0, 0.0};
};
void to_json(json &j, const IChartAxisLimits &s);
// void from_json(const json &j, IChartAxisLimits &s);

// ================= IChart ==============
// GUI only write
struct IChart {
  bool show = false;
  uint16_t chartWidth = 0;
  float axisInterval[2] = {0.0, 0.0};
  bool metadata = false;
};
// void to_json(json &j, const IChart &s);
void from_json(const json &j, IChart &s);

// ================= IChartState ==============
// GUI only write
struct IChartState {
  IChart samplesChan0;
  IChart samplesChan1;
  IChart spectrumChan0;
  IChart spectrumChan1;
  IChart crossSpectrum;
  IChart phaseSpectrum;
  IChart bearing;
};
// void to_json(json &j, const IChartState &s);
void from_json(const json &j, IChartState &s);
