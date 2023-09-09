/*
- nodeflow -
BSD 3-Clause License

Copyright (c) 2022, Ruwen Kohm
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace nf
{
template <typename Accuracy>
class Timer
{
    using DurationAccuracy = Accuracy;

public:
    Timer()
    {
        tic();
    }

    inline void tic()
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    inline void toc() const
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, DurationAccuracy> el_double = now - start_;
        std::cout << "[Finished in " << el_double.count() << "]\n";
    }

    inline void toc(double& out) const
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, DurationAccuracy> el_double = now - start_;
        out = el_double.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

template <typename Accuracy = std::micro>
class ScopedTimer
{
public:
    ScopedTimer()
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~ScopedTimer()
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, Accuracy> el_double = now - start_;
        std::cout << "[Finished in " << el_double.count() << "]\n";
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};
} // namespace nf