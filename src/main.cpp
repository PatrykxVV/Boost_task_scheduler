#include "task_scheduler.hpp"
#include <iostream>
#include <thread>

int main()
{
    TaskScheduler scheduler(2);

    scheduler.submit([] {
        std::cout << "Task A (odrazu) w watku "
                    << std::this_thread::get_id() << '\n';
    });

    scheduler.schedule_after(std::chrono::milliseconds(1000), [] {
        std::cout << "Task B (po 1s) w watku "
                    << std::this_thread::get_id() << '\n';
    });

    scheduler.schedule_after(std::chrono::milliseconds(500), [] {
        std::cout << "Task C (po 0.5s) w watku "
            << std::this_thread::get_id() << '\n';
    });


    scheduler.wait();
}