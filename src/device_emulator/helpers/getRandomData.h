#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <typename T>
std::vector<T> getRandomData(float min, float max, size_t size) {

  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};
  // Generates random integers
  std::uniform_real_distribution<float> dist{min, max};

  auto gen = [&]() { return dist(mersenne_engine); };

  std::vector<T> vec(size);
  std::generate(vec.begin(), vec.end(), gen);

  return vec;
}