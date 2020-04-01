# FeverRPC-ng

一个基于 Socket 长连接双向 RPC 框架。(重构版)

> 至于为什么叫 FeverRPC，是因为第一次写的时候（大三上）发烧了。

## Example

这个RPC可以让你写出类似于以下的代码：

```cpp
// Client.cpp

#include "feverrpc.hpp"

int main(){
    FeverRPC::Client rpc("127.0.0.1");

    int ans = rpc.call<int>("add", 1, 2, 3);
}
```

```cpp
// Server

#include "feverrpc.hpp"

int add(int a, int b, int c){
    return a + b + c;
}

int main(){
    FeverRPC::Server rpc;

    rpc.bind("add", add);
    rpc.c2s();
}
```

## Features

### Support

- 使用TCP/Socket长连接
- 双向RPC
- 支持任意长度、类型参数绑定
- 基于Msgpack，可自定义序列化类型
- Socket 支持任意大小传输功能 (int)

### Not Support
- void返回值
- 绑定非静态成员函数

## Dependencies

### Requirements

你需要解决以下依赖

- msgpack
- C++17 `std::apply`
- Linux 未在其他平台上进行过测试

### Linux API dependencies



## Devlopment
*如果你是一名开发者，这里的内容需要你认真阅读。*

### why should I use it
你能获得的优势：

在开发阶段，不在需要协商底层通信内容，只需要协商回调函数的接口（可以完全忽略网络传输的问题）
如果有新增的数据类型，不需要对底层的通信接口进行改动，只需要双方协商好字段和类型就可以。

### the things you should implement by yourself
因为最底层只提供回调函数的支持，那么即时通信的功能（收到一个消息，转发给任意用户）不在 feverrpc 负责的内容之中。想要达成这个目的，需要上层在注册的函数中，将获取到的信息转发给业务逻辑，然后再从业务逻辑调用其他 Socket 的 rpc。

### Related Efforts

- 感谢 [button-rpc](https://github.com/button-chen/buttonrpc_cpp14) 给予了我最关键的知识点
- Blogs you can find on Google Search
