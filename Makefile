FILEPATH:=badapple.txt

all:
	$(CC) -O2 -o badapple *.c -D'PATH="$(FILEPATH)"'

clean:
	rm -rf badapple

install: clean
	$(MAKE) FILEPATH="/usr/local/share/badapple/badapple.txt"
	mkdir -p /usr/local/share/badapple
	install -t "/usr/local/share/badapple" "badapple.txt"
	install -t "/usr/local/bin" "badapple"

uninstall:
	rm -rf "/usr/local/share/badapple"
	rm -f "/usr/local/bin/badapple"

.PHONY: clean install uninstall

