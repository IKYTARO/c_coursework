#!/usr/bin/env bash
#
# init.sh — инициализация структуры учебного репозитория.
#
# Исходное состояние (после копирования тестов курса):
#   <topic>/tests/<task>/001.dat, 001.ans, ...
#
# Целевое состояние:
#   <topic>/<task>.c
#   tests/<topic>/<task>/001.dat, 001.ans, ...
#
# Использование:
#   ./init.sh                      # обработать все темы в текущем каталоге
#   ./init.sh greedy hwalg         # обработать только указанные темы
#   ./init.sh --dry-run            # показать, что будет сделано
#
set -euo pipefail

DRY_RUN=0
TOPICS=()

# --- разбор аргументов ---
for arg in "$@"; do
    case "$arg" in
        -n|--dry-run) DRY_RUN=1 ;;
        -h|--help)
            sed -n '2,16p' "$0" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        -*)
            echo "неизвестный флаг: $arg" >&2
            exit 2
            ;;
        *) TOPICS+=("$arg") ;;
    esac
done

run() {
    if [ "$DRY_RUN" -eq 1 ]; then
        echo "  [dry-run] $*"
    else
        "$@"
    fi
}

# --- если темы не переданы, найдём все каталоги с подкаталогом tests/ ---
if [ ${#TOPICS[@]} -eq 0 ]; then
    while IFS= read -r d; do
        TOPICS+=("$(dirname "$d")")
    done < <(find . -mindepth 2 -maxdepth 2 -type d -name tests | sort)
fi

if [ ${#TOPICS[@]} -eq 0 ]; then
    echo "не найдено ни одной темы (каталогов с подкаталогом tests/)" >&2
    exit 1
fi

TOTAL_OK=0
TOTAL_SKIP=0

for topic in "${TOPICS[@]}"; do
    topic="${topic#./}"           # убрать ведущий ./
    topic="${topic%/}"            # убрать хвостовой /

    src_tests="$topic/tests"
    dst_tests="tests/$topic"

    echo "==> тема: $topic"

    if [ ! -d "$src_tests" ]; then
        echo "    ⚠️  нет каталога $src_tests — пропуск"
        continue
    fi

    # 1. Создать .c-файлы для каждой задачи
    shopt -s nullglob
    for task_dir in "$src_tests"/*/; do
        task="$(basename "$task_dir")"
        cfile="$topic/$task.c"
        if [ -e "$cfile" ]; then
            echo "    · уже существует: $cfile"
            TOTAL_SKIP=$((TOTAL_SKIP+1))
        else
            run touch "$cfile"
            echo "    + $cfile"
            TOTAL_OK=$((TOTAL_OK+1))
        fi
    done
    shopt -u nullglob

    # 2. Перенести тесты в tests/<topic>/
    if [ -e "$dst_tests" ]; then
        echo "    ⚠️  $dst_tests уже существует — перенос пропущен."
        echo "       Сначала разберитесь с этим каталогом вручную."
        continue
    fi

    run mkdir -p "tests"
    run mv "$src_tests" "$dst_tests"
    echo "    → $src_tests  →  $dst_tests"
done

echo
echo "готово: создано $TOTAL_OK файлов, пропущено (уже были) $TOTAL_SKIP"
if [ "$DRY_RUN" -eq 1 ]; then
    echo "(dry-run: изменения не применялись)"
fi