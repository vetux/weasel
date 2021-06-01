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

if (WIN32)
    file(GLOB_RECURSE SRC src/widgets/*.cpp src/main.cpp src/sched/win32/*.cpp)
elseif (UNIX)
    file(GLOB_RECURSE SRC src/widgets/*.cpp src/main.cpp src/sched/posix/*.cpp)
endif ()

file(GLOB_RECURSE HDR include/*.hpp)
file(GLOB_RECURSE HDR_GUI include/widgets/*.hpp)

qt5_wrap_cpp(WRAP_CPP ${HDR_GUI})

if (WIN32)
    add_executable(weasel WIN32 ${SRC} ${WRAP_CPP})
else ()
    add_executable(weasel ${SRC} ${WRAP_CPP})
endif ()

target_link_libraries(weasel Qt5::Core Qt5::Widgets) # Qt5
target_link_libraries(weasel ${Python_LIBRARIES}) # Python

target_include_directories(weasel PUBLIC include/)
target_include_directories(weasel PUBLIC src/)