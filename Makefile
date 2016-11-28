tar = hyphen
src = $(wildcard src/*.c)
obj = $(src:.c=.o)

CFLAGS = -std=c99 -pedantic-errors -Wall -O2 -DDESTDIR=$(DESTDIR) -D_POSIX_C_SOURCE=200809L

$(tar): $(obj)
	$(CC) -o $@ $^

#自动依赖分析
-include $(obj:.o=.d)
%.d: %.c
	@$(CC) $(CFLAGS) -MM $< >$@

.PHONY: clean run test install uninstall

clean:
	@rm -f $(obj) $(obj:.o=.d) $(tar) test/out

run: $(tar)
	@./$(tar)

test: $(tar)
	@sh test/run.sh

PREFIX = /usr/local

install: $(tar)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(tar) $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/$(tar)
	cp -r share/ $(DESTDIR)$(PREFIX)/share/$(tar)

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/$(tar)
	rm -rf $(DESTDIR)$(PREFIX)/share/$(tar)
