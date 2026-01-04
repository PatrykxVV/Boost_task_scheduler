cmake_minimum_required(VERSION 3.10)
project(boost_task_scheduler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task_scheduler
    src/main.cpp
    src/task_scheduler.cpp
)

target_include_directories(task_scheduler PRIVATE src)
target_link_libraries(task_scheduler PRIVATE pthread)