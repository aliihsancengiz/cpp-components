set(TARGET_NAME iomultiplexer)


add_library(${TARGET_NAME}
    IOMultiplexer.cpp
)


target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Logger
    PRIVATE
        ${CMAKE_SOURCE_DIR}/ThreadPool
)
target_link_libraries(${TARGET_NAME}
    PUBLIC

    PRIVATE
        pthread
        logger

)