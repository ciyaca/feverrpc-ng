// Client side C/C++ program to demonstrate Socket programming
#include "feverrpc/feverrpc.hpp"
#include <feverrpc/feverrpc-client.hpp>
#include <feverrpc/utils.hpp>
#include <string>
#include <thread>
#include <chrono>
#include <cstdio>
using namespace std;

string token = "";

string get_token() { return token; }

int print(string message) {
    puts(message.c_str());
    return 1;
}

int main(int argc, char const *argv[]) {

    cin>> token;

    FeverRPC::Client rpc("127.0.0.1");

    thread _thread{[]() {
        FeverRPC::Client _rpc("127.0.0.1");
        _rpc.bind("get_token", get_token);
        _rpc.bind("print", print);
        _rpc.s2c();
    }};
    thread_guard g(_thread);

    while (true) {
        string word;
        cin>> word;
        // string ans = rpc.call<string>("repeat", word, 5);
        // cout << "[answer:]" << ans << endl; // Yes! Yes! Yes! Yes! Yes!
        rpc.call<int>("broadcast", token, word);
    }
    return 0;
}
