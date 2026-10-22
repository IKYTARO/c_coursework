CLANG_FORMAT ?= clang-format
SRCS         := $(shell find . -name '*.c' -not -path './build/*' -not -path './tests/*')

format:
	$(CLANG_FORMAT) -i $(SRCS)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(SRCS)

clean:
	rm -rf build

.PHONY: format format-check