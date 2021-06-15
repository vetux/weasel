file(GLOB_RECURSE System.Source src/system/*.cpp)

add_library(weasel STATIC SHARED ${System.Source})

target_include_directories(weasel PUBLIC include/)
target_include_directories(weasel PUBLIC src/)