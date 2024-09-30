#pragma once

#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

struct SubData {
  float pi;
  bool happy;
  std::string name;
};
void from_json(const json &j, SubData &s) {
  if (j.contains("pi"))
    j.at("pi").get_to(s.pi);
  if (j.contains("happy"))
    j.at("happy").get_to(s.happy);
  if (j.contains("name"))
    j.at("name").get_to(s.name);
};

enum class Mode { on, off };
void from_json(const json &j, Mode &s) {
  if (j == "on")
    s = Mode::on;
  if (j == "off")
    s = Mode::off;
}

struct Data {
  Mode mode;
  std::string brtt;
  std::string country;
  std::string ip;
  uint64_t time;
  float limit[4] = {0.0, 0.0, 0.0, 0.0};
  SubData subData;
};
void to_json(json &j, const Data &s) {
  j = json{{"brtt", s.brtt},
           {"country", s.country},
           {"ip", s.ip},
           {"time", s.time},
           {"limit", s.limit}};
};
void from_json(const json &j, Data &s) {
  if (j.contains("mode"))
    j.at("mode").get_to(s.mode);
  if (j.contains("brtt"))
    j.at("brtt").get_to(s.brtt);
  if (j.contains("country"))
    j.at("country").get_to(s.country);
  if (j.contains("ip"))
    j.at("ip").get_to(s.ip);
  if (j.contains("time"))
    j.at("time").get_to(s.time);
  if (j.contains("limit")) {
    j.at("limit").get_to(s.limit);
    //    j.at("limit")[0].get_to(p.limit[0]);
    //    j.at("limit")[1].get_to(p.limit[1]);
  }
  if (j.contains("subData"))
    j.at("subData").get_to(s.subData);
};

void json_test() {
  std::string jsonStr = R"({
    "mode": "off",
    "brtt": "Testtt",
    "country": "RBRR",
    "ip": "192.168.0.0",
    "limit": [123.4458, 8965.6645, 12.01, 568.2],
    "subData": {
      "pi": 3.144,
      "happy": false
    }
  })";
  std::cout << "source string: " << jsonStr << std::endl;
  json js = json::parse(jsonStr);

  std::cout << "json string: " << js.dump(4) << std::endl;

  //  ns::Data d{js.get<ns::Data>()};
  Data d;
  d.time = 1111222;
  d.subData.name = "Alex";
  js.get_to(d);

  std::cout << d.brtt << " " << d.country << " " << d.ip << " " << d.time
            << std::endl;

  json js2 = d;
  std::cout << "json2 string: " << js2.dump(4) << std::endl;

  //  json data = R"({
  //    "pi": 3.141,
  //    "happy": true,
  //    "name": "Niels",
  //    "nothing": null,
  //    "answer": {
  //      "everything": 42
  //    },
  //    "list": [1, 0, 2],
  //    "object": {
  //      "currency": "USD",
  //      "value": 42.99
  //    }
  //  })"_json;
  //
  //  Data myData{data.get<Data>()};
  //
  //  float pi;
  //  data.at("pi").get_to(pi);
  //
  //  for (auto &[key, val] : data.items()) {
  //    std::cout << "key: " << key << " : " << val << std::endl;
  //    //    switch (key) { case "pi": };
  //
  //    if (val.is_structured()) {
  //      for (auto &[key2, val2] : val.items()) {
  //        std::cout << "key: " << key2 << " : " << val2 << std::endl;
  //      };
  //    }
  //  };
}