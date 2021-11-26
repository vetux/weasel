find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

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

# weasel-qt

add_executable(weasel-qt ${Frontend.Qt.Source} ${Frontend.Qt.WrappedSource})

target_link_libraries(weasel-qt weasel)

target_link_libraries(weasel-qt Qt5::Core Qt5::Widgets) # Qt5

target_include_directories(weasel-qt PUBLIC frontend/include/)
target_include_directories(weasel-qt PUBLIC frontend/src/)

# weasel-qt-static

add_executable(weasel-qt-static ${Frontend.Qt.Source} ${Frontend.Qt.WrappedSource})

target_link_libraries(weasel-qt-static weasel-static)

target_link_libraries(weasel-qt-static Qt5::Core Qt5::Widgets) # Qt5

target_include_directories(weasel-qt-static PUBLIC frontend/include/)
target_include_directories(weasel-qt-static PUBLIC frontend/src/)
