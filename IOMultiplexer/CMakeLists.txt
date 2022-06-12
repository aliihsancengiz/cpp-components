set(TARGET_NAME iomultiplexer)


add_library(${TARGET_NAME}
    IOMultiplexer.cpp
)


target_include_directories(${TARGET_NAME}
    PUBLIC

    PRIVATE
)
target_link_libraries(${TARGET_NAME}
    PUBLIC

    PRIVATE
        pthread

)