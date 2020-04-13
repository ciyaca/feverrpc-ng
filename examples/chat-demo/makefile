INCLUDE=-I../../includes
MSGPACK=-Imsgpack/include

bin/%.o:%.cpp
	g++ -c $< ${MSGPACK} ${INCLUDE} --std=c++17 -o $@

bin/%.o:../../src/%.cpp ../../includes/feverrpc/%.hpp
	g++ -c $< ${MSGPACK} ${INCLUDE} --std=c++17 -o $@


all: bin server client

.PHONY: clean

clean:
	-rm -r bin

server: bin/server.o bin/feverrpc.o bin/feverrpc-factory.o bin/utils.o bin/feverrpc-server.o
	g++ $^ -o bin/$@ -pthread ${INCLUDE}

client: bin/client.o bin/feverrpc.o bin/feverrpc-client.o bin/utils.o bin/feverrpc-client.o
	g++ $^ -o bin/$@ -pthread ${INCLUDE}

bin:
	mkdir bin


