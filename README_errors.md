## Устаноовка userver 

### Подгружаем официальный репо
1) git clone https://github.com/userver-framework/userver.git && cd userver

### Создание своего сервиса через готовый скрипт из официального репо 
Нужно запустить скрипт ./service_template/userver-create-service.sh myservice который просто запускает userver-create-service.py(там была проблема с путями поэтому я сразу запустил этот файл .py)
2) python3 scripts/userver-create-service.py Kvadra2_service

# Ошибки

1) я билдил проект как make build-debug/ что неправильно поэтому и бинарник не собирался 

ПРАВИЛЬНО СОБИРАТЬ ТАК: make build-debug

2) 

# TO DO

1) ДОбавить проверки чтения в парсинге(учесть общий вид файла)

2) Переписать парсинг CPU самому


# Устанавливаем чистую Node.js (npm установится автоматически вместе с ней)
curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -

sudo apt install -y nodejs

npm create vite@latest sys-monitor -- --template vanilla-ts

# 1. Заходим в уже созданную папку проекта
cd sys-monitor

# 2. Запускаем чистую установку зависимостей вручную.
# Флаги отключают лишние сетевые проверки аудита, чтобы ничего не зависало.
npm install --no-audit --no-fund


npm run dev -- --host