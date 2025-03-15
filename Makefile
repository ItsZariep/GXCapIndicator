WITHX11 ?= 0
PREFIX ?= /usr/local

ifeq ($(WITHX11), 0)
	XLIB = -ludev -linput -levdev -I/usr/include/libevdev-1.0/
else
	XLIB = -lX11
	FLAGS +=-DWITHX11
endif

CC = cc
CFLAGS = `pkg-config --cflags gtk+-3.0 ayatana-appindicator3-0.1` $(FLAGS) $(XLIB)
LIBS = `pkg-config --libs gtk+-3.0 ayatana-appindicator3-0.1` -lpthread $(FLAGS) $(XLIB)

SRC = gxcapindicator.c
OBJ = $(SRC:.c=.o)
EXE = gxcapindicator

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

debug: $(SRC)
	$(CC) $(CFLAGS) -g $^ -o debug $(LIBS)

test: $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJ) $(EXE) debug

install: $(EXE)
	install -Dm755 $(EXE) $(PREFIX)/bin/$(EXE)
	install -Dm755 gxcapindicator.desktop $(PREFIX)/share/applications/gxcapindicator.desktop
	install -Dm644 icons/keyboard-caps-disabled.svg $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-caps-disabled.svg
	install -Dm644 icons/keyboard-caps-enabled.svg $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-caps-enabled.svg
	install -Dm644 icons/keyboard-num-disabled.svg $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-num-disabled.svg
	install -Dm644 icons/keyboard-num-enabled.svg $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-num-enabled.svg

uninstall:
	rm -f $(PREFIX)/bin/$(EXE)
	rm -f $(PREFIX)/share/applications/gxcapindicator.desktop
	rm -f $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-caps-disabled.svg
	rm -f $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-caps-enabled.svg
	rm -f $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-num-disabled.svg
	rm -f $(PREFIX)/share/icons/hicolor/32x32/devices/keyboard-num-enabled.svg
