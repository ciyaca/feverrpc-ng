// Client side C/C++ program to demonstrate Socket programming
#include "feverrpc/feverrpc.hpp"
#include <chrono>
#include <cstdio>
#include <feverrpc/feverrpc-client.hpp>
#include <feverrpc/utils.hpp>
#include <string>
#include <thread>
using namespace std;

string token = "";

string get_token() { return token; }

// 打印出接收到的广播消息
int print(string message) {
    puts(message.c_str());
    return 1;
}

int main(int argc, char const *argv[]) {

    // input your id
    cin >> token;

    FeverRPC::Client rpc("127.0.0.1");

    // server to client
    thread _thread{[]() {
        FeverRPC::Client _rpc("127.0.0.1");
        _rpc.bind("get_token", get_token);
        _rpc.bind("print", print);
        _rpc.s2c();
    }};
    thread_guard g(_thread);

    while (true) {
        string word;
        cin >> word;
        rpc.call<int>("broadcast", token, word);
    }
    return 0;
}
