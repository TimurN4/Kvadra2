#pragma once
#include <userver/formats/json_fwd.hpp>
#include <string>


namespace parser {

struct Battery {
    std::string power;
    std::string charging_status;
};

struct CpuTicks {
    std::string name;
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long guest_nice;

    double cpu_usage;
};

struct RAM {
    unsigned long long MemTotal;
    unsigned long long MemFree;
    unsigned long long MemAvailable;
    unsigned long long Buffers;
    unsigned long long Cached;

    double memory_usage;
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

