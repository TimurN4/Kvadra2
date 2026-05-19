// src/types.ts

export interface CpuTick {
    name: string;
    user: number;
    nice: number;
    system: number;
    idle: number;
    iowait: number;
    irq: number;
    softirq: number;
    steal: number;
    guest: number;
    guest_nice: number;
    cpu_usage: number; // Процент использования (0-100)
}

export interface RAM {
    MemTotal: number;      // В килобайтах (из /proc/meminfo)
    MemFree: number;
    MemAvailable: number;
    Buffers: number;
    Cached: number;
    memory_usage: number;  // Процент использования
}

export interface Battery {
    power: string;
    is_charging: string; // Изменили с charging_status на is_charging
}

export interface SystemData {
    CPU: CpuTick[];
    RAM: RAM;
    battery: Battery;
}