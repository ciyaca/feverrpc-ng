// Server side C/C++ program to demonstrate Socket programming
#include <feverrpc/feverrpc-server.hpp>
#include <string>
using namespace std;


string repeat(int id, string text, int times) {
    string ret;
    while (times--) {
        ret += text;
    }
    return ret;
}

int test(int a, int b, int c, int d){
    return a+ b + c-d ;
}

int main(int argc, char const *argv[]) {
    FeverRPC::Server rpc = FeverRPC::Server();
    rpc.bind("repeat", repeat);
    rpc.bind("test", test);
    rpc.c2s();
    return 0;
}