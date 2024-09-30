#include "device_emulator.hpp"
#include "../server/clientList.h"
#include "../server/server.hpp"
#include "helpers/getMetadata.h"
#include "helpers/getRandomData.h"
#include "interfaces/IBinaryMsg.h"

DevEmulator::DevEmulator() {
  freqCenter_ = 1500e6;
  sampleRate_ = 61.44e6;
  dF_ = sampleRate_ / 2;
  fMin_ = freqCenter_ - dF_ / 2;
  fMax_ = freqCenter_ + dF_ / 2;

  fftSize_ = 128;
  fStep_ = dF_ / static_cast<float>(fftSize_);

  timeStep_ = 1 / dF_;
  timeMin_ = 0.0;
  timeMax_ = timeStep_ * static_cast<float>(fftSize_);

  frame_ = 0;

  deviceState_.mode = DeviceMode::off;
  deviceState_.shutdown = false;
  deviceState_.freqCenter = freqCenter_;
  deviceState_.sampleRateIdx = 0;
  deviceState_.fftSizeIdx = 0;
  deviceState_.gainChan0 = 12;
  deviceState_.gainChan1 = 15;

  chartAxisLimits_.samplesChan0[0] = timeMin_;
  chartAxisLimits_.samplesChan0[1] = timeMax_;
  chartAxisLimits_.samplesChan1[0] = timeMin_;
  chartAxisLimits_.samplesChan1[1] = timeMax_;
  chartAxisLimits_.spectrumChan0[0] = fMin_;
  chartAxisLimits_.spectrumChan0[1] = fMax_;
  chartAxisLimits_.spectrumChan1[0] = fMin_;
  chartAxisLimits_.spectrumChan1[1] = fMax_;
  chartAxisLimits_.crossSpectrum[0] = fMin_;
  chartAxisLimits_.crossSpectrum[1] = fMax_;
}

void DevEmulator::setDeviceState(const json &data) {
  std::lock_guard lock{mtx_};
  data.get_to(deviceState_);
  std::cout << "DevEmulator::setDeviceState: Получено новое состояние: "
            << data.dump(4) << '\n';

  if (deviceState_.shutdown) {
    std::cout << "DevEmulator::setDeviceState: Завершение работы" << '\n';
    exit(0);
  }

  // Broadcast for all clients
  auto str = json{{"deviceState", data}}.dump();
  auto buff = std::make_shared<std::vector<char>>(str.begin(), str.end());
  for (auto &client : clientList) {
    client.ws->send(buff, buff->size(), true);
  }
}

void DevEmulator::setChartState(const json &data, websocket_session *ws) {
  std::lock_guard lock{mtx_};
  std::cout << "DevEmulator::setChartState: Получено новое состояние: "
            << data.dump(4) << '\n';

  for (auto &client : clientList) {
    if (client.ws == ws) {
      data.get_to(client.chartState);
      break;
    }
  }
}

const IDeviceState &DevEmulator::getDeviceState() { return deviceState_; }
const std::vector<IDeviceLogMsg> &DevEmulator::getDeviceLog() {
  return deviceLog_;
}
const IChartAxisLimits &DevEmulator::getChartAxisLimits() {
  return chartAxisLimits_;
};

void DevEmulator::sendSamplesChan() const {
  // Broadcast for all clients
  for (auto &client : clientList) {
    if (client.chartState.samplesChan0.show) {
      ISamplesChan samplesChan0;
      samplesChan0.code = 0;
      samplesChan0.timeStart = timeMin_;
      samplesChan0.timeStep = timeStep_;
      samplesChan0.sizeArray = fftSize_;
      samplesChan0.re = getRandomData<int16_t>(-1000, 1000, fftSize_);
      samplesChan0.im = getRandomData<int16_t>(-1000, 1000, fftSize_);
      if (client.chartState.samplesChan0.metadata) {
        samplesChan0.metadata = getMetadata();
      }
      samplesChan0.sizeMetadata = samplesChan0.metadata.size();

      auto buffer1 =
          std::make_shared<std::vector<char>>(samplesChan0.serialize());
      client.ws->send(buffer1, buffer1->size(), false);
    }
    if (client.chartState.samplesChan1.show) {
      ISamplesChan samplesChan1;
      samplesChan1.code = 1;
      samplesChan1.timeStart = timeMin_;
      samplesChan1.timeStep = timeStep_;
      samplesChan1.sizeArray = fftSize_;
      samplesChan1.re = getRandomData<int16_t>(-500, 500, fftSize_);
      samplesChan1.im = getRandomData<int16_t>(-500, 500, fftSize_);
      if (client.chartState.samplesChan1.metadata) {
        samplesChan1.metadata = getMetadata();
      }
      samplesChan1.sizeMetadata = samplesChan1.metadata.size();

      auto buffer2 =
          std::make_shared<std::vector<char>>(samplesChan1.serialize());
      client.ws->send(buffer2, buffer2->size(), false);
    }
  }
};

void DevEmulator::sendSpectrumChan() const {
  // Broadcast for all clients
  for (auto &client : clientList) {
    if (client.chartState.spectrumChan0.show) {
      ISpectrumChan spectrumChan0;
      spectrumChan0.code = 2;
      spectrumChan0.fStart = fMin_;
      spectrumChan0.fStep = fStep_;
      spectrumChan0.sizeArray = fftSize_;
      spectrumChan0.spectrum = getRandomData<int8_t>(-10, 10, fftSize_);
      if (client.chartState.spectrumChan0.metadata) {
        spectrumChan0.metadata = getMetadata();
      }
      spectrumChan0.sizeMetadata = spectrumChan0.metadata.size();

      auto buffer1 =
          std::make_shared<std::vector<char>>(spectrumChan0.serialize());
      client.ws->send(buffer1, buffer1->size(), false);
    }
    if (client.chartState.spectrumChan1.show) {
      ISpectrumChan spectrumChan1;
      spectrumChan1.code = 3;
      spectrumChan1.fStart = fMin_;
      spectrumChan1.fStep = fStep_;
      spectrumChan1.sizeArray = fftSize_;
      spectrumChan1.spectrum = getRandomData<int8_t>(40, 50, fftSize_);
      if (client.chartState.spectrumChan1.metadata) {
        spectrumChan1.metadata = getMetadata();
      }
      spectrumChan1.sizeMetadata = spectrumChan1.metadata.size();

      auto buffer2 =
          std::make_shared<std::vector<char>>(spectrumChan1.serialize());
      client.ws->send(buffer2, buffer2->size(), false);
    }
  }
}

void DevEmulator::sendCrossSpectrum() const {
  // Broadcast for all clients
  for (auto &client : clientList) {
    if (client.chartState.crossSpectrum.show) {
      ICrossSpectrum crossSpectrum;
      crossSpectrum.code = 4;
      crossSpectrum.date = std::chrono::system_clock::now().time_since_epoch() /
                           std::chrono::milliseconds(1);
      crossSpectrum.fStart = fMin_;
      crossSpectrum.fStep = fStep_;
      crossSpectrum.sizeArray = fftSize_;
      crossSpectrum.spectrum = getRandomData<int8_t>(-20, -10, fftSize_);
      if (client.chartState.crossSpectrum.metadata) {
        crossSpectrum.metadata = getMetadata();
      }
      crossSpectrum.sizeMetadata = crossSpectrum.metadata.size();

      auto buffer =
          std::make_shared<std::vector<char>>(crossSpectrum.serialize());
      client.ws->send(buffer, buffer->size(), false);
    }
  }
}

void DevEmulator::sendPhaseSpectrum() const {
  // Broadcast for all clients
  for (auto &client : clientList) {
    if (client.chartState.phaseSpectrum.show) {
      IPhaseSpectrum phaseSpectrum;
      phaseSpectrum.code = 5;
      phaseSpectrum.fStart = fMin_;
      phaseSpectrum.fStep = fStep_;
      phaseSpectrum.sizeArray = fftSize_;
      phaseSpectrum.phase = getRandomData<int16_t>(-180, 180, fftSize_);
      if (client.chartState.phaseSpectrum.metadata) {
        phaseSpectrum.metadata = getMetadata();
      }
      phaseSpectrum.sizeMetadata = phaseSpectrum.metadata.size();

      auto buffer =
          std::make_shared<std::vector<char>>(phaseSpectrum.serialize());
      client.ws->send(buffer, buffer->size(), false);
    }
  }
}

void DevEmulator::sendBearing() const {
  // Broadcast for all clients
  for (auto &client : clientList) {
    IBearing bearing;
    bearing.code = 6;
    bearing.bearing = getRandomData<float>(123, 132, 1)[0];
    bearing.bearingQuality = getRandomData<uint8_t>(5, 100, 1)[0];
    bearing.bearingStd = getRandomData<float>(3, 15, 1)[0];
    bearing.sizeArrayDistribution = 7;
    bearing.ampDistribution = getRandomData<int16_t>(-10, 10, 7);
    bearing.phaseDistribution = getRandomData<int16_t>(-180, 180, 7);
    bearing.beamPatternStep = 0.5;
    bearing.sizeArrayBeamPattern = (uint16_t)(360. / bearing.beamPatternStep);
    bearing.beamPattern =
        getRandomData<float>(0.1, 1, bearing.sizeArrayBeamPattern);
    if (client.chartState.bearing.metadata) {
      bearing.metadata = getMetadata();
    }
    bearing.sizeMetadata = bearing.metadata.size();

    auto buffer = std::make_shared<std::vector<char>>(bearing.serialize());
    client.ws->send(buffer, buffer->size(), false);
  }
}

void DevEmulator::sendDeviceLog() {
  std::vector<IDeviceLogMsg> deviceLog;

  IDeviceLogMsg msg;
  msg.type = LogMsgType::info;
  msg.date = std::chrono::system_clock::now().time_since_epoch() /
             std::chrono::milliseconds(1);
  msg.message = std::string{"Номер фрейма "} + std::to_string(frame_) +
                std::string{". Получены настройки от пользователя для "
                            "обнаружителя"};
  deviceLog.push_back(msg);

  msg.type = LogMsgType::warning;
  msg.date = std::chrono::system_clock::now().time_since_epoch() /
             std::chrono::milliseconds(1);
  msg.message = std::string{"Номер фрейма "} + std::to_string(frame_) +
                std::string{". Полученные настройки от пользователя превышают "
                            "допустимые значения"};
  deviceLog.push_back(msg);

  msg.type = LogMsgType::error;
  msg.date = std::chrono::system_clock::now().time_since_epoch() /
             std::chrono::milliseconds(1);
  msg.message =
      std::string{"Номер фрейма "} + std::to_string(frame_) +
      std::string{
          ". Обнаружитель: COM-порт не отвечает после десяти попыток запроса"};
  deviceLog.push_back(msg);

  // Broadcast for all clients
  auto str = json{{"deviceLog", deviceLog}}.dump();
  auto buff = std::make_shared<std::vector<char>>(str.begin(), str.end());
  for (auto &client : clientList) {
    client.ws->send(buff, buff->size(), true);
  }

  deviceLog_ = std::move(deviceLog);
}

void DevEmulator::run() {
  std::cout << "DevEmulator TID: " << std::this_thread::get_id()
            << " started\n";

  while (!deviceState_.shutdown) {
    if (deviceState_.mode != DeviceMode::off) {
      sendSamplesChan();
      sendSpectrumChan();
      sendCrossSpectrum();
      sendPhaseSpectrum();
      sendBearing();

      frame_ += 1;
      if (frame_ % 50 == 0) {
        sendDeviceLog();
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  };
}

//  // ============================================
//  Res response;
//
//  auto buffer = std::make_shared<std::vector<char>>(response.serialize());
//
//  //  ws->send(buffer, buffer->size(), false);
//  for (auto &client : clientList) {
//    client.ws->send(buffer, buffer->size(), false);
//  }
//
//  struct D {
//    void operator()(std::vector<char> *p) const {
//      std::cout << "Call delete from function object" << '\n';
//      delete p;
//    }
//  };
//
//  auto str = data.dump();
//  //  std::shared_ptr<std::vector<char>> buffer2(
//  //      new std::vector<char>(str.begin(), str.end()), D());
//  auto buffer2 = std::make_shared<std::vector<char>>(str.begin(),
//  str.end());
//
//  for (auto &client : clientList) {
//    client.ws->send(buffer2, buffer2->size(), true);
//  }