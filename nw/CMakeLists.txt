project(nw)


add_subdirectory(snd)
add_subdirectory(ut)

add_library(nw INTERFACE)

target_include_directories(nw INTERFACE ../)

target_link_libraries(nw INTERFACE snd ut)