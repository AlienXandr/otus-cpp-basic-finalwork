#include "IJsonMsg.h"

// ================= DeviceMode ==============
void to_json(json &j, const DeviceMode &s) {
  if (s == DeviceMode::on)
    j = "on";
  if (s == DeviceMode::off)
    j = "off";
};
void from_json(const json &j, DeviceMode &s) {
  if (j == "on")
    s = DeviceMode::on;
  if (j == "off")
    s = DeviceMode::off;
}

// ================= IDeviceState ==============
// GUI read|write
void to_json(json &j, const IDeviceState &s) {
  j = json{{"mode", s.mode},
           {"shutdown", s.shutdown},
           {"freqCenter", s.freqCenter},
           {"sampleRateIdx", s.sampleRateIdx},
           {"fftSizeIdx", s.fftSizeIdx},
           {"gainChan0", s.gainChan0},
           {"gainChan1", s.gainChan1}};
};
void from_json(const json &j, IDeviceState &s) {
  if (j.contains("mode"))
    j.at("mode").get_to(s.mode);
  if (j.contains("shutdown"))
    j.at("shutdown").get_to(s.shutdown);
  if (j.contains("freqCenter"))
    j.at("freqCenter").get_to(s.freqCenter);
  if (j.contains("sampleRateIdx"))
    j.at("sampleRateIdx").get_to(s.sampleRateIdx);
  if (j.contains("fftSizeIdx"))
    j.at("fftSizeIdx").get_to(s.fftSizeIdx);
  if (j.contains("gainChan0"))
    j.at("gainChan0").get_to(s.gainChan0);
  if (j.contains("gainChan1"))
    j.at("gainChan1").get_to(s.gainChan1);
};

// ================= LogMsgType ==============
void to_json(json &j, const LogMsgType &s) {
  if (s == LogMsgType::info)
    j = "info";
  if (s == LogMsgType::warning)
    j = "warning";
  if (s == LogMsgType::error)
    j = "error";
};

// ================= IDeviceLogMsg ==============
// GUI only read
void to_json(json &j, const IDeviceLogMsg &s) {
  j = json{{"type", s.type}, {"date", s.date}, {"message", s.message}};
};
// void from_json(const json &j, IDeviceLogMsg &s) {};

// ================= IChartAxisLimits ==============
// GUI only read
void to_json(json &j, const IChartAxisLimits &s) {
  j = json{{"samplesChan0", s.samplesChan0},
           {"samplesChan1", s.samplesChan1},
           {"spectrumChan0", s.spectrumChan0},
           {"spectrumChan1", s.spectrumChan1},
           {"crossSpectrum", s.crossSpectrum}};
};
// void from_json(const json &j, IChartAxisLimits &s) {};

// ================= IChart ==============
// GUI only write
// void to_json(json &j, const IChart &s) {};
void from_json(const json &j, IChart &s) {
  if (j.contains("show"))
    j.at("show").get_to(s.show);
  if (j.contains("chartWidth"))
    j.at("chartWidth").get_to(s.chartWidth);
  if (j.contains("axisInterval")) {
    j.at("axisInterval").get_to(s.axisInterval);
  }
  if (j.contains("metadata"))
    j.at("metadata").get_to(s.metadata);
};

// ================= IChartState ==============
// GUI only write
// void to_json(json &j, const IChartState &s) {};
void from_json(const json &j, IChartState &s) {
  if (j.contains("samplesChan0"))
    j.at("samplesChan0").get_to(s.samplesChan0);
  if (j.contains("samplesChan1"))
    j.at("samplesChan1").get_to(s.samplesChan1);
  if (j.contains("spectrumChan0"))
    j.at("spectrumChan0").get_to(s.spectrumChan0);
  if (j.contains("spectrumChan1"))
    j.at("spectrumChan1").get_to(s.spectrumChan1);
  if (j.contains("crossSpectrum"))
    j.at("crossSpectrum").get_to(s.crossSpectrum);
  if (j.contains("phaseSpectrum"))
    j.at("phaseSpectrum").get_to(s.phaseSpectrum);
  if (j.contains("bearing"))
    j.at("bearing").get_to(s.bearing);
};
