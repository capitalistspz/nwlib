project(ut)

add_library(ut
        Thread.cpp
        Thread.h
        ThreadHandler.cpp
        ThreadHandler.h
        MessageQueue.cpp
        MessageQueue.h

)

target_include_directories(ut PUBLIC ../)