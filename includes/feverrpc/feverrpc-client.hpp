#include <feverrpc/feverrpc.hpp>

namespace FeverRPC {

class Client : public FeverRPC {
  private:
    // client to server socket
    int _c2s_socket_handler;
    // server to client socket
    int _s2c_socket_handler;

    // interm call
    template <typename Ret> Ret socket_call(msgpack::sbuffer &buffer);
    // listen handler
    void listen(const int &_s2c_socket_handler);
  public:
    Client(const char *host);
    ~Client();
    // server to client
    void s2c();
    // make rpc call
    template <typename Ret, typename... Args>
    Ret call(std::string name, Args... args);
};

// FeverRPC::Client

template <typename Ret, typename... Args>
Ret Client::call(std::string name, Args... args) {
    msgpack::sbuffer buffer;

    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    // push func name
    pk.pack(name);
    // push args with types
    pk.pack(std::make_tuple(args...));

    // call and return
    return socket_call<Ret>(buffer);
}

template <typename Ret> Ret Client::socket_call(msgpack::sbuffer &buffer) {
    // protecting programming
    if (_c2s_socket_handler < 0) {
        connect_socket(_HOST, _C2S_PORT, _c2s_socket_handler);
    }
    // socket_call_(socket_handler, buffer.data(), buffer.size(), buff);
    msgpack::sbuffer recv_buff;

    print_sbuffer(buffer);

    send_and_recv(_c2s_socket_handler, buffer.data(), buffer.size(), recv_buff);
    // deserializes these objects using msgpack::unpacker.
    return unpack_ret_val<Ret>(recv_buff);
}

} // namespace FeverRPC