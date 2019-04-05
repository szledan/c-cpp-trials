/*
 * Copyright (C) 2019, Szilard Ledan <szledan@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <chrono>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <mutex>
#include <thread>
#include <time.h>
#include <vector>

#define ACCUMULATE(TS_START, TS_STOP) double(TS_STOP.tv_sec - TS_START.tv_sec) + double(TS_STOP.tv_nsec - TS_START.tv_nsec) / (1000000000.0);

const int threadCount = 8;
std::mutex coutMutex;
//std::chrono::duration<std::chrono::high_resolution_clock, std::chrono::nanoseconds> realDuration[threadCount];
//timespec threadCPUtimeDuration[threadCount];

double from = -50.0;
double to = 50.0;
double dx = 0.00001;
double sum = 0.0;

void integral(int t)
{
    auto realStart = std::chrono::high_resolution_clock::now();
    struct timespec threadCPUtimeStart, threadCPUtimeStop;
    clockid_t clk_id = CLOCK_THREAD_CPUTIME_ID;
    clock_gettime(clk_id, &threadCPUtimeStart);

    const double stripe = (to - from) / double(threadCount);
    double sumPart = 0.0;
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        double x = t * stripe;
        do {
            sumPart += std::exp(-std::sin(x));
        } while ((x += dx) < (t * stripe + stripe));
    }

    std::chrono::duration<double> realDuration = std::chrono::high_resolution_clock::now() - realStart;
    clock_gettime(clk_id, &threadCPUtimeStop);

    double accum = ACCUMULATE(threadCPUtimeStart, threadCPUtimeStop);

    coutMutex.lock();
    std::cout << t << ".:" << std::fixed << std::setprecision(5)
              << " " << realDuration.count() << "sec"
              << " " << accum << "sec"
              << std::defaultfloat << std::setprecision(10) << " int(" << (t * stripe) << ", " << (t * stripe + stripe) << ")=" << sumPart * dx
              << std::endl;
    sum += sumPart * dx;
    coutMutex.unlock();
}

int main(int argc, char* argv[])
{
    std::vector<std::thread> threads;

    if (argc > 1) {
        from = std::atof(argv[1]);
    }
    if (argc > 2) {
        to = std::atof(argv[2]);
    }
    if (argc > 3) {
        dx = std::atof(argv[3]);
    }

    std::cout << "Usage: " << argv[0] << " [FROM [TO [DX]]]" << std::endl;
    std::cout << "f(x) = e^(-sin(x))" << std::endl
              << "dx=" << dx << std::endl
              << "thread count=" << threadCount << std::endl;

    auto realStart = std::chrono::high_resolution_clock::now();
    struct timespec threadCPUtimeStart, threadCPUtimeStop;
    clockid_t clk_id = CLOCK_THREAD_CPUTIME_ID;
    clock_gettime(clk_id, &threadCPUtimeStart);

    {
        for (int t = 0; t < threadCount; ++t) {
            threads.push_back(std::thread(integral, t));
        }
        for (int t = 0; t < threadCount; ++t) {
            threads[t].join();
        }
    }
    std::chrono::duration<double> realDuration = std::chrono::high_resolution_clock::now() - realStart;
    clock_gettime(clk_id, &threadCPUtimeStop);

    double accum = ACCUMULATE(threadCPUtimeStart, threadCPUtimeStop);
    std::cout << std::endl <<"Full time was:"
              << " " << realDuration.count() << "sec"
              << " " << accum << "sec"
              << " integral(from=" << from << ", to=" << to << ")=" << sum
              << std::endl;

    return 0;
}
