
CXX = clang++
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
all: chirp_service_layer.grpc.pb.cc chirp_key_value.grpc.pb.cc chirp_key_value.pb.cc chirp_service_layer.pb.cc key_value service_layer client
key_value: chirp_key_value.pb.o chirp_key_value.grpc.pb.o chirp_service_layer.pb.o chirp_service_layer.grpc.pb.o key_value_client_grpc.o utils/regex.o service_layer_backend.o key_value_backend.o service_layer_client_grpc.o key_value_server_grpc.o key_value_main_runnable.cc
	$(CXX) $^ $(LDFLAGS) -g -o $@
service_layer: chirp_key_value.pb.o chirp_key_value.grpc.pb.o chirp_service_layer.pb.o chirp_service_layer.grpc.pb.o key_value_client_grpc.o utils/regex.o service_layer_backend.o key_value_backend.o service_layer_client_grpc.o service_layer_server_grpc.o service_layer_main_runnable.cc
	$(CXX) $^ $(LDFLAGS) -g -o $@
client: chirp_key_value.pb.o chirp_key_value.grpc.pb.o chirp_service_layer.pb.o chirp_service_layer.grpc.pb.o key_value_client_grpc.o service_layer_backend.o utils/regex.o key_value_backend.o service_layer_client_grpc.o client_line.cc
	$(CXX) $^ $(LDFLAGS) -g -o $@
unit_testing: unit_testing.o chirp_service_layer.pb.o service_layer_backend_testing.o key_value_backend.o utils/regex.o
	$(CXX) $^ $(LDFLAGS) -g -o $@
.PRECIOUS: %.grpc.pb.cc
chirp_service_layer.grpc.pb.cc: protos/chirp_service_layer.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
chirp_key_value.grpc.pb.cc: protos/chirp_key_value.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
.PRECIOUS: %.pb.cc
chirp_key_value.pb.cc: protos/chirp_key_value.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<
chirp_service_layer.pb.cc: protos/chirp_service_layer.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<
clean:
	rm *.o
