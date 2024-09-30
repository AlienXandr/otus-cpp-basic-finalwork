#pragma once

#include <cstdint>
#include <string>
#include <vector>

void serializeField(std::vector<char> &buff, const char *ptr, size_t size) {
  for (size_t i = 0; i < size; i++) {
    buff.push_back(ptr[i]);
  }
}

#pragma pack(push, 1)
struct ISamplesChan {
  uint8_t code;       //  uint8 [0 | 1], Код посылки
  float timeStart;    // float32, Начальное время, с
  float timeStep;     // float32, Шаг по времени, с
  uint16_t sizeArray; // uint16, Размер массива
  uint8_t emptyByte; // uint8 Нужен для выравнивания до кратности 2ки
  std::vector<int16_t> re; // int16, Re отсчёты канала_0 (!!! Длина вектора
                           // всегда должна быть кратна 2)
  std::vector<int16_t> im; // int16, Im отсчёты канала_0 (!!! Длина вектора
                           // всегда должна быть кратна 2)
  uint16_t sizeMetadata; // uint16, Размер строки с метаданными
  std::string metadata; // string, Строка с метаданными в кодировке 'utf-8'

  std::vector<char> serialize() {
    std::vector<char> buff;
    serializeField(buff, (const char *)&code, sizeof(code));
    serializeField(buff, (const char *)&timeStart, sizeof(timeStart));
    serializeField(buff, (const char *)&timeStep, sizeof(timeStep));
    serializeField(buff, (const char *)&sizeArray, sizeof(sizeArray));
    serializeField(buff, (const char *)&emptyByte, sizeof(emptyByte));
    serializeField(buff, (const char *)&re[0], sizeof(int16_t) * re.size());
    serializeField(buff, (const char *)&im[0], sizeof(int16_t) * im.size());
    serializeField(buff, (const char *)&sizeMetadata, sizeof(sizeMetadata));
    serializeField(buff, (const char *)&metadata[0],
                   sizeof(char) * metadata.size());
    return buff;
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ISpectrumChan {
  uint8_t code; // uint8 [2 | 3], Код посылки
  float fStart; // float32, Начальная частота в спектре, Гц
  float fStep; // float32, Шаг по частоте в спектре, Гц
  uint16_t sizeArray;           // uint16, Размер массива
  std::vector<int8_t> spectrum; // int8, Спектр канала, дБ
  uint16_t sizeMetadata; // uint16, Размер строки с метаданными
  std::string metadata; // string, Строка с метаданными в кодировке 'utf-8'

  std::vector<char> serialize() {
    std::vector<char> buff;
    serializeField(buff, (const char *)&code, sizeof(code));
    serializeField(buff, (const char *)&fStart, sizeof(fStart));
    serializeField(buff, (const char *)&fStep, sizeof(fStep));
    serializeField(buff, (const char *)&sizeArray, sizeof(sizeArray));
    serializeField(buff, (const char *)&spectrum[0],
                   sizeof(int8_t) * spectrum.size());
    serializeField(buff, (const char *)&sizeMetadata, sizeof(sizeMetadata));
    serializeField(buff, (const char *)&metadata[0],
                   sizeof(char) * metadata.size());
    return buff;
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ICrossSpectrum {
  uint8_t code;  // uint8 [4], Код посылки
  uint64_t date; // uint64, Текущее время timeEPOX, мс
  float fStart; // float32, Начальная частота в спектре, Гц
  float fStep; // float32, Шаг по частоте в спектре, Гц
  uint16_t sizeArray;           // uint16, Размер массива
  std::vector<int8_t> spectrum; // int8, Спектр канала, дБ
  uint16_t sizeMetadata; // uint16, Размер строки с метаданными
  std::string metadata; // string, Строка с метаданными в кодировке 'utf-8'

  std::vector<char> serialize() {
    std::vector<char> buff;
    serializeField(buff, (const char *)&code, sizeof(code));
    serializeField(buff, (const char *)&date, sizeof(date));
    serializeField(buff, (const char *)&fStart, sizeof(fStart));
    serializeField(buff, (const char *)&fStep, sizeof(fStep));
    serializeField(buff, (const char *)&sizeArray, sizeof(sizeArray));
    serializeField(buff, (const char *)&spectrum[0],
                   sizeof(int8_t) * spectrum.size());
    serializeField(buff, (const char *)&sizeMetadata, sizeof(sizeMetadata));
    serializeField(buff, (const char *)&metadata[0],
                   sizeof(char) * metadata.size());
    return buff;
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct IPhaseSpectrum {
  uint8_t code; // uint8 [5], Код посылки
  float fStart; // float32, Начальная частота в спектре, Гц
  float fStep; // float32, Шаг по частоте в спектре, Гц
  uint16_t sizeArray; // uint16, Размер массива
  uint8_t emptyByte; // uint8, Нужен для выравнивания до кратности 2ки
  std::vector<int16_t> phase; // int16, Фаза, град
  uint16_t sizeMetadata; // uint16, Размер строки с метаданными
  std::string metadata; // string, Строка с метаданными в кодировке 'utf-8'

  std::vector<char> serialize() {
    std::vector<char> buff;
    serializeField(buff, (const char *)&code, sizeof(code));
    serializeField(buff, (const char *)&fStart, sizeof(fStart));
    serializeField(buff, (const char *)&fStep, sizeof(fStep));
    serializeField(buff, (const char *)&sizeArray, sizeof(sizeArray));
    serializeField(buff, (const char *)&emptyByte, sizeof(emptyByte));
    serializeField(buff, (const char *)&phase[0],
                   sizeof(int16_t) * phase.size());
    serializeField(buff, (const char *)&sizeMetadata, sizeof(sizeMetadata));
    serializeField(buff, (const char *)&metadata[0],
                   sizeof(char) * metadata.size());
    return buff;
  }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct IBearing {
  uint8_t code;           // uint8 [6], Код посылки
  float bearing;          // float32, Текущий пеленг, град
  uint8_t bearingQuality; // uint8, Качество текущего пеленга, %
  float bearingStd; // float32, СКО текущего пеленга, град
  uint16_t sizeArrayDistribution;         // uint16, Размер массива
  std::vector<int16_t> ampDistribution;   // int16, Амплитуда, дБ
  std::vector<int16_t> phaseDistribution; // int16, Фаза, град
  float beamPatternStep; // float32, Шаг по азимуту в ДНА, град
  uint16_t sizeArrayBeamPattern; // uint16, Размер массива
  uint8_t emptyBytes[2]{0, 0}; // uint8[2], Нужен для выравнивания до
                               // кратности 4ки
  std::vector<float> beamPattern; // float32, Диаграмма направленности,
                                  // коэф. корр.
  uint16_t sizeMetadata; // uint16, Размер строки с метаданными
  std::string metadata; // string, Строка с метаданными в кодировке 'utf-8'

  std::vector<char> serialize() {
    std::vector<char> buff;
    serializeField(buff, (const char *)&code, sizeof(code));
    serializeField(buff, (const char *)&bearing, sizeof(bearing));
    serializeField(buff, (const char *)&bearingQuality, sizeof(bearingQuality));
    serializeField(buff, (const char *)&bearingStd, sizeof(bearingStd));
    serializeField(buff, (const char *)&sizeArrayDistribution,
                   sizeof(sizeArrayDistribution));
    serializeField(buff, (const char *)&ampDistribution[0],
                   sizeof(int16_t) * ampDistribution.size());
    serializeField(buff, (const char *)&phaseDistribution[0],
                   sizeof(int16_t) * phaseDistribution.size());
    serializeField(buff, (const char *)&beamPatternStep,
                   sizeof(beamPatternStep));
    serializeField(buff, (const char *)&sizeArrayBeamPattern,
                   sizeof(sizeArrayBeamPattern));
    serializeField(buff, (const char *)&emptyBytes[0], sizeof(uint8_t) * 2);
    serializeField(buff, (const char *)&beamPattern[0],
                   sizeof(float) * beamPattern.size());
    serializeField(buff, (const char *)&sizeMetadata, sizeof(sizeMetadata));
    serializeField(buff, (const char *)&metadata[0],
                   sizeof(char) * metadata.size());
    return buff;
  }
};
#pragma pack(pop)