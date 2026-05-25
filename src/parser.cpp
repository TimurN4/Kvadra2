#include <parser.hpp>
#include <fstream>
#include <thread>
#include <chrono>
#include <userver/formats/json.hpp>
#include <userver/engine/sleep.hpp>



namespace parser {

userver::formats::json::Value
Serialize(const Battery& battery, userver::formats::serialize::To<userver::formats::json::Value>) {
    userver::formats::json::ValueBuilder ans;
    ans["power"] = battery.power + "%";
    ans["is_charging"] = battery.charging_status;
    return ans.ExtractValue();
}

userver::formats::json::Value
Serialize(const CpuTicks& tick, userver::formats::serialize::To<userver::formats::json::Value>) {
    userver::formats::json::ValueBuilder ans;
    ans["name"] = tick.name;
    ans["user"] = tick.user;
    ans["nice"] = tick.nice;
    ans["system"] = tick.system;
    ans["idle"] = tick.idle;
    ans["iowait"] = tick.iowait;
    ans["irq"] = tick.irq;
    ans["softirq"] = tick.softirq;
    ans["steal"] = tick.steal;
    ans["guest"] = tick.guest;
    ans["guest_nice"] = tick.guest_nice;
    ans["cpu_usage"] = tick.cpu_usage;
    return ans.ExtractValue();
}

userver::formats::json::Value
Serialize(const RAM& ram, userver::formats::serialize::To<userver::formats::json::Value>) {
    userver::formats::json::ValueBuilder ans;
    ans["MemTotal"] = ram.MemTotal;
    ans["MemFree"] = ram.MemFree;
    ans["MemAvailable"] = ram.MemAvailable;
    ans["Buffers"] = ram.Buffers;
    ans["Cached"] = ram.Cached;
    ans["memory_usage"] = ram.memory_usage;
    return ans.ExtractValue();
}


// begin
namespace {

std::vector<CpuTicks> ReadProcStat() {
    std::ifstream file("/proc/stat");

    if (!file.is_open()) {
        throw std::runtime_error("cannot open /proc/stat");
    }

    std::vector<CpuTicks> cpus;

    std::string line;

    while (std::getline(file, line)) {
        if (!line.starts_with("cpu")) {
            break;
        }

        CpuTicks cpu;

        std::istringstream iss(line);

        iss >> cpu.name
            >> cpu.user
            >> cpu.nice
            >> cpu.system
            >> cpu.idle
            >> cpu.iowait
            >> cpu.irq
            >> cpu.softirq
            >> cpu.steal
            >> cpu.guest
            >> cpu.guest_nice;

        cpus.push_back(std::move(cpu));
    }

    return cpus;
}

unsigned long long GetIdle(const CpuTicks& cpu) {
    return cpu.idle + cpu.iowait;
}

unsigned long long GetTotal(const CpuTicks& cpu) {
    return
        cpu.user +
        cpu.nice +
        cpu.system +
        cpu.idle +
        cpu.iowait +
        cpu.irq +
        cpu.softirq +
        cpu.steal;
}

double CalculateUsage(const CpuTicks& prev, const CpuTicks& curr) {
    const auto prev_idle = GetIdle(prev);
    const auto curr_idle = GetIdle(curr);

    const auto prev_total = GetTotal(prev);
    const auto curr_total = GetTotal(curr);

    const auto total_delta = curr_total - prev_total;
    const auto idle_delta = curr_idle - prev_idle;

    if (total_delta == 0) {
        return 0.0;
    }

    return static_cast<double>(total_delta - idle_delta) / static_cast<double>(total_delta) * 100.0;
}

} // end namespace

void parseCPU(userver::formats::json::ValueBuilder& json) {

    auto prev = ReadProcStat();

    // std::this_thread::sleep_for(
    //     std::chrono::milliseconds(100)
    // );

    //std::chrono::seconds timer(1);
    userver::engine::InterruptibleSleepFor(std::chrono::milliseconds(100));

    auto curr = ReadProcStat();

    userver::formats::json::ValueBuilder cpu_array(
        userver::formats::common::Type::kArray
    );

    const auto count =
        std::min(prev.size(), curr.size());

    for (size_t i = 0; i < count; ++i) {
        curr[i].cpu_usage = CalculateUsage(prev[i], curr[i]);
        cpu_array.PushBack(curr[i]);
    }

    json["CPU"] = cpu_array.ExtractValue();
}

// end

namespace {

void fillRAMfild(RAM& ram, std::string& field_name, unsigned long long value) {
    if(field_name == "MemTotal") {
        ram.MemTotal = value;
    } else if(field_name == "MemFree") {
        ram.MemFree = value;
    } else if(field_name == "MemAvailable") {
        ram.MemAvailable = value;
    } else if(field_name == "Buffers") {
        ram.Buffers = value;
    } else if (field_name == "Cached") {
        ram.Cached = value;
    }
}

} // end namespace

void parseRAM(userver::formats::json::ValueBuilder& json) {
    RAM ram;
    std::string line;
    std::ifstream f("/proc/meminfo");
    if(f.is_open()) {
        while(getline(f, line) && line.find("SwapCached") == std::string::npos) {
            std::stringstream ss(line);
            unsigned long long num;
            ss >> line >> num; line.pop_back();
            fillRAMfild(ram, line, num);
        }
        if (ram.MemTotal > 0) {
            ram.memory_usage = 100.0 * (ram.MemTotal - ram.MemAvailable) / ram.MemTotal;
        } else {
            ram.memory_usage = 0.0;
        } // was wrong (ram.MemFree / ram.MemTotal)
    }

    json["RAM"] = ram;
}

void parseBattery(userver::formats::json::ValueBuilder& json) {
    Battery battery;
    std::ifstream f;
    
    f.open("/sys/class/power_supply/BAT1/capacity");
    if(f.is_open()) {
        std::getline(f, battery.power);
        f.clear();
        f.close();
    }

    f.open("/sys/class/power_supply/BAT1/status");
    if(f.is_open()) {
        std::getline(f, battery.charging_status);
        f.clear();
        f.close();
    }

    json["battery"] = battery;
}

void parse(userver::formats::json::ValueBuilder& json) {
    parseCPU(json);
    parseRAM(json);
    parseBattery(json);
}

} // end namespace parser