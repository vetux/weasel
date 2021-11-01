file(GLOB_RECURSE System.Source system/src/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(weasel STATIC SHARED ${System.Source})

target_include_directories(weasel PUBLIC system/include/)
target_include_directories(weasel PRIVATE system/src/)