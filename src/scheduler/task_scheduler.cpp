#include "scheduler/task_scheduler.hpp"

TaskScheduler::TaskScheduler(std::size_t threads,
                             std::size_t max_tasks)
    : work_guard_(boost::asio::make_work_guard(io_)),
      max_tasks_(max_tasks),
      in_flight_(0)
{
    // Start puli wątków
    for (std::size_t i = 0; i < threads; ++i) {
        threads_.emplace_back([this] {
            io_.run();
        });
    }
}

TaskScheduler::~TaskScheduler() {
    // Zatrzymanie event loop i wątków
    work_guard_.reset();
    for (auto& t : threads_) {
        if (t.joinable())
            t.join();
    }
}

bool TaskScheduler::try_submit(std::function<void()> task) {
    std::size_t current = in_flight_.load();

    // CAS: próbujemy zarezerwować slot
    while (current < max_tasks_) {
        if (in_flight_.compare_exchange_weak(current, current + 1)) {
            boost::asio::post(io_, [this, task = std::move(task)] {
                task();          // wykonanie taska
                in_flight_--;    // zwolnienie slotu
            });
            return true;
        }
    }
    // Brak miejsca → backpressure
    return false;
}

std::size_t TaskScheduler::in_flight() const {
    return in_flight_.load();
}
