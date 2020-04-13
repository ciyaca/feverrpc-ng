# Simple-demo

简单的单向 RPC

## Usage

```bash
make

./bin/server

./bin/client
```

先开启一个服务端，然后开启多个客户端。

每个客户端第一次输入是自己的 token（可以理解为 id），
之后每一次输入都是一次群发信息。

## warning

这套逻辑不要直接使用在生产环境上。