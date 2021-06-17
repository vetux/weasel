find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Python COMPONENTS Interpreter Development)
message("Python_FOUND:${Python_FOUND}")
message("Python_VERSION:${Python_VERSION}")
message("Python_Development_FOUND:${Python_Development_FOUND}")
message("Python_LIBRARIES:${Python_LIBRARIES}")
message("Python_INCLUDE_DIRS:${Python_INCLUDE_DIRS}")

set(Frontend.Qt.GuiHeaders
        include/frontend/qt/mainwindow.hpp
        include/frontend/qt/prop/processpropertiesdialog.hpp
        include/frontend/qt/prop/tab/disktab.hpp
        include/frontend/qt/prop/tab/generaltab.hpp
        include/frontend/qt/prop/tab/networktab.hpp
        include/frontend/qt/prop/tab/threadstab.hpp
        include/frontend/qt/toolbar/cpubarwidget.hpp
        include/frontend/qt/toolbar/memorybarwidget.hpp
        include/frontend/qt/toolbar/toolbarwidget.hpp
        include/frontend/qt/tree/processtreewidget.hpp)

file(GLOB_RECURSE Frontend.Qt.Source src/frontend/qt/*.cpp)

qt5_wrap_cpp(Frontend.Qt.WrappedSource ${Frontend.Qt.GuiHeaders})

add_executable(weasel-qt ${Frontend.Qt.Source} ${Frontend.Qt.WrappedSource})

target_link_libraries(weasel-qt weasel)

target_link_libraries(weasel-qt Qt5::Core Qt5::Widgets) # Qt5
target_link_libraries(weasel-qt ${Python_LIBRARIES}) # Python

target_include_directories(weasel-qt PUBLIC include/)
target_include_directories(weasel-qt PUBLIC src/)