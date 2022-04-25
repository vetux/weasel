find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(Weasel.Frontend.Qt.GuiHeaders
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
        frontend/include/tabs/tree/processtreewidget.hpp)

file(GLOB_RECURSE Weasel.Frontend.Qt.Source frontend/src/*.cpp)

qt5_wrap_cpp(Weasel.Frontend.Qt.WrappedSource ${Weasel.Frontend.Qt.GuiHeaders})

# weasel-frontend-qt

add_executable(weasel-frontend-qt ${Weasel.Frontend.Qt.Source} ${Weasel.Frontend.Qt.WrappedSource})

target_link_libraries(weasel-frontend-qt weasel-library)

target_link_libraries(weasel-frontend-qt Qt5::Core Qt5::Widgets) # Qt5

target_include_directories(weasel-frontend-qt PUBLIC frontend/include/)
target_include_directories(weasel-frontend-qt PUBLIC frontend/src/)

# weasel-frontend-qt-static

add_executable(weasel-frontend-qt-static ${Weasel.Frontend.Qt.Source} ${Weasel.Frontend.Qt.WrappedSource})

target_link_libraries(weasel-frontend-qt-static weasel-library-static)

target_link_libraries(weasel-frontend-qt-static Qt5::Core Qt5::Widgets) # Qt5

target_include_directories(weasel-frontend-qt-static PUBLIC frontend/include/)
target_include_directories(weasel-frontend-qt-static PUBLIC frontend/src/)
