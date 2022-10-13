FBT	=	../fbt
APPPATH =	unleashed-firmware/applications_user/zero_jvm
CPPFLAGS = -I unleashed-firmware/furi -I unleashed-firmware/applications/services

# this target is only for proper parsing of header files by CLion
# do not try to make this target, it would fail :)
placeholder:
	exit 1
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/main.c

# this is intended target
all:
	mkdir -p $(APPPATH)
	cp -r src/* $(APPPATH)/
	cd unleashed-firmware && $(FBT) fap_zero_jvm

clean:
	rm -rf $(APPPATH)
