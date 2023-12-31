# CppTaskAppPortgres

Этот проект представляет собой простой консольный менеджер задач с интеграцией с базой данных PostgreSQL. Он позволяет пользователям добавлять, просматривать, обновлять статус, удалять и обновлять задачи. Проект разработан на C++ с использованием библиотеки libpqxx для взаимодействия с PostgreSQL и включает в себя консольный пользовательский интерфейс для взаимодействия с функциональностью менеджера задач.

- Добавление задачи: Пользователь может добавить новую задачу, указав её описание и срок выполнения (дедлайн).

- Просмотр списка задач: Пользователь может просмотреть список задач, отсортированный по дедлайну. Для каждой задачи выводится её описание, дедлайн и текущий статус.

- Обновление статуса задачи: Пользователь может обновить статус задачи на "completed", "in progress" или "uncompleted".

- Удаление задачи: Пользователь может удалить задачу по её идентификатору.

- Обновление задачи: Пользователь может обновить описание и дедлайн существующей задачи.

- Оформление интерфейса, пользователю предоставляется возможность взаимодействовать с менеджером задач через красочные рамки и цветовую кодировку, что облегчает ориентацию и понимание доступных действий.

Пример использования:

![Пример](images/Example.png)

## Структура базы данных

Для хранения задач и их связи с пользователями используется две таблицы:

### Таблица tasks

Структура таблицы tasks:

| Столбец     | Тип      | Допустимость NULL | По умолчанию |
|-------------|----------|-------------------|--------------|
| id          | integer  | not null          | nextval('tasks_id_seq'::regclass) |
| description | text     |                   |              |
| deadline    | date     |                   |              |
| status      | integer  |                   | 1            |

Индексы:
- "tasks_pkey" PRIMARY KEY, btree (id)

### Таблица task_mapping

Структура таблицы task_mapping:

| Столбец | Тип     | Допустимость NULL | По умолчанию |
|---------|---------|-------------------|--------------|
| real_id | integer |                   |              |
| user_id | integer | not null          | nextval('task_mapping_user_id_seq'::regclass) |

Индексы:
- "task_mapping_pkey" PRIMARY KEY, btree (user_id)
  
Ограничения внешнего ключа:
- "task_mapping_real_id_fkey" FOREIGN KEY (real_id) REFERENCES tasks(id)

## Создание таблиц

Для создания таблиц в PostgreSQL можно использовать следующие SQL-команды:

```sql
CREATE TABLE tasks (
  id serial PRIMARY KEY,
  description text,
  deadline date,
  status integer DEFAULT 1
);

CREATE TABLE task_mapping (
  real_id integer REFERENCES tasks(id),
  user_id serial PRIMARY KEY
);
```
## Сборка

Для сборки проекта потребуются библиотека libpqxx

Команда для установки на Linux:

```
sudo apt install libpqxx-dev
```

Необходимо будет создать свою базу данных, таблицы(примеры выше) и в функции main ввести имя базы, имя пользователя и пароль.

В проекте также реализован механизм автоматического форматирования и проверки стиля кода с использованием инструмента clang-format. Для этого включены цели format и check-style в CMake-скрипт.

```
cd build
cmake ..
make
./CppBoostedTaskApp
```

Enjoy!
