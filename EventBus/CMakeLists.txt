set(TARGET_NAME eventbus)


add_library(${TARGET_NAME}
    EventBus.cpp
)


target_include_directories(${TARGET_NAME}
    PUBLIC

    PRIVATE
)
target_link_libraries(${TARGET_NAME}
    PUBLIC

    PRIVATE

)