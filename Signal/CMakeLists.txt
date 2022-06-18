set(TARGET_NAME signal)


add_library(${TARGET_NAME}
    Signal.cpp
)


target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/IOMultiplexer
        ${CMAKE_SOURCE_DIR}/Logger
    PRIVATE
)
target_link_libraries(${TARGET_NAME}
    PUBLIC

    PRIVATE
        iomultiplexer
        logger
)