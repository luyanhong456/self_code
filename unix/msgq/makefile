include $(ADSZ_DEV_COMMON_PATH)/makefile.comm

MY_LIBS=-ladutil -lumpclient -lexp_config \
    -ltcplib  -ladutil -lz\


MY_INCS=-I$(ADSZ_DEV_COMMON_PATH)/include/ \

VPATH=
OUTPUT=msgreceive
all:$(OUTPUT)

msgreceive:msgreceive.o

msgsend:msgsend.o


clean:
	rm -f *.o *.a $(OUTPUT)