#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <atomic>
#include <vector>
#include <thread>

// Prosty scheduler tasków z limitem równoległości
class TaskScheduler {
public:
    // threads – liczba wątków roboczych
    // max_tasks – ile tasków max może być wykonywanych naraz
    TaskScheduler(std::size_t threads, std::size_t max_tasks);
    ~TaskScheduler();

    // Próbuje dodać task (z backpressure)
    bool try_submit(std::function<void()> task);

    // Ile tasków aktualnie „w locie”
    std::size_t in_flight() const;

private:
    boost::asio::io_context io_;

    // Zapobiega zakończeniu io_context gdy chwilowo brak tasków
    boost::asio::executor_work_guard<
        boost::asio::io_context::executor_type> work_guard_;

    std::vector<std::thread> threads_;

    const std::size_t max_tasks_;
    std::atomic<std::size_t> in_flight_;
};
