file(GLOB_RECURSE Core.Source src/core/*.cpp)

add_library(weasel STATIC SHARED ${Core.Source})

target_include_directories(weasel PUBLIC include/)
target_include_directories(weasel PUBLIC src/)