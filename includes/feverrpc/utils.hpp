#pragma once
namespace std {
#ifdef DEBUG
#define dbgprintf(format, args...)                                               \
    printf("[%s,%s,%d] " format "\n", __FILE__, __FUNCTION__, __LINE__, args)
#define dbgputs(string)                                                          \
    printf("[%s,%s,%d] " string "\n", __FILE__, __FUNCTION__, __LINE__)
#else
#define dbgprintf(format, args...)                                               \
    {}
#define dbgputs(string)                                                          \
    {}
#endif

} // namespace std

#include <thread>

class thread_guard {
    std::thread &t;

  public:
    explicit thread_guard(std::thread &_t) : t(_t){};

    ~thread_guard() {
        dbgputs("destructing thread");
        if (t.joinable()) {
            dbgprintf("[%lld]waiting for anthor thread to destruct %lld",
                   std::this_thread::get_id(), t.get_id());

            t.join();
            dbgprintf("[%lld]thread destructed", std::this_thread::get_id());
        }
    }

    thread_guard(const thread_guard &) = delete;
    thread_guard &operator=(const thread_guard &) = delete;
};
