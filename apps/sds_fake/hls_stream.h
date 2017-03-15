#include <cassert>
#include <queue>
#include <iostream>
#include <string>
#ifndef HLS_STREAM_H
#define HLS_STREAM_H

namespace hls {
    template<typename T>
    class stream {
        std::string name;
        std::queue<T> q;
    public:
        stream(std::string name = "") :name(name) {
        }
        void write(T val) {
            q.push(val);
        }
        T read() {
            assert(!q.empty());
            T res = q.front();
            q.pop();
            return res;
        }
        ~stream() {
            if (!q.empty()) {
                std::cerr << name << " not empty!\n";
                std::cerr << q.size() << " element(s) left!\n";
            }
        }
    };
}

#endif
