cmake_minimum_required(VERSION 3.10)
project(boost_task_scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system thread)
find_library(RDKAFKA_LIBRARY rdkafka REQUIRED)

add_executable(kafka_consumer
    src/kafka/kafka_consumer.cpp
    src/scheduler/task_scheduler.cpp
)

target_include_directories(kafka_consumer PRIVATE src)

target_link_libraries(kafka_consumer
    PRIVATE Boost::system Boost::thread ${RDKAFKA_LIBRARY}
)
