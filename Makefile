CC:=$(shell command -v musl-gcc 2>/dev/null || command -v gcc 2>/dev/null || command -v cc 2>/dev/null)
BIN=slug

all: $(BIN)

$(BIN): %: %.c
	$(CC) -o $@ $< -static

clean:
	rm $(BIN)

install:
	cp $(BIN) /usr/bin/$(BIN)

strip:
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $(BIN)
