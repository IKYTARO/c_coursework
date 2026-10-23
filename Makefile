CLANG_FORMAT ?= clang-format
SOURCES      := $(shell find . -name '*.c' -not -path './build/*' -not -path './tests/*')
HEADERS      := $(shell find . -name '*.h' -not -path './build/*' -not -path './tests/*')

format:
	$(CLANG_FORMAT) -i $(SOURCES) $(HEADERS)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(SOURCES) $(HEADERS)

clean:
	rm -rf build

.PHONY: format format-check