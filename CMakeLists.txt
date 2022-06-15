cmake_minimum_required(VERSION 3.16)
project("IO Multiplexer POC" VERSION 0.0.4)

set(TARGET "ioMultiplexerPOC")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(IOMultiplexer)
add_subdirectory(EventBus)
add_subdirectory(Timer)
add_subdirectory(Event)

add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/IOMultiplexer
    ${CMAKE_SOURCE_DIR}/EventBus
    ${CMAKE_SOURCE_DIR}/Timer
    ${CMAKE_SOURCE_DIR}/Event
)

target_link_libraries(${TARGET}
    PUBLIC
        iomultiplexer
        timer
        event
    PRIVATE
)
