#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace cpputils
{
class Timer
{
public:
    Timer()
    {
        tic();
    }

    void tic()
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    void toc() const
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> el_double = now - start_;
        std::cout << "[Finished in " << el_double.count() << "ms]\n";
    }

    void toc(double& out) const
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> el_double = now - start_;
        out = el_double.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
} // namespace cpputils