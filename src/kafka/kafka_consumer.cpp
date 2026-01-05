#include <librdkafka/rdkafka.h>
#include <iostream>
#include <csignal>
#include <cstring>



static bool running = true;

void signal_handler(int) {
    running = false;
}

int main(){

    char errstr[512];

    // Rejestracja obslugi sygnalow
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // Konfiguracja customera
    rd_kafka_conf_t* conf = rd_kafka_conf_new();

    // Adres dla brokera kafka

    if(rd_kafka_conf_set(conf,
                        "bootstrap.servers",
                        "localhost:9092",
                        errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK){
                std::cerr << "Config error: " << errstr << '\n';
                return 1;
                        }

    // group.id jako identyfikator consumer group
    // Konsumenci z tym samym group.id dziela prace
    
    rd_kafka_conf_set(conf,
                    "group.id",
                    "scheduler-consumer",
                    nullptr,
                    0);

    /*
        Jeśli nie mam zapisanego offsetu
        -earliest = czytamy od poczatku topicu
        -latest = czytamy tylko nowe wiad.
    */ 

    rd_kafka_conf_set(conf,
                    "auto.offset.reset",
                    "earliest",
                    nullptr,
                    0);

    /*
        Tworzenie consumera, librdkafka zarzadza lifetime konfiguracji
    */
                    

    rd_kafka_t* consumer = 
        rd_kafka_new(RD_KAFKA_CONSUMER,
                    conf,
                    errstr,
                    sizeof(errstr));

}