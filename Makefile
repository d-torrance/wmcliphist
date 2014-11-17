CC = gcc

INCLUDES = `gtk-config --cflags` -I. -Ifoodock -g

DESTDIR = "/usr/local/bin"

# for normal use
CFLAGS = -Wall -O2 -ansi -pedantic $(INCLUDES)
DEBUG = 

# for debuggind purposes
# ISO doesn't support macros with variable number of arguments so -pedantic
# must not be used
#CFLAGS = -Wall -g -ansi $(INCLUDES) -DFNCALL_DEBUG
#DEBUG = debug.o


LFLAGS = `gtk-config --libs`


OBJECTS = wmcliphist.o clipboard.o gui.o rcconfig.o history.o hotkeys.o $(DEBUG)
TARGET = wmcliphist

all: $(TARGET)

lclint:
	lclint $(INCLUDES) +posixlib *.c >lclint.log

wmcliphist: $(OBJECTS) foodock/foodock.o
	$(CC) $(LFLAGS) -o $@ $(OBJECTS) foodock/foodock.o

wmcliphist.o: wmcliphist.c wmcliphist.h \
	icon/ico_60x60_black.xpm icon/ico_60x60_gray.xpm \
	icon/ico_60x60_white.xpm icon/ico_60x60_mask.xbm \
	icon/ico_40x40_black.xpm icon/ico_40x40_gray.xpm \
	icon/ico_40x40_white.xpm icon/ico_40x40_mask.xbm \
	icon/ico_30x30_black.xpm icon/ico_30x30_gray.xpm \
	icon/ico_30x30_white.xpm icon/ico_30x30_mask.xbm \
	icon/ico_16x16.xpm icon/ico_16x16_mask.xbm \
	foodock/foodock.h

clipboard.o: clipboard.c wmcliphist.h

rcconfig.o: rcconfig.c wmcliphist.h

gui.o: gui.c wmcliphist.h

history.o: history.c wmcliphist.h

hotkeys.o: hotkeys.c wmcliphist.h

clean:
	rm -rf $(OBJECTS) $(TARGET)
	@(cd foodock && make clean)

install:
	cp wmcliphist $(DESTDIR)
