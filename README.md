# 499-slampota
Stephanie Lampotang
slampota@usc.edu

Installation:
Use vagrant file to install the correct version of vagrant. Once it is set up: vagrant up and vagrant ssh into the folder. 

Next, install grpc by running the following in the terminal once you are in the vagrant folder:

$ [sudo] apt-get install build-essential autoconf libtool pkg-config
$ [sudo] apt-get install libgflags-dev libgtest-dev
$ [sudo] apt-get install clang libc++-dev

$ cd grpc
$ git submodule update --init
$ make

$ cd grpc/third_party/protobuf
$ sudo make install

You should be within the vagrant folder still, and this is where you should clone or download this repository.

Run sudo apt-get install libgflags-dev

To compile the proto files, run the following:

protoc -I ./protos --cpp_out=. ./protos/chirp_key_value.proto
protoc -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/chirp_key_value.proto
protoc -I ./protos --cpp_out=. ./protos/chirp_service_layer.proto
protoc -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/chirp_service_layer.proto
g++ -Wall -std=c++11 -g -c -o chirp_key_value.pb.o chirp_key_value.pb.cc
g++ -Wall -std=c++11 -g -c -o chirp_key_value.grpc.pb.o chirp_key_value.grpc.pb.cc
g++ -Wall -std=c++11 -g -c -o chirp_service_layer.pb.o chirp_service_layer.pb.cc
g++ -Wall -std=c++11 -g -c -o chirp_service_layer.grpc.pb.o chirp_service_layer.grpc.pb.cc

To compile the other files, run the following:

g++ -Wall -std=c++11 -g -c -o key_value_backend.o key_value_backend.cc
g++ -Wall -std=c++11 -g -c -o key_value_server_grpc.o key_value_server_grpc.cc
g++ -Wall -std=c++11 -g -c -o key_value_client_grpc.o key_value_client_grpc.cc
g++ -Wall -std=c++11 -g -c -o service_layer_backend.o service_layer_backend.cc
g++ -Wall -std=c++11 -g -c -o service_layer_server_grpc.o service_layer_server_grpc.cc
g++ -Wall -std=c++11 -g -c -o service_layer_client_grpc.o service_layer_client_grpc.cc
g++ -Wall -std=c++11 -g -c -o client_line.o client_line.cc
g++ -Wall -std=c++11 -g -c -o key_value_main_runnable.o key_value_main_runnable.cc
g++ -Wall -std=c++11 -g -c -o service_layer_main_runnable.o service_layer_main_runnable.cc

To link the files, run in one terminal:

$ g++ chirp_key_value.pb.o chirp_key_value.grpc.pb.o chirp_service_layer.pb.o chirp_service_layer.grpc.pb.o key_value_client_grpc.o service_layer_backend.o key_value_backend.o service_layer_client_grpc.o key_value_server_grpc.o key_value_main_runnable.cc -g -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -lgflags -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o key_value

$ ./key_value

(make sure you've navigated to the right folder after vagrant sshing in)

In another terminal:

$ g++ chirp_key_value.pb.o chirp_key_value.grpc.pb.o chirp_service_layer.pb.o chirp_service_layer.grpc.pb.o key_value_client_grpc.o service_layer_backend.o key_value_backend.o service_layer_client_grpc.o service_layer_server_grpc.o service_layer_main_runnable.cc -g -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -lgflags -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o service_layer

$ ./service_layer

In another terminal:

$ g++ chirp_key_value.pb.o chirp_key_value.grpc.pb.o chirp_service_layer.pb.o chirp_service_layer.grpc.pb.o key_value_client_grpc.o service_layer_backend.o key_value_backend.o service_layer_client_grpc.o client_line.cc -g -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -lgflags -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o client

Now when you run ./client use the flags appropriately to register, login, chirp, reply, or monitor chirps.

For example:

./client --register Steph
./client --user Steph --chirp "My first chirp!"
./client --register Kelly
./client --user Steph --follow Kelly
./client --user Kelly --chirp "That's cool" --reply "chirp_by: Steph 0"
./client --user Kelly --read "chirp_by: Steph 0" 
./client --user Stephanie --monitor


Unit testing:

g++ -Wall -std=c++11 -g -c -o key_value_backend.o key_value_backend.cc
g++ -Wall -std=c++11 -g -c -o service_layer_backend_testing.o service_layer_backend_testing.cc
g++ -Wall -std=c++11 -g -c -o unit_testing.o unit_testing.cc
g++ -Wall -std=c++11 -g -L/usr/local/lib -o unit_test unit_testing.o chirp_service_layer.pb.o service_layer_backend_testing.o key_value_backend.o -lgtest -lpthread -lprotobuf
./unit_test