FBT	=	../fbt
APPPATH =	unleashed-firmware/applications_user/zero_jvm

all:
	mkdir -p $(APPPATH)
	cp -r src/* $(APPPATH)/
	cd unleashed-firmware && $(FBT) fap_zero_jvm

clean:
	rm -rf $(APPPATH)
