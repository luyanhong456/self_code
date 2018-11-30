include ${EXPRESS_BASE_PATH}/makefile.comm

MY_LIBS=-L$(PLATFORM_BASE_PATH)/export_lib_centos64 -lcomponent_configcenter_set \
    -ltcplib -lcurl -ladutil -ljson  

MY_INCS=-I$(PLATFORM_BASE_PATH)/export_include\
	-I$(PLATFORM_BASE_PATH)/export_include/c2cplatform/component/configcenter/ \

PROTOS=

OUTPUT = test_http_client

	
all:$(OUTPUT)

test_http_client:test_http_client.o http_client.o
test_http_stub:test_http_stub.o

clean:
	rm -f *.o *.a $(OUTPUT) *pb.cc *pb.h
