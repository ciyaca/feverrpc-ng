// Server side C/C++ program to demonstrate Socket programming
// #include <feverrpc/feverrpc-server.hpp>
#include "feverrpc/feverrpc-server.hpp"
#include "feverrpc/utils.hpp"
#include <feverrpc/feverrpc-factory.hpp>
#include <string>
#include <thread>
#include <vector>
using namespace std;

vector<std::pair<string, FeverRPC::Caller>> vec;

int broadcast(string token, string message) {
    for (auto i = vec.begin(); i != vec.end(); i++) {
        if (i->first != token) {
            // is other user
            dbgprintf("token %s", token.c_str());
            i->second.call<int>("print", token + " say: " +  message);
        }
    }
    return 1;
}

string repeat(string text, int times) {
    string ret;
    while (times--) {
        ret += text;
    }
    return ret;
}

int main(int argc, char const *argv[]) {

    thread trd{[]() {
        FeverRPC::Factory f;
        while (true) {
            FeverRPC::Caller ce = f.accept();
            string token = ce.call<string>("get_token");
            dbgprintf("get token: %s", token.c_str());
            vec.push_back(std::pair{token, ce});
        }
    }};
    thread_guard gg(trd);

    while (true) {
        thread _thread{[]() {
            FeverRPC::Server rpc;
            rpc.bind("repeat", repeat);
            rpc.bind("broadcast", broadcast);
            rpc.c2s();
        }};
        thread_guard g(_thread);
    };

    return 0;
}