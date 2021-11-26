file(GLOB_RECURSE System.Source system/src/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(weasel SHARED ${System.Source})

target_include_directories(weasel PUBLIC system/include/)
target_include_directories(weasel PRIVATE system/src/)

add_library(weasel-static STATIC ${System.Source})

target_include_directories(weasel-static PUBLIC system/include/)
target_include_directories(weasel-static PRIVATE system/src/)