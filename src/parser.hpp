#pragma once
#include <userver/formats/json_fwd.hpp>
#include <string>


namespace parser {

struct Battery {
    std::string power{};
    std::string charging_status{};
};

struct CpuTicks {
    std::string name{};
    unsigned long long user{0};
    unsigned long long nice{0};
    unsigned long long system{0};
    unsigned long long idle{0};
    unsigned long long iowait{0};
    unsigned long long irq{0};
    unsigned long long softirq{0};
    unsigned long long steal{0};
    unsigned long long guest{0};
    unsigned long long guest_nice{0};

    double cpu_usage{0};
};

struct RAM {
    unsigned long long MemTotal{0};
    unsigned long long MemFree{0};
    unsigned long long MemAvailable{0};
    unsigned long long Buffers{0};
    unsigned long long Cached{0};

    double memory_usage{0.0};
};

userver::formats::json::Value
Serialize(const Battery& battery, userver::formats::serialize::To<userver::formats::json::Value>);

userver::formats::json::Value
Serialize(const CpuTicks& tick, userver::formats::serialize::To<userver::formats::json::Value>);

userver::formats::json::Value
Serialize(const RAM& ram, userver::formats::serialize::To<userver::formats::json::Value>);

void parseCPU(userver::formats::json::ValueBuilder& json);

void parseRAM(userver::formats::json::ValueBuilder& json);

void parseBattery(userver::formats::json::ValueBuilder& json);

void parse(userver::formats::json::ValueBuilder& json);

} // end namespace parser

