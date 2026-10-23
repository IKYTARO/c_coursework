#!/usr/bin/env bash
set -u

# Использование:
#   ./test.sh                          — прогнать все задачи
#   ./test.sh greedy/01_powers_of_four — только одну
#   ./test.sh 01_powers_of_four        — найти по имени во всех темах

TESTS_DIR="${TESTS_DIR:-tests}"
BIN_DIR="build"
CC="${CC:-gcc}"
CFLAGS="${CFLAGS:--std=c11 -Wall -Wextra -Werror -O2 -g}"
TIMEOUT="${TIMEOUT:-2}"

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

    local total=0 passed=0
    for dat in "$task_dir"/*.dat; do
        [ -f "$dat" ] || continue
        local ans="${dat%.dat}.ans"
        [ -f "$ans" ] || continue

        total=$((total+1))
        local name
        name="$(basename "$dat" .dat)"          # "001"

        # actual + код возврата + таймаут
        local actual rc
        actual=$(timeout "$TIMEOUT" "$bin" < "$dat" 2>/tmp/run_err)
        rc=$?

        if [ $rc -eq 124 ]; then
            echo "--- FAIL: Test $name (timeout ${TIMEOUT}s)"
            continue
        fi
        if [ $rc -ne 0 ]; then
            echo "--- FAIL: Test $name (exit code $rc)"
            print_block "input"    "$dat"
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
            print_block    "input"    "$dat"
            print_block    "expected" "$ans"
            print_block_str "actual"  "$actual"
        fi
    done

    [ "$passed" -eq "$total" ]
}

# --- main ---
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