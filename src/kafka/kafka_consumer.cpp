#include "scheduler/task_scheduler.hpp"

#include <librdkafka/rdkafka.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>

static bool running = true;

// Ctrl+C → graceful shutdown
void signal_handler(int) {
    running = false;
}

int main() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // Scheduler: 2 wątki, max 4 taski naraz
    TaskScheduler scheduler(2, 4);

    char errstr[512];
    rd_kafka_conf_t* conf = rd_kafka_conf_new();

    // Adres brokera
    rd_kafka_conf_set(conf, "bootstrap.servers",
                      "localhost:9092", errstr, sizeof(errstr));

    // Consumer group
    rd_kafka_conf_set(conf, "group.id",
                      "task-scheduler-group", nullptr, 0);

    // Commit robimy ręcznie
    rd_kafka_conf_set(conf, "enable.auto.commit",
                      "false", nullptr, 0);

    rd_kafka_conf_set(conf, "auto.offset.reset",
                      "earliest", nullptr, 0);

    // Tworzenie consumera
    rd_kafka_t* consumer =
        rd_kafka_new(RD_KAFKA_CONSUMER, conf,
                     errstr, sizeof(errstr));

    rd_kafka_poll_set_consumer(consumer);

    // Subskrypcja topicu
    rd_kafka_topic_partition_list_t* topics =
        rd_kafka_topic_partition_list_new(1);
    rd_kafka_topic_partition_list_add(
        topics, "price-events", -1);
    rd_kafka_subscribe(consumer, topics);
    rd_kafka_topic_partition_list_destroy(topics);

    std::cout << "Kafka consumer started\n";

    // Główna pętla
    while (running) {
        rd_kafka_message_t* msg =
            rd_kafka_consumer_poll(consumer, 1000);

        if (!msg)
            continue;

        if (!msg->err) {
            std::string payload(
                static_cast<char*>(msg->payload),
                msg->len);

            // Event → task
            bool accepted = scheduler.try_submit([payload] {
                std::cout << "Processing: " << payload
                          << " (thread "
                          << std::this_thread::get_id() << ")\n";
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(500));
            });

            // Commit tylko jeśli task przyjęty
            if (accepted) {
                rd_kafka_commit_message(consumer, msg, 0);
            } else {
                std::cout << "Scheduler full – backpressure\n";
            }
        }

        rd_kafka_message_destroy(msg);
    }

    std::cout << "Shutting down\n";
    rd_kafka_consumer_close(consumer);
    rd_kafka_destroy(consumer);
}
