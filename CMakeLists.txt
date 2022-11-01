cmake_minimum_required(VERSION 3.16)
project("IO Multiplexer POC" VERSION 0.1.1)

set(TARGET "ioMultiplexerPOC")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog REQUIRED)

add_subdirectory(IOMultiplexer)
add_subdirectory(Timer)
add_subdirectory(Event)
add_subdirectory(Logger)
add_subdirectory(ThreadPool)
add_subdirectory(IOContext)
add_subdirectory(Buffer)

add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${CMAKE_SOURCE_DIR}/IOMultiplexer
    ${CMAKE_SOURCE_DIR}/EventBus/include/EventBus
    ${CMAKE_SOURCE_DIR}/Timer
    ${CMAKE_SOURCE_DIR}/Event
    ${CMAKE_SOURCE_DIR}/Logger
    ${CMAKE_SOURCE_DIR}/ThreadPool
    ${CMAKE_SOURCE_DIR}/IOContext
    ${CMAKE_SOURCE_DIR}/Buffer
)

target_link_libraries(${TARGET}
    PUBLIC
    iomultiplexer
    timer
    event
    logger
    threadpool
    iocontext
    PRIVATE
)
