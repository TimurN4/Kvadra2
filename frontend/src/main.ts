// src/main.ts
import './style.css';
// Исправление ошибки verbatimModuleSyntax: добавляем ключевое слово 'type'
import type { SystemData } from './types';

// Динамически определяем хост (localhost или IP WSL), чтобы не было проблем с портами
const WS_URL = `ws://${window.location.hostname}:8080/ws`; 

console.log('Попытка подключения к WebSocket по адресу:', WS_URL);

// DOM Элементы
const statusEl = document.getElementById('connection-status');
const cpuContainer = document.getElementById('cpu-container');
const ramProgress = document.getElementById('ram-progress') as HTMLProgressElement | null;
const ramText = document.getElementById('ram-text');
const ramTotal = document.getElementById('ram-total');
const ramAvailable = document.getElementById('ram-available');
const batteryLevel = document.getElementById('battery-level');
const batteryStatus = document.getElementById('battery-status');

let socket: WebSocket | null = null;
let isReconnecting = false;

const kbToGb = (kb: number): string => (kb / 1024 / 1024).toFixed(2);

function updateUI(data: SystemData) {
    if (!cpuContainer || !ramProgress || !ramText || !ramTotal || !ramAvailable || !batteryLevel || !batteryStatus) {
        console.warn('Не все элементы UI найдены на странице. Проверьте index.html');
        return;
    }

    // 1. Обновление CPU
    cpuContainer.innerHTML = '';
    data.CPU.forEach(core => {
        const usageText = core.cpu_usage.toFixed(1);
        const isHigh = core.cpu_usage > 85;

        const row = document.createElement('div');
        row.className = 'core-row';
        row.innerHTML = `
            <span class="core-name">${core.name}</span>
            <progress max="100" value="${usageText}" class="${isHigh ? 'high-usage' : ''}"></progress>
            <span class="value" style="width: 50px; text-align: right;">${usageText}%</span>
        `;
        cpuContainer.appendChild(row);
    });

    // 2. Обновление RAM
    const ramUsage = data.RAM.memory_usage.toFixed(1);
    ramProgress.value = parseFloat(ramUsage);
    ramProgress.className = parseFloat(ramUsage) > 85 ? 'high-usage' : '';
    ramText.textContent = `${ramUsage}%`;
    ramTotal.textContent = kbToGb(data.RAM.MemTotal);
    ramAvailable.textContent = kbToGb(data.RAM.MemAvailable);

    // 3. Обновление Батареи
    batteryLevel.textContent = data.battery.power.trim() || 'N/A';
    batteryStatus.textContent = data.battery.is_charging?.trim() || 'N/A';
}

function connectWebSocket() {
    socket = new WebSocket(WS_URL);

    socket.onopen = () => {
        console.log('Успешно подключено к C++ бэкенду');
        if (statusEl) {
            statusEl.textContent = 'Online';
            statusEl.className = 'online';
        }
        isReconnecting = false;
    };

    socket.onmessage = (event) => {
        try {
            const data: SystemData = JSON.parse(event.data);
            updateUI(data);
        } catch (err) {
            console.error('Ошибка парсинга JSON от бэкенда:', err);
        }
    };

    socket.onclose = () => {
        console.log('Соединение закрыто. Попытка переподключения...');
        if (statusEl) {
            statusEl.textContent = 'Reconnecting...';
            statusEl.className = 'offline';
        }
        socket = null;
        
        if (!isReconnecting) {
            isReconnecting = true;
            setTimeout(connectWebSocket, 2000);
        }
    };

    socket.onerror = (error) => {
        console.error('Ошибка WebSocket:', error);
        socket?.close();
    };
}

// Запуск
connectWebSocket();