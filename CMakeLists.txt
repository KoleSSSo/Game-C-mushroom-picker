cmake_minimum_required(VERSION 3.16)

project(Game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    styledialog.cpp
    styledialog.h
    records.cpp
    records.h
    settingsdialog.cpp
    settingsdialog.h
    gamescreen.cpp  # Убедитесь, что файл существует
    gamescreen.h
)

add_executable(Game ${PROJECT_SOURCES}
    mushroom.h mushroom.cpp)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)
    set_source_files_properties(resources.qrc PROPERTIES QT_RESOURCE_ALIAS "/")
    qt_add_resources(Game "app_resources"
        PREFIX "/"
        BASE ${CMAKE_CURRENT_SOURCE_DIR}
        FILES resources.qrc
    )
endif()

target_link_libraries(Game PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
)

set_target_properties(Game PROPERTIES
    AUTOMOC ON  # Важно для обработки Q_OBJECT
    AUTOUIC ON
    AUTORCC ON
)

if(QT_VERSION_MAJOR EQUAL 6 AND QT_VERSION_MINOR GREATER 2)
    qt_finalize_executable(Game)
endif()
