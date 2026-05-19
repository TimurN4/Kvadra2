# Kvadra2_service

Шаблон C++ сервиса который использует [userver framework](https://github.com/userver-framework/userver).

## Download and Build

Чтобы использовать проект нужно иметь docker, все зависимости уже установлены в devcontainer.

### Клонирование репозитория
1. git clone https://github.com/TimurN4/Kvadra2.git && cd Kvadra2_service

### Запуск проекта в devcontainer(VS CODE)
2. ctrl + shirft + P и в панели выбрать Dev Containers: Reopen in Container

# Настройка BACKEND

### Создаем дерево проекта 
1. make cmake-debug

### Сборка проекта
2. make build-debug

### Запуск сервера
3. ./build-debug/Kvadra2_service --config ./configs/static_config.yaml --config_vars ./configs/config_vars.yaml

# Настройка FRONTEND
## Запускаем новый терминал

### Настраиваем официальный репозиторий Node.js версии 20.x
1. curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -

### Устанавливаем Node.js в Linux 
2. sudo apt install -y nodejs

### Устанавливаем зависимости проекта
3. npm install --no-audit --no-fund

### Запускаем FRONTEND
4. npm run dev -- --host

### Открываем страничку в браузере с метриками
5. http://localhost:5173/

P.S: Если npm run dev -- --host перестал работать нужно пересобрать проект: npm install --no-audit --no-fund а потом опять npm run dev -- --host



## License

The original template is distributed under the [Apache-2.0 License](https://github.com/userver-framework/userver/blob/develop/LICENSE)
and [CLA](https://github.com/userver-framework/userver/blob/develop/CONTRIBUTING.md). Services based on the template may change
the license and CLA.
