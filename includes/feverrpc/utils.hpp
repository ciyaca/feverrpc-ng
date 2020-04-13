#pragma once
namespace std {
#ifndef DEBUG
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
        puts("destructing thread");
        if (t.joinable()) {
            printf("[%lld]waiting for anthor thread to destruct %lld \n",
                   std::this_thread::get_id(), t.get_id());

            t.join();
            printf("[%lld]thread destructed\n", std::this_thread::get_id());
        }
    }

    thread_guard(const thread_guard &) = delete;
    thread_guard &operator=(const thread_guard &) = delete;
};
