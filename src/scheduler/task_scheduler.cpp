#include "task_scheduler.hpp"
#include <iostream>


TaskScheduler::TaskScheduler(std::size_t thread_count):
    work_guard_(boost::asio::make_work_guard(io_))
    {
        // Zostaja uruchomione watki, ktore beda wolac io_.run()
        for(std::size_t i = 0; i < thread_count; i++){
            threads_.emplace_back([this]{
                io_.run();
            });
        }
    }

    TaskScheduler::~TaskScheduler(){
        // Przekazujemy, ze 'wiecej pracy nie bedzie'
        work_guard_.reset();

        //Czekamy az watki sie skoncza
        for(auto& t : threads_){
            if(t.joinable())
                t.join();
        }
    }

    
    void TaskScheduler::submit(std::function<void()> task){
        boost::asio::post(io_, std::move(task));
    }

    void TaskScheduler::schedule_after(std::chrono::milliseconds delay, std::function<void()> task)
    {
        // Timer MUSI KONIECZNIE zyc do momentu odpalenia handlera
        auto timer = std::make_shared<boost::asio::steady_timer>(io_, delay);

        timer->async_wait(
            [task = std::move(task), timer]
            (const boost::system::error_code& ec)
            {
                if(!ec){
                    task();
                }
            }
        );
    }

  void TaskScheduler::wait(){
        // Koniec przyjmowania pracy
        work_guard_.reset();

        for(auto& t : threads_){
            if(t.joinable())
                t.join();
        }
    }
