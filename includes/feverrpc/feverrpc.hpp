/*!
 * \file feverrpc.cpp
 * \brief 一个基于Socket长连接双向RPC框架，嵌入了登录认证功能。
 *
 * https://github.com/Courier-Developer/feverrpc
 * 使用TCP/Socket长连接
 * 双向RPC
 * 支持任意长度、类型参数绑定
 * 基于Msgpack，可自定义序列化类型
 * Socket支持任意大小传输功能（int)
 * 支持多线程，有多线程调度模块
 * 服务端线程可相互通信
 * 嵌入登录功能
 *
 * \author 冯开宇
 * \version 0.1
 * \date 2019-9-1
 */
#ifndef FEVERRPC_FEVERRPC_H_
#define FEVERRPC_FEVERRPC_H_

#include "msgpack.hpp"
#include <chrono>
#include <cstdio>
#include <cstdlib>

#include <functional>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

// TODO MORE DOCS

/// \brief RPC框架的命名空间
///
/// 包含RPC本身，socket通讯相关函数以及自定义异常
namespace FeverRPC {

/// \brief Socket一次通信中最大数据发送大小
const int _CHUNK_SIZE = 1024;

/// \brief 客户端到服务器通信端口
const int _C2S_PORT = 8012;

/// \brief 服务端到客户端通信端口
const int _S2C_PORT = 8021;

/// \brief Socket通讯异常
class SocketException : public std::exception {
    virtual const char *what() const throw() {
        return "Connection Failed / Socket Error";
    }
};

/// \brief 函数调用异常，说明你没有注册该函数
class FunctionCallException : public std::exception {

    virtual const char *what() const throw() {
        return "function call error. no such function.";
    }
};

/// \brief 使用Socket发送数据
///
/// 支持unsigned int大小内的任意大小数据传输，不支持断点续传
/// \param socket_handler socket句柄
/// \param data_send_buffer the pointer that point to your data to be sent
/// \param data_send_size size of data
/// \return if there is error. 0 is ok.
int send_data(const int &socket_handler, const char *data_send_buffer,
              int data_send_size);

/// \brief 接收数据
/// \param socket_handler docket handler
/// \param data_recv_buffer msgpack::sbuffer used for recive data
/// \return if there is an error.
int recv_data(const int &socket_handler, msgpack::sbuffer &data_recv_buffer);

// 连接到一个Socket，用户客户端。
int connect_socket(const char *__host, const int __port,
                   int &new_socket_handler);

// 初始化Socket，监听端口。用于服务端。
int initialize_socket(const int __port, int &server_fd, sockaddr_in &address);

// 包装序列化数据
class Serializer {
  public:
    msgpack::sbuffer buffer;
};

// 封装RPC的基本方法
class FeverRPC {

  protected:
    // 强枚举类型
    // whether is Client or Server
    enum class rpc_role { RPC_CLINT, RPC_SERVER };
    // return code
    enum class rpc_ret_code : unsigned int {
        RPC_RET_SUCCESS = 0,
        RPC_RET_FUNCTION_NOT_BIND,
        RPC_RET_RECV_TIMEOUT,
        RPC_RET_UNRETURNED,
    };

    // role, !deprecated!
    rpc_role _rpc_role;
    // return code, !deprecated!
    rpc_ret_code _rpc_ret_code;
    // host
    const char *_HOST;
    // mapping form function name to function
    // can be state as static parameter
    std::map<std::string, std::function<void(Serializer *, msgpack::object)>>
        funcs_map;

  public:
    // bind name to function
    template <typename Func> void bind(std::string name, Func func);

  protected:
    // callproxy 由一组函数构成，实际上内部实在解开 msgpack 的包，
    // 转换为可以传入函数的实际参数。
    // 最终 callproxy 调用 call_helper 进行真实的函数调用，
    // 并再次通过 callproxy 将返回值传回。
    // callproxy 是被注册进 map 中的，相当于一个闭包。
    template <typename Func>
    void callproxy(Func f, Serializer *pr, msgpack::object args_obj);
    template <typename Ret, typename... Args>
    void callproxy_(Ret (*func)(Args...), Serializer *pr,
                    msgpack::object args_obj);
    template <typename Ret, typename... Args>
    void callproxy_(std::function<Ret(Args... ps)> func, Serializer *pr,
                    msgpack::object args_obj);
    template <typename Ret, typename Func, typename... Args>
    // 进行实际的函数调用。
    Ret call_helper(Func f, std::tuple<Args...> args);

    // 通过 function name 选择相应的 callproxy，
    // 并进行调用。
    Serializer call_(std::string name, msgpack::object args_obj);

    Serializer call_chooser(std::string &name, msgpack::object &args_obj);
    // 发起 rpc 调用，并接收返回值
    int send_and_recv(const int &socket_handler, const char *data_send_buffer,
                      int data_send_size, msgpack::sbuffer &data_recv_buffer);

    template <typename Ret> Ret unpack_ret_val(msgpack::sbuffer &buffer);

    // 监听请求，执行后发送返回值
    Serializer recv_call_and_send(const int &socket_handler);
    // 帮助输出
    void print_sbuffer(msgpack::sbuffer &buffer);
};

/////////////////////////////////////////////////////////////
//
/// SOME DEFINITIONS CANNOT TO BE DEFINED IN ANNOTHER FILE
//
/////////////////////////////////////////////////////////////
template <typename Func> void FeverRPC::bind(std::string name, Func func) {
    // 注册闭包
    funcs_map[name] = std::bind(&FeverRPC::callproxy<Func>, this, func,
                                std::placeholders::_1, std::placeholders::_2);
}

template <typename Func>
inline void FeverRPC::callproxy(Func f, Serializer *pr,
                                msgpack::object args_obj) {
    callproxy_(f, pr, args_obj);
};
template <typename Ret, typename... Args>
inline void FeverRPC::callproxy_(Ret (*func)(Args...), Serializer *pr,
                                 msgpack::object args_obj) {
    // 解开返回值
    callproxy_(std::function<Ret(Args...)>(func), pr, args_obj);
}
template <typename Ret, typename... Args>
void FeverRPC::callproxy_(std::function<Ret(Args... ps)> func, Serializer *pr,
                          msgpack::object args_obj) {
    std::tuple<Args...> args;
    args_obj.convert(args);

    // 解开参数
    Ret ret = call_helper<Ret>(func, args);

    msgpack::pack(pr->buffer, ret);
    return;
}
template <typename Ret, typename Func, typename... Args>
Ret FeverRPC::call_helper(Func f, std::tuple<Args...> args) {
    // 利用 c++17 新特性进行函数调用
    // 否则要手动展开
    return std::apply(f, args);
}

template <typename Ret> Ret FeverRPC::unpack_ret_val(msgpack::sbuffer &buffer) {

    msgpack::unpacker pac;
    // feeds the buffer.
    pac.reserve_buffer(buffer.size());
    memcpy(pac.buffer(), buffer.data(), buffer.size());
    pac.buffer_consumed(buffer.size());

    // now starts streaming deserialization.
    msgpack::object_handle oh;
    pac.next(oh);
    msgpack::object obj = oh.get();
    Ret ret;
    obj.convert(ret);
    return ret;
}

}; // namespace FeverRPC

#endif // FEVERRPC_FEVERRPC_H_