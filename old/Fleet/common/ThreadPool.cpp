#include "ThreadPool.h"

void Thread::park() {
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work{boost::asio::make_work_guard(ctx_)};
    ctx_.run();
}

void ThreadPool::post(std::function<void()> fn)
{
    boost::asio::post(io_context_, fn);
}

void ThreadPool::start()
{
    for (auto i = 0; i < pool_size_; ++i)
    {
        user_threads_[i] = std::make_unique<Thread>(io_context_);
        threads_of_ctrl_[i] = std::make_unique<boost::thread>(&Thread::park, user_threads_[i].get());
    }
}

void ThreadPool::stop()
{
    io_context_.stop();
    for (auto i = 0; i < pool_size_; ++i)
    {
        threads_of_ctrl_[i]->join();
    }
}