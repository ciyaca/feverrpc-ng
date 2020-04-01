// Client side C/C++ program to demonstrate Socket programming
#include <feverrpc/feverrpc-client.hpp>
using namespace std;


int main(int argc, char const *argv[]) {
    FeverRPC::Client rpc("127.0.0.1");

    string ans = rpc.call<string>("repeat", "Yes! ", 5);
    cout << "[answer:]" << ans << endl; // Yes! Yes! Yes! Yes! Yes!
    ans = rpc.call<string>("repeat", "I love you!", 3);
    cout << "[answer:]" << ans << endl;
    int ret = rpc.call<int>("test", 1, 2,3,4);
    cout << "test: "<< ret<<endl;

    return 0;
}
