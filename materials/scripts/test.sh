#!/usr/bin/env bash
set -u

# Использование:
#   ./test.sh                          — прогнать все задачи
#   ./test.sh greedy/01_powers_of_four — только одну
#   ./test.sh 01_powers_of_four        — найти по имени во всех темах
#
# Схема запуска задачи определяется файлом run.conf в каталоге тестов:
#   tests/<topic>/<task>/run.conf
# Поддерживаемые переменные (все опциональны):
#   RUN_ARGS_TEMPLATE   — аргументы командной строки (шаблон)
#   RUN_STDIN_TEMPLATE  — файл, подаваемый на stdin (шаблон); пусто => без stdin
#   RUN_INPUT_GLOB      — glob для входных файлов (по умолчанию '*.dat');
#                         имя теста берётся как basename без расширения
#   RUN_INPUT_EXT       — расширение входного файла для подстановки ${name}
#                         (по умолчанию: расширение из RUN_INPUT_GLOB)
#   RUN_ANS_TEMPLATE    — шаблон файла с ответом (по умолчанию '${name}.ans')
#
# В шаблонах доступны: ${name}, ${task_dir}, ${input_ext}, ${ans_ext}.
#
# Пример для hwasm/problem_mk:
#   RUN_ARGS_TEMPLATE='${task_dir}/${name}.enc'
#   RUN_STDIN_TEMPLATE='${name}.dat'

TESTS_DIR="${TESTS_DIR:-tests}"
BIN_DIR="build"
CC="${CC:-gcc}"
CFLAGS="${CFLAGS:--std=gnu11 -Wall -Wextra -Werror -O2 -g}"
TIMEOUT="${TIMEOUT:-2}"

# ---------------------------------------------------------------------------
# Форматирование вывода
# ---------------------------------------------------------------------------

# Отступ для блока details (как в go test — 8 пробелов)
INDENT="        "

# Печать многострочного значения с отступом и префиксом.
# $1 — префикс ("input", "expected", "actual")
# $2 — путь к файлу (или "-" для stdin-буфера)
print_block() {
    local label="$1" file="$2"
    local first=1
    # tr -d '\r' — на случай CRLF из Windows
    while IFS= read -r line || [ -n "$line" ]; do
        if [ $first -eq 1 ]; then
            printf '%s%-8s: %s\n' "$INDENT" "$label" "$line"
            first=0
        else
            printf '%s%-8s  %s\n' "$INDENT" "" "$line"
        fi
    done < <(tr -d '\r' < "$file")
    # если файл пустой — всё равно напечатать строку с префиксом
    if [ $first -eq 1 ]; then
        printf '%s%-8s: (empty)\n' "$INDENT" "$label"
    fi
}

# То же, но для строки из stdin-буфера (actual)
print_block_str() {
    local label="$1" str="$2"
    local first=1
    while IFS= read -r line || [ -n "$line" ]; do
        if [ $first -eq 1 ]; then
            printf '%s%-8s: %s\n' "$INDENT" "$label" "$line"
            first=0
        else
            printf '%s%-8s  %s\n' "$INDENT" "" "$line"
        fi
    done <<< "$str"
    if [ $first -eq 1 ]; then
        printf '%s%-8s: (empty)\n' "$INDENT" "$label"
    fi
}

# ---------------------------------------------------------------------------
# run.conf: загрузка и подстановка шаблонов
# ---------------------------------------------------------------------------

# Значения по умолчанию (стандартная схема: ./bin < input.dat > actual)
RUN_ARGS_TEMPLATE=""
RUN_STDIN_TEMPLATE='${name}.dat'
RUN_INPUT_GLOB="*.dat"
RUN_INPUT_EXT=""
RUN_ANS_TEMPLATE='${name}.ans'

load_run_conf() {
    local task_dir="$1"
    RUN_ARGS_TEMPLATE=""
    RUN_STDIN_TEMPLATE='${name}.dat'
    RUN_INPUT_GLOB="*.dat"
    RUN_INPUT_EXT=""
    RUN_ANS_TEMPLATE='${name}.ans'

    local conf="$task_dir/run.conf"
    [ -f "$conf" ] || return 0

    # shellcheck disable=SC1090
    . "$conf"

    # Если RUN_INPUT_EXT не задан явно, выводим из glob:
    # "*.enc" -> "enc"; "*.in" -> "in"; сложные glob — оставляем как есть.
    if [ -z "$RUN_INPUT_EXT" ]; then
        case "$RUN_INPUT_GLOB" in
            \*.*) RUN_INPUT_EXT="${RUN_INPUT_GLOB#\*.}" ;;
            *)    RUN_INPUT_EXT="" ;;
        esac
    fi
}

# Подстановка ${name}/${task_dir}/${input_ext}/${ans_ext} в шаблоне.
# Используем sed вместо eval — предсказуемо и безопасно.
expand_template() {
    local tpl="$1" name="$2" task_dir="$3" input_ext="$4" ans_ext="$5"
    # Экранируем спецсимволы sed в подставляемых значениях
    local name_e task_e in_e ans_e
    name_e=$(printf '%s' "$name"     | sed 's/[\/&]/\\&/g')
    task_e=$(printf '%s' "$task_dir" | sed 's/[\/&]/\\&/g')
    in_e=$(printf '%s' "$input_ext"  | sed 's/[\/&]/\\&/g')
    ans_e=$(printf '%s' "$ans_ext"   | sed 's/[\/&]/\\&/g')
    printf '%s' "$tpl" | sed \
        -e "s/\${name}/$name_e/g" \
        -e "s/\${task_dir}/$task_e/g" \
        -e "s/\${input_ext}/$in_e/g" \
        -e "s/\${ans_ext}/$ans_e/g"
}

# ---------------------------------------------------------------------------
# Запуск одной задачи
# ---------------------------------------------------------------------------

run_one() {
    local src="$1"
    local task_dir="$2"
    local bin="$BIN_DIR/$(basename "${src%.c}")"

    mkdir -p "$BIN_DIR"
    if ! $CC $CFLAGS "$src" -o "$bin" -lm 2>/tmp/cc_err; then
        echo "❌ COMPILE FAIL: $src"
        cat /tmp/cc_err
        return 1
    fi

    load_run_conf "$task_dir"

    # Расширение для ${name} в шаблоне ответа (для RUN_ANS_TEMPLATE)
    local ans_ext="${RUN_ANS_TEMPLATE##*.}"
    case "$ans_ext" in
        *'${'*) ans_ext="" ;;    # шаблон без явного расширения — не угадываем
    esac

    local total=0 passed=0
    shopt -s nullglob
    local inputs=( "$task_dir"/$RUN_INPUT_GLOB )
    shopt -u nullglob

    if [ ${#inputs[@]} -eq 0 ]; then
        echo "⚠️  нет входных файлов по шаблону '$RUN_INPUT_GLOB' в $task_dir"
        return 1
    fi

    for dat in "${inputs[@]}"; do
        [ -f "$dat" ] || continue

        local name
        name="$(basename "$dat")"
        name="${name%.*}"                        # "001" — без расширения

        # Путь к файлу ответа через шаблон
        local ans_rel
        ans_rel=$(expand_template "$RUN_ANS_TEMPLATE" "$name" "$task_dir" "$RUN_INPUT_EXT" "$ans_ext")
        local ans="$task_dir/$ans_rel"
        # Если шаблон уже содержит ${task_dir} — не дублируем
        case "$RUN_ANS_TEMPLATE" in
            *'${task_dir}'*) ans="$ans_rel" ;;
        esac

        if [ ! -f "$ans" ]; then
            echo "--- FAIL: Test $name (нет $ans_rel)"
            continue
        fi

        total=$((total+1))

        # Собираем команду запуска
        local args_str stdin_rel stdin_file
        args_str=$(expand_template "$RUN_ARGS_TEMPLATE"  "$name" "$task_dir" "$RUN_INPUT_EXT" "$ans_ext")
        stdin_rel=$(expand_template "$RUN_STDIN_TEMPLATE" "$name" "$task_dir" "$RUN_INPUT_EXT" "$ans_ext")

        if [ -n "$stdin_rel" ]; then
            stdin_file="$task_dir/$stdin_rel"
            case "$RUN_STDIN_TEMPLATE" in
                *'${task_dir}'*) stdin_file="$stdin_rel" ;;
            esac
            if [ ! -f "$stdin_file" ]; then
                echo "--- FAIL: Test $name (нет $stdin_rel)"
                continue
            fi
        else
            stdin_file=""
        fi

        # actual + код возврата + таймаут
        local actual rc
        # shellcheck disable=SC2086
        if [ -n "$stdin_file" ]; then
            actual=$(timeout "$TIMEOUT" "$bin" $args_str < "$stdin_file" 2>/tmp/run_err)
        else
            actual=$(timeout "$TIMEOUT" "$bin" $args_str 2>/tmp/run_err)
        fi
        rc=$?

        if [ $rc -eq 124 ]; then
            echo "--- FAIL: Test $name (timeout ${TIMEOUT}s)"
            continue
        fi
        if [ $rc -ne 0 ]; then
            echo "--- FAIL: Test $name (exit code $rc)"
            print_block "input"    "$stdin_file"   # для argv-режима тут будет dat
            print_block "expected" "$ans"
            print_block_str "actual" "$actual"
            continue
        fi

        # Сравнение с нормализацией: убираем CR, игнорируем хвостовые пробелы
        local norm_actual norm_expected
        norm_actual=$(printf '%s\n' "$actual" | tr -d '\r' | sed 's/[[:space:]]*$//')
        norm_expected=$(tr -d '\r' < "$ans" | sed 's/[[:space:]]*$//')

        if [ "$norm_actual" = "$norm_expected" ]; then
            echo "--- PASS: Test $name"
            passed=$((passed+1))
        else
            echo "--- FAIL: Test $name"
            print_block    "input"    "$stdin_file"
            print_block    "expected" "$ans"
            print_block_str "actual"  "$actual"
        fi
    done

    [ "$passed" -eq "$total" ]
}

# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------

fail=0
if [ $# -eq 0 ]; then
    while IFS= read -r src; do
        topic=$(basename "$(dirname "$src")")
        base=$(basename "${src%.c}")
        task_dir="$TESTS_DIR/$topic/$base"
        if [ -d "$task_dir" ]; then
            echo "=== RUN : $src"
            run_one "$src" "$task_dir" || fail=1
        else
            echo "⚠️  нет тестов для $src"
        fi
    done < <(find . -path ./build -prune -o -name '*.c' -print | grep -v "^./tests")
else
    for arg in "$@"; do
        if [ -f "$arg" ]; then
            topic=$(basename "$(dirname "$arg")")
            base=$(basename "${arg%.c}")
            echo "=== RUN : $arg"
            run_one "$arg" "$TESTS_DIR/$topic/$base" || fail=1
        else
            found=0
            while IFS= read -r src; do
                topic=$(basename "$(dirname "$src")")
                echo "=== RUN : $src"
                run_one "$src" "$TESTS_DIR/$topic/$arg" || fail=1
                found=1
            done < <(find . -name "${arg}.c" -not -path './build/*')
            [ $found -eq 0 ] && echo "не найдено: $arg"
        fi
    done
fi

if [ $fail -eq 0 ]; then
    echo "PASS"
else
    echo "FAIL"
fi
exit $fail