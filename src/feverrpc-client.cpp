#include <feverrpc/feverrpc-client.hpp>
#include <feverrpc/utils.hpp>
namespace FeverRPC {
// FeverRPC::Client
Client::Client(const char *host) {
    // client
    _rpc_role = rpc_role::RPC_CLINT;
    _rpc_ret_code = rpc_ret_code::RPC_RET_SUCCESS;
    _HOST = host;
    _c2s_socket_handler = -1;
    _s2c_socket_handler = -1;
}
Client::~Client() {
    dbgprintf("_c2s=%d,_s2c=%d", _c2s_socket_handler, _s2c_socket_handler);
    if (_c2s_socket_handler > 0) {
        dbgprintf("[%lld]close _c2s", std::this_thread::get_id());
        close(_c2s_socket_handler);
    }

    if (_s2c_socket_handler > 0) {
        dbgprintf("[%lld]close _s2c", std::this_thread::get_id());
        close(_s2c_socket_handler);
    }
}
void Client::s2c() {
    dbgprintf("[%lld]in [s2c], waiting...", std::this_thread::get_id());
    // try to build a long link socket to server
    connect_socket(_HOST, _S2C_PORT, _s2c_socket_handler);
    dbgprintf("dd%d", _s2c_socket_handler);
    while (1) {
        // equal to recv_call_and_send

        listen(_s2c_socket_handler);
    }
    dbgprintf("enddd dd%d", _s2c_socket_handler);

    dbgprintf("[%lld]leave [s2c], exits", std::this_thread::get_id());
}

void Client::listen(const int &_s2c_socket_handler) {
    recv_call_and_send(_s2c_socket_handler);
}
} // namespace FeverRPC
