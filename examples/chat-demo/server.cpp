// Server side C/C++ program to demonstrate Socket programming
#include "feverrpc/feverrpc-server.hpp"
#include "feverrpc/utils.hpp"
#include <feverrpc/feverrpc-factory.hpp>
#include <string>
#include <thread>
#include <vector>
using namespace std;

// 使用 vector 作为全局变量来存储 <id, rpc 回调函数>，
// 实际使用中可以考虑 map，并且需要考虑到客户端的退出问题。
vector<std::pair<string, FeverRPC::Caller>> vec;

// 简单设计一个广播消息的代码。
// 改代码会注册到 RPC 中，并由客户端调用。
// `token` 是客户端传来的 id。
int broadcast(string token, string message) {
    for (auto i = vec.begin(); i != vec.end(); i++) {

        // 当不是自己的时候
        if (i->first != token) {
            dbgprintf("token %s", token.c_str());
            // 反向调用客户端代码
            i->second.call<int>("print", token + " say: " +  message);
        }
    }
    return 1;
}

int main(int argc, char const *argv[]) {

    // server to client
    thread trd{[]() {
        FeverRPC::Factory f;
        while (true) {
            FeverRPC::Caller ce = f.accept();
            // 获得连接后，需要先从客户端获取 token，然后送入全局 vec
            string token = ce.call<string>("get_token");
            dbgprintf("get token: %s", token.c_str());
            vec.push_back(std::pair{token, ce});
        }
    }};
    thread_guard gg(trd);

    // client to server
    while (true) {
        thread _thread{[]() {
            FeverRPC::Server rpc;
            rpc.bind("broadcast", broadcast);
            rpc.c2s();
        }};
        thread_guard g(_thread);
    };

    return 0;
}
