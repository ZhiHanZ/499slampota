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

finally, run:

$ cd third_party/protobuf
$ make && sudo make install

You should be within the vagrant folder still, and this is where you should clone or download this repository.

Run sudo apt-get install libgflags-dev

To compile the proto files, run the following:

$ protoc -I ./protos --cpp_out=. /protos/chirpkv.proto
$ protoc -I ./protos --cpp_out=. /protos/chirpsl.proto
$ protoc -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/chirpkv.proto 
$ protoc -I ./protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/chirpsl.proto

To compile the other files, run the following:

$ g++ -Wall -std=c++11 -g -c -o kv_backend.o kv_backend.cc
$ g++ -Wall -std=c++11 -g -c -o kv_server_grpc.o kv_server_grpc.cc
$ g++ -Wall -std=c++11 -g -c -o kv_client_grpc.o kv_client_grpc.cc
$ g++ -Wall -std=c++11 -g -c -o sl_backend.o sl_backend.cc
$ g++ -Wall -std=c++11 -g -c -o sl_server_grpc.o sl_server_grpc.cc
$ g++ -Wall -std=c++11 -g -c -o sl_client_grpc.o sl_client_grpc.cc
$ g++ -Wall -std=c++11 -g -c -o client_line.o client_line.cc

To link the files, run in one terminal:

$ g++ chirpkv.pb.o chirpkv.grpc.pb.o chirpsl.pb.o chirpsl.grpc.pb.o kv_client_grpc.o sl_backend.o kv_backend.o sl_client_grpc.o kv_server_grpc.cc -g -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -lgflags -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o kv

$ ./kv

(make sure you've navigated to the right folder after vagrant sshing in)

In another terminal:

$ g++ chirpkv.pb.o chirpkv.grpc.pb.o chirpsl.pb.o chirpsl.grpc.pb.o kv_client_grpc.o sl_backend.o kv_backend.o sl_client_grpc.o sl_server_grpc.cc -g -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -lgflags -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o sl

$ ./sl

In another terminal:

$ g++ chirpkv.pb.o chirpkv.grpc.pb.o chirpsl.pb.o chirpsl.grpc.pb.o kv_client_grpc.o sl_backend.o kv_backend.o sl_client_grpc.o client_line.cc -g -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` -lgflags -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o client

Now when you run ./client use the flags appropriately to register, login, chirp, reply, or monitor chirps.

For example:

./client --register Steph
./client --user Steph --chirp "My first chirp!"
./client --register Kelly
./client --user Steph --follow Kelly
./client --user Kelly --chirp "That's cool" --reply "chirp_by: Steph 0"
./client --user Kelly --read "chirp_by: Steph 0" 
./client --user Stephanie --monitor