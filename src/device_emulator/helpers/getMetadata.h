#pragma once

#include "getRandomData.h"
#include <string>

std::string getMetadata() {
  auto rnd = getRandomData<float>(0, 1, 4);
  return std::string{"meta1: "} + std::to_string(rnd[0]) +
         std::string{" meta2: "} + std::to_string(rnd[1]) +
         std::string{"\nmeta3: "} + std::to_string(rnd[2]) +
         std::string{"\nmeta4: "} + std::to_string(rnd[3]);
};
