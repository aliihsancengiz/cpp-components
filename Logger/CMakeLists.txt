set(TARGET_NAME logger)

add_library(${TARGET_NAME}
    Logger.cpp
)

target_include_directories(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        
    PRIVATE
)

