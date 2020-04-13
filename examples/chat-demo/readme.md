# Chat-demo

带有群发功能的双向通信。（在群发中排除了自己，说明已经有了身份鉴别能力）

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

具体的逻辑请参考[如何解决双向调用中的身份对应](../../readme.md#solve-the-paradox-of-biconnections-identification-problem)。
