
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++17
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
					 -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
					 -ldl -lgtest -lpthread -lgflags -lglog
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PROTOS_PATH = protos/
vpath %.proto $(PROTOS_PATH)
all: regex_test chirp_service_layer.pb.cc chirp_service_layer.grpc.pb.cc
regex_test: regex_test.o regex.o
	$(CXX) $^ $(LDFLAGS) -g -o $@
.PRECIOUS: %.grpc.pb.cc
chirp_service_layer.grpc.pb.cc: protos/chirp_service_layer.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=protos/. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
.PRECIOUS: %.pb.cc
chirp_service_layer.pb.cc: protos/chirp_service_layer.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=protos/. $<
clean:
	rm *.o regex_test
