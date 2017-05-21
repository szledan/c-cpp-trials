#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct Job {
    static constexpr int kNumber = 100000;

    Job(int ndx = 0) : index(ndx) {}

    int index;
    int buffer[kNumber];
};

std::mutex gQueueMutex;
std::condition_variable gQueueCondVar;
std::queue<Job> gQueue;

bool finished = false;

void producer(int n)
{
    for(int i = 0; i < n; ++i) {
        {
            std::lock_guard<std::mutex> lock(gQueueMutex);
            gQueue.push(Job(i));
            std::cout << "pushing " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
        gQueueCondVar.notify_all();
    }
}

void consumer()
{
    while (true) {
        Job job;
        {
            std::unique_lock<std::mutex> lock(gQueueMutex);
            gQueueCondVar.wait(lock, []{ return finished || !gQueue.empty(); });
            if (finished && gQueue.empty()) {
                break;
            }
            job = gQueue.front();
            gQueue.pop();
        }

        {
            const int repeat = 2000 * (job.index + 1);
            std::cerr << "consuming " << job.index << "; repeat " << repeat <<std::endl;

            for (int c = 0; c < repeat; ++c) {
                for (int i = 0; i < job.kNumber; ++i) {
                    job.buffer[i] = i;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

int main(int argc, char* argv[])
{
    std::thread prod(producer, 10);
    std::vector<std::thread> consumers;

    { // Constructor
        for (size_t i = 0; i < 20; ++i) {
            consumers.push_back(std::thread(consumer));
        }
    }

    // ...
    for (int f = 0; f < 10; ++f) {
        ;
    }

    { // Destructor
        finished = true;
        gQueueCondVar.notify_all();

        prod.join();
        for (size_t i = 0; i < consumers.size(); ++i) {
            consumers[i].join();
        }
    }

    std::cout << "finished!" << std::endl;

    return 0;
}
