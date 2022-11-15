CC = gcc
CFLAGS = -g -m32 -Wall -fsanitize=address


FBT	=	../fbt
APPPATH =	unleashed-firmware/applications_user/zero_jvm
CPPFLAGS = -I unleashed-firmware/furi -I unleashed-firmware/applications/services
SRC = src/zero_jvm_default_entry.c src/zero_jvm/loader.c src/zero_jvm/runtime.c src/zero_jvm/frame.c


# Target for a building on a x86 architecture for debug
all: x86

x86: $(SRC)
	$(CC) -o zero_jvm_default $^ $(CFLAGS) -I /src/zero_jvm


# Target for building on a flipper
flipper:
	mkdir -p $(APPPATH)
	cp -r src/* $(APPPATH)/
	rm $(APPPATH)/zero_jvm_default_entry.c
	cd unleashed-firmware && $(FBT) fap_zero_jvm

clean:
	rm -rf $(APPPATH) zero_jvm_default
