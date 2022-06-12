set(TARGET_NAME timer)


add_library(${TARGET_NAME}
    Timer.cpp
)


target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/IOMultiplexer
    PRIVATE
)
target_link_libraries(${TARGET_NAME}
    PUBLIC

    PRIVATE
        iomultiplexer
)