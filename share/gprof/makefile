include $(EXPRESS_BASE_PATH)/makefile.comm

MY_LIBS = -ladutil -ljson

MY_INCS = -I${PLATFORM_BASE_PATH}/common/util/include/ \

OUTPUT = test_json_gprof

all:$(OUTPUT)

test_json_gprof:test_json_gprof.o

clean:
	rm -f *.o *.a $(OUTPUT) *pb.cc *pb.h