# Asio Task Scheduler

Lekki **task scheduler w C++17** oparty o **Boost.Asio**, pokazujący praktyczne użycie Asio jako
**silnika wykonywania zdarzeń (event execution engine)**, a nie wyłącznie biblioteki sieciowej.

Projekt skupia się na mechanice:
- modelu **event-driven**,
- jawnego zarządzania **pulą wątków**,
- kolejkowania i wykonywania zadań,
- kontrolowanego zamykania aplikacji (**RAII / graceful shutdown**).

Scheduler stanowi prostą, czytelną bazę pod systemy asynchroniczne
(np. przetwarzanie zdarzeń, integracje z message brokerami).
