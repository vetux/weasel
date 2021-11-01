find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Python COMPONENTS Interpreter Development)
message("Python_FOUND:${Python_FOUND}")
message("Python_VERSION:${Python_VERSION}")
message("Python_Development_FOUND:${Python_Development_FOUND}")
message("Python_LIBRARIES:${Python_LIBRARIES}")
message("Python_INCLUDE_DIRS:${Python_INCLUDE_DIRS}")

set(Frontend.Qt.GuiHeaders
        frontend/include/mainwindow.hpp
        frontend/include/dialog/processpropertiesdialog.hpp
        frontend/include/dialog/tab/disktab.hpp
        frontend/include/dialog/tab/generaltab.hpp
        frontend/include/dialog/tab/networktab.hpp
        frontend/include/dialog/tab/threadstab.hpp
        frontend/include/settings/settingsdialog.hpp
        frontend/include/toolbar/cpubarwidget.hpp
        frontend/include/toolbar/memorybarwidget.hpp
        frontend/include/toolbar/toolbarwidget.hpp
        frontend/include/tree/processtreewidget.hpp)

file(GLOB_RECURSE Frontend.Qt.Source frontend/src/*.cpp)

qt5_wrap_cpp(Frontend.Qt.WrappedSource ${Frontend.Qt.GuiHeaders})

add_executable(weasel-qt ${Frontend.Qt.Source} ${Frontend.Qt.WrappedSource})

target_link_libraries(weasel-qt weasel)

target_link_libraries(weasel-qt Qt5::Core Qt5::Widgets) # Qt5
target_link_libraries(weasel-qt ${Python_LIBRARIES}) # Python

target_include_directories(weasel-qt PUBLIC frontend/include/)
target_include_directories(weasel-qt PUBLIC frontend/src/)

# Change the output file name of the qt frontend to weasel
# because on linux the library filename is libweasel.so and therefore the executable can be named weasel
set_target_properties(weasel-qt PROPERTIES OUTPUT_NAME weasel)