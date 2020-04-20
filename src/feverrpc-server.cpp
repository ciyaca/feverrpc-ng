#include <feverrpc/feverrpc-server.hpp>
#include <feverrpc/utils.hpp>
namespace FeverRPC {

Server::~Server() {
    dbgprintf("[%lld]destruct Server.\n", std::this_thread::get_id());
}

void Server::s2c() {
    // 一个监听线程，用于服务端向客户端推送消息
    // 首先，监听socket，当有连接时，新建线程
    // 服务端会立刻调用login函数，获得用户名和密码。
    // 然后向数据库进行认证，如果失败，则断开链接。
    // 如果成功，证明该用户登录。存储用户id，并向线程调度模块注册线程，证明该用户上线。
    dbgprintf("[%lld]in [s2c], waiting...\n", std::this_thread::get_id());
    // try to initialize a socket in another thread.
    int server_fd, new_socket_handler;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    // initialize socket
    initialize_socket(_S2C_PORT, server_fd, address);
    // block and wait for connect
    while (1) {
        dbgprintf("[%lld][s2c] waiting for new socket to connect.\n",
               std::this_thread::get_id());
        if ((new_socket_handler = accept(server_fd, (struct sockaddr *)&address,
                                         (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // 新建线程
    };
    dbgprintf("[%lld][Server::s2c]this thread ended!\n",
           std::this_thread::get_id());
}

void Server::c2s() {

    dbgprintf("[%lld]in [c2s], waiting...\n", std::this_thread::get_id());
    // run socket server
    // ! 1. initialize socket
    int server_fd, new_socket_handler;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    initialize_socket(_C2S_PORT, server_fd, address);

    // ! 2. block and waiting for data.
    while (1) {
        dbgprintf("[%lld][c2s] waiting for new socket to connect.\n",
               std::this_thread::get_id());
        if ((new_socket_handler = accept(server_fd, (struct sockaddr *)&address,
                                         (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::thread _thread{[new_socket_handler, this]() {
            int uid = -1;
            dbgprintf("[new thread][c2s] %lld\n", std::this_thread::get_id());

            // 这以后，被调用函数可以通过TM获取uid
            while (1) {
                try {
                    recv_call_and_send(new_socket_handler);
                    dbgprintf("[%lld]recv_call_and_send(new_socket_handler);\n",
                           std::this_thread::get_id());
                } catch (const std::exception &e) {
                    // 这里会在客户端退出时捕获异常，并进行逻辑处理
                    // 基本的思想是，利用threadManager
                    // 释放另一个线程的资源（socket handler 等）
                    // 然后释放自己的资源
                    puts(e.what());
                    break;
                }
            }
            dbgprintf("[%lld][c2s]this thread ended!\n",
                   std::this_thread::get_id());
        }};
        _thread.detach();
    }

    dbgprintf("[%lld][Server::c2s]this thread ended!\n",
           std::this_thread::get_id());
    return;
};
} // namespace FeverRPC