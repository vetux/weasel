file(GLOB_RECURSE System.Source src/system/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(weasel STATIC SHARED ${System.Source})

target_include_directories(weasel PUBLIC include/)
target_include_directories(weasel PUBLIC src/)