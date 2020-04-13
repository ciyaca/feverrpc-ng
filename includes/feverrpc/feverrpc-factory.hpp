#pragma once
// may use
// https://github.com/clpsz/linux-ipcs/tree/master/XSI_msgq
#include "feverrpc/feverrpc.hpp"
#include <msgpack/v3/sbuffer_decl.hpp>
#include <netinet/in.h>

namespace FeverRPC {
    class Caller;

    class Factory {
        int server_socket;
        sockaddr_in server_address;
        int server_address_len;

        // 初始化 Socket
        public:
        Factory();

        // 调用 `accept` 监听连接，获得后立即返回句柄
        Caller accept();
    };

    class Caller : FeverRPC {
        private:
        int socket_hd;
        template <typename Ret>
        Ret socket_call(msgpack::sbuffer &);
        public:
        // 送入已经连接的 Socket
        Caller(int socket_hd): socket_hd(socket_hd) {};
        // call
        template<typename Ret, typename... Args>
        Ret call(std::string name, Args... args);
    };

        template<typename Ret, typename... Args>
        Ret Caller::call(std::string name, Args... args){
            msgpack::sbuffer bf;

            msgpack::packer<msgpack::sbuffer> pk(&bf);
            pk.pack(name);
            pk.pack(std::make_tuple(args...));
            return socket_call<Ret>(bf);
        }

        template<typename Ret>
        Ret Caller::socket_call(msgpack::sbuffer &bf){
            msgpack::sbuffer recv_bf;
            send_and_recv(socket_hd, bf.data(), bf.size(),recv_bf);

            return unpack_ret_val<Ret>(recv_bf);
        }
}