INCLUDE=-I../../includes
MSGPACK=-Imsgpack/include

bin/%.o:%.cpp
	g++ -c $< ${MSGPACK} ${INCLUDE} --std=c++17 -o $@ $(CFLAGS)

bin/%.o:../../src/%.cpp
	g++ -c $< ${MSGPACK} ${INCLUDE} --std=c++17 -o $@ $(CFLAGS)

all: bin server client

.PHONY: clean

clean:
	-rm -r bin

server: bin/server.o bin/feverrpc.o bin/feverrpc-server.o
	g++ $^ -o bin/$@ -pthread ${INCLUDE}

client: bin/client.o bin/feverrpc.o bin/feverrpc-client.o
	g++ $^ -o bin/$@ -pthread ${INCLUDE}

bin:
	mkdir bin


