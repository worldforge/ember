add_library(mojoal
        mojoal.c)
target_include_directories(mojoal PUBLIC AL)
target_link_libraries(mojoal SDL2::SDL2)


