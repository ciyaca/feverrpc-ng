#include "feverrpc/feverrpc-factory.hpp"
#include "feverrpc/feverrpc.hpp"
#include "feverrpc/utils.hpp"
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

namespace FeverRPC {
using namespace std;
Factory::Factory() {
    server_address_len = sizeof(server_address);
    // initialize socket
    initialize_socket(_S2C_PORT, server_socket, server_address);
}

Caller Factory::accept() {
    // 监听 socket 连接，返回 caller

    int new_socket_handler;
    if ((new_socket_handler =
             ::accept(server_socket, (sockaddr *)&server_address,
                      (socklen_t *)&server_address_len)) < 0) {
        perror("accept");
    }
    return Caller(new_socket_handler);
}


} // namespace FeverRPC