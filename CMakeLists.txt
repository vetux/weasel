cmake_minimum_required(VERSION 3.17)

project(Weasel)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Python COMPONENTS Interpreter Development)
message("Python_FOUND:${Python_FOUND}")
message("Python_VERSION:${Python_VERSION}")
message("Python_Development_FOUND:${Python_Development_FOUND}")
message("Python_LIBRARIES:${Python_LIBRARIES}")
message("Python_INCLUDE_DIRS:${Python_INCLUDE_DIRS}")

file(GLOB_RECURSE HDR include/*.hpp)
file(GLOB_RECURSE HDR_GUI include/widgets/*.hpp)

file(GLOB_RECURSE SRC src/*.cpp)

qt5_wrap_cpp(WRAP_CPP ${HDR_GUI})

add_executable(weasel ${SRC} ${WRAP_CPP})

target_link_libraries(weasel Qt5::Core Qt5::Widgets) # Qt5
target_link_libraries(weasel ${Python_LIBRARIES}) # Python

target_include_directories(weasel PUBLIC include/)
target_include_directories(weasel PUBLIC src/)