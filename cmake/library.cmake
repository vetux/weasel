file(GLOB_RECURSE Weasel.Source library/src/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(weasel-library SHARED ${Weasel.Source})

target_include_directories(weasel-library PUBLIC library/include/)
target_include_directories(weasel-library PRIVATE library/src/)

set_target_properties(weasel-library PROPERTIES OUTPUT_NAME weasel)

add_library(weasel-library-static STATIC ${Weasel.Source})

target_include_directories(weasel-library-static PUBLIC library/include/)
target_include_directories(weasel-library-static PRIVATE library/src/)

set_target_properties(weasel-library-static PROPERTIES OUTPUT_NAME weasel)
