# Kvadra2_service

Шаблон C++ сервиса который использует [userver framework](https://github.com/userver-framework/userver).

# Download and Build

Чтобы использовать проект нужно иметь docker, все зависимости уже установлены в devcontainer.

### Клонирование репозитория
1. git clone https://github.com/TimurN4/Kvadra2.git && cd Kvadra2_service

### Запуск проекта в devcontainer(VS CODE)
2. ctrl + shirft + P и в панели выбрать Dev Containers: Reopen in Container

## Настройки BACKEND

### Создаем дерево проекта 
1. make cmake-debug

### Сборка проекта
2. make build-debug

### Запуск сервера
3. ./build-debug/Kvadra2_service --config ./configs/static_config.yaml --config_vars ./configs/config_vars.yaml

## Настройки FRONTEND
## Запускаем новый терминал

+ Изначально в dev container не было Node.js поэтому сначала скачаем его 
### Настраиваем официальный репозиторий Node.js версии 20.x
1. curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -

### Устанавливаем Node.js в Linux 
2. sudo apt install -y nodejs

### Устанавливаем зависимости проекта
3. cd frontend && npm install --no-audit --no-fund

### Запускаем FRONTEND
4. npm run dev -- --host

### Открываем страничку с метриками в браузере
5. http://localhost:5173/

P.S: Если npm run dev -- --host перестал работать нужно пересобрать проект: npm install --no-audit --no-fund(из папки frontend) а потом опять npm run dev -- --host

# Описание проекта 

System Resource Monitor
Приложение для мониторинга состояния ресурсов PC (CPU, RAM, Battery) в реальном времени, реализованное по архитектуре Client-Server.

🛠 Архитектурные особенности

Backend: Разработан на языке C++ с использованием фреймворка USERVER(современный открытый асинхронный фреймворк с богатым набором абстракции для быстрого и комфортного создания микросервисов на C++)

+ Реализована серверная часть на базе WebSocketHandlerBase для обеспечения двусторонней связи и потоковой передачи телеметрии.

+ Сбор данных о системе осуществляется через парсинг файлов в виртуальной файловой системе /proc/ и /sys/class/power_supply/ в отдельном fs-task-processor, что исключает блокировку основного потока обработки запросов.

Frontend: Реализован на стеке HTML5 + CSS3 + TypeScript с использованием сборщика Vite.

+ Используется строгая типизация данных (интерфейсы SystemData) для корректной обработки JSON-ответов от бэкенда.

+ Динамическое обновление UI без перезагрузки страницы через WebSocket API.

IPC: Для передачи данных между бэкендом и фронтендом используется протокол WebSocket, обеспечивающий минимальную задержку при обновлении метрик.

## License

The original template is distributed under the [Apache-2.0 License](https://github.com/userver-framework/userver/blob/develop/LICENSE)
and [CLA](https://github.com/userver-framework/userver/blob/develop/CONTRIBUTING.md). Services based on the template may change
the license and CLA.
