project(snd)

add_library(snd
        RemoteSpeaker.h
        RemoteSpeaker.cpp
        internal/RemoteSpeakerManager.cpp
        internal/RemoteSpeakerManager.h
)

target_include_directories(snd PUBLIC ../)