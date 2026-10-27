# c_coursework

Решения задач с курса "Практика языка Си" Констатина Владимирова (МФТИ, 2023-2024).

Здесь собраны мои варианты решений, система тестирования на основе официальных тестов курса, вспомогательные скрипты и шаблоны.

## Авторство

Ссылки на автора и оригинальный курс:

- Видеолекции: https://www.youtube.com/@tilir
- Репозиторий: https://github.com/tilir

Ознакомиться с условиями задач можно на платформах Stepik и Яндекс.Контест:

- Stepik: https://stepik.org/course/209865/syllabus
- Яндекс.Контест: https://contest.yandex.ru/contest/66459/enter/

## Структура репозитория

```text
.
├── greedy/                  # тема: жадные алгоритмы
│   ├── statements/              # заготовки кода для решения и тестирования
│   ├── 01_powers_of_four.c      # решение задачи (имя = имя каталога тестов)
│   ├── 02_egyptian_fracs.c
│   └── ...
├── hwasm/                   # тема: ассемблер
│   ├── statements/
│   ├── problem_as.c
│   ├── problem_mk.c
│   └── ...
├── ...
├── tests/                   # тесты курса
│   └── <topic>/<task>/
│       ├── 001.dat
│       ├── 001.ans
│       ├── ...
│       └── run.conf             # (опционально) схема запуска задачи
├── materials/               # шаблоны для локального использования
│   ├── scripts/
│   │   ├── test.sh
│   │   └── init.sh
│   ├── linters/
│   │   └── .clang-format
│   └── docs/
│       └── description-template.md
├── Makefile
├── build/                   # артефакты сборки (в .gitignore)
├── .clang-format            # локальная копия (в .gitignore)
├── test.sh                  # локальная копия (в .gitignore)
└── README.md
```

## Быстрый старт

### 1. Клонировать репозиторий

```bash
git clone https://github.com/IKYTARO/c_coursework.git c_coursework
cd c_coursework
```

### 2. Скопировать шаблоны из `materials/` в корень

```bash
cp materials/scripts/test.sh        .
cp materials/linters/.clang-format  .
chmod +x test.sh
```

(Эти файлы в `.gitignore`, так что локальные копии не попадут в git.
Если правите шаблон — правьте `materials/...` и копируйте заново.)

### 3. Решение и тестирование

В каждом каталоге темы каждой задаче соответствует свой `.c`-файл, в котором содержится решение (или еще нет);
Для запуска тестов используем следующую команду:

```bash
./test.sh <topic>/<problem_name>.c # одна задача по пути, например ./test.sh greedy/01_powers_of_four.c
```

Формат вывода:

```text
=== RUN : greedy/01_powers_of_four.c
--- PASS: Test 001
--- PASS: Test 002
--- FAIL: Test 003
        input   : 5 1 2 3 4 5
        expected: 3
        actual  : 2
FAIL
```

- `--- PASS: Test NNN` — тест пройден;
- `--- FAIL: Test NNN` — тест провален; ниже печатаются `input`, `expected` и `actual`;
- в конце — `PASS`, если все тесты всех запущенных задач прошли, иначе `FAIL`.

### 3.1 Нестандартные схемы запуска: `run.conf`

По умолчанию задача запускается как `./bin < NNN.dat`, ответ берётся из
`NNN.ans`. Если задача требует другого запуска (например, аргумент-файл),
положите рядом с тестами задачи файл `run.conf` со следующими опциями:

| Переменная           | По умолчанию        | Назначение                                                |
|----------------------|---------------------|-----------------------------------------------------------|
| `RUN_ARGS_TEMPLATE`  | `""`                | аргументы командной строки                                |
| `RUN_STDIN_TEMPLATE` | `'${name}.dat'`     | файл на stdin (пусто — без stdin)                         |
| `RUN_INPUT_GLOB`     | `'*.dat'`           | glob входных файлов (имя теста = basename без расширения) |
| `RUN_INPUT_EXT`      | из `RUN_INPUT_GLOB` | расширение входа, доступно как `${input_ext}`             |
| `RUN_ANS_TEMPLATE`   | `'${name}.ans'`     | шаблон файла с ответом                                    |

Например, для запуска задачи `hwasm/problem_mk` требуется формат: `./bin tests/hwasm/problem_mk/001.enc < tests/hwasm/problem_mk/001.dat`. Тогда для данной задачи формируем следующий файл:

```sh
# tests/hwasm/problem_mk/run.conf
RUN_ARGS_TEMPLATE='${task_dir}/${name}.enc'
RUN_STDIN_TEMPLATE='${name}.dat'
```
## Форматирование кода

Стиль задан в `.clang-format` (шаблон — `materials/.clang-format`).

```bash
make format           # переформатировать все .c и .h файлы (in-place)
make format-check     # проверить, не меняя файлы; ненулевой код при отклонениях
```