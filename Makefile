CC = gcc
CFLAGS = -g

FBT	=	../fbt
APPPATH =	unleashed-firmware/applications_user/zero_jvm
CPPFLAGS = -I unleashed-firmware/furi -I unleashed-firmware/applications/services

# Target for a building on a x86 architecture for debug
x86:
	$(CC) $(CFLAGS) src/zero_jvm_default_entry.c -o zero_jvm_default


# Target for building on a flipper
flipper:
	mkdir -p $(APPPATH)
	cp -r src/* $(APPPATH)/
	rm $(APPPATH)/zero_jvm_default_entry.c
	cd unleashed-firmware && $(FBT) fap_zero_jvm

clean:
	rm -rf $(APPPATH)
