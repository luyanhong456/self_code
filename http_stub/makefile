include ${EXPRESS_BASE_PATH}/makefile.comm

MY_LIBS=-lfcgi -lcgics  -ljson -lnetwk  -ladutil -lcurl -lneo_cgi -lneo_cs -lneo_utl\
	-ltcplib -lcurl -ladutil -ljson -lprotobuf \
    -L${PLATFORM_BASE_PATH_V6}/export_lib_centos64 -lconfig_access

MY_INCS=-I${PLATFORM_BASE_PATH_V6}/export_include \
		-I$(EXPRESS_BASE_PATH)/biz/ocpa/show/include/ \
		-I/home/luyanhong/luyanhong/commontest/fapp\
		
PROTOS=

OUTPUT = test_http_stub 
 
	
all:$(OUTPUT)

test_http_stub:test_http_stub.o 


clean:
	rm -f *.o *.a $(OUTPUT) *pb.cc *pb.h