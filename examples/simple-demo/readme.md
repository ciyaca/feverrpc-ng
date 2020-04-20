# Simple-demo

简单的单向 RPC

## Usage

```bash
make

./bin/server

./bin/client

# debug make, use `make clean` before regenerate
CFLAG="-DDEBUG" make

./bin/server

./bin/client
```