cmake_minimum_required(VERSION 3.16)

project(ipv6_check VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwidget.cpp mainwidget.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ipv6_check
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        NetworkInfo.h
        networkinterfacemanager.h networkinterfacemanager.cpp
        networkinterfacemodel.h networkinterfacemodel.cpp
        networkinterfacemodel.h networkinterfacemodel.cpp
        fronts.qrc
        include/ElaWidgetTools.qrc
        ModelView/T_TreeItem.cpp ModelView/T_TreeItem.h ModelView/T_TreeViewModel.cpp ModelView/T_TreeViewModel.h
        networkinterfacepage.h networkinterfacepage.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ipv6_check APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ipv6_check SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ipv6_check
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(include)

target_link_libraries(ipv6_check PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/elawidgettools.lib
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ipv6_check)
endif()
set_target_properties(ipv6_check PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ipv6_check
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ipv6_check)
endif()
