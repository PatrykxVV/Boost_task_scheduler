# Asio Task Scheduler

Lekki **task scheduler w C++17** oparty o **Boost.Asio**, pokazujący praktyczne użycie Asio jako
**silnika wykonywania zdarzeń (event execution engine)**, a nie tylko biblioteki sieciowej.

Projekt demonstruje:
- model event-driven,
- jawne zarządzanie wątkami,
- opóźnione zadania (timers),
- poprawny shutdown oparty o RAII.

---

## Dlaczego ten projekt?

Większość przykładów Boost.Asio skupia się na socketach i networkingu.  
Ten projekt pokazuje **rdzeń Asio**:
> kolejkę zdarzeń + egzekucję w wątkach dostarczonych przez użytkownika.

Scheduler:
- **nie tworzy wątków magicznie**,
- **nie ukrywa modelu wykonania**,
- daje pełną kontrolę nad *kiedy* i *gdzie* task się wykona.

To dokładnie ten model, na którym bazują m.in. gRPC, ROS2 i systemy low-latency.

---

## Features

- Immediate task execution (`submit`)
- Delayed tasks via `steady_timer` (`schedule_after`)
- Multi-threaded execution using `io_context`
- Clean RAII-based shutdown
- No busy waiting, no `sleep_for` in scheduler logic

---

## Struktura projektu

src/
├── main.cpp # przykładowe użycie
├── task_scheduler.hpp # publiczny interfejs
└── task_scheduler.cpp # implementacja