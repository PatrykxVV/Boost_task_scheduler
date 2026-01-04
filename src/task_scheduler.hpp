#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <chrono>
#include <memory>

class TaskScheduler{
    public:
        explicit TaskScheduler(std::size_t thread_count);
        ~TaskScheduler();

        // Zadanie do wykonania ,,jak najszybciej''
        void submit(std::function<void()> task);

        // Zadanie do wykonania po czasie
        void schedule_after(std::chrono::milliseconds delay,
                            std::function<void()> task);


        void wait();
        
    private:
            boost::asio::io_context io_;
            boost::asio::executor_work_guard<
                boost::asio::io_context::executor_type> work_guard_;

        std::vector<std::thread> threads_;
};