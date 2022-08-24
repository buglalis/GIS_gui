cmake_minimum_required(VERSION 3.16)
project(GIS_gui)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(TARGET_SOURCE_DIR ${CMAKE_SOURCE_DIR})


if(NOT DEFINED ENV{QGIS_PATH})
    set(QGIS_PATH /home/vasiliy/qgis-install CACHE PATH "specify path to QGIS install directory")
endif()
message("QGIS_PATH is defined -- ${QGIS_PATH}")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Xml
        Sql
        REQUIRED)

link_directories(/usr/lib)
link_directories(${QGIS_PATH}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(GIS_gui  main.cpp
                        gismainwindow.cpp
                        gismainwindow.h
                        widgets/map/GISMapWidget.cpp
                        widgets/map/GISMapWidget.h
                        widgets/map/CentralWidget.cpp
                        widgets/map/CentralWidget.h
                        widgets/tools/MapActions.cpp
                        widgets/tools/MapActions.h
                        widgets/tools/CustomControlButton.cpp
                        widgets/tools/CustomControlButton.h
                        resources.qrc widgets/map/GISMapLayerManager.cpp widgets/map/GISMapLayerManager.h widgets/map/GISMapContoller.cpp widgets/map/GISMapContoller.h widgets/menu/MenuController.cpp widgets/menu/MenuController.h widgets/input/InputWidget.cpp widgets/input/InputWidget.h database/DatabaseWorker.cpp database/DatabaseWorker.h database/DatabaseBrowser.cpp database/DatabaseBrowser.h database/DatabaseController.cpp database/DatabaseController.h)

target_include_directories(GIS_gui PRIVATE
                                    ${QGIS_PATH}/include/qgis
                                    /usr/include/gdal)

target_link_directories(GIS_gui PRIVATE ${QGIS_PATH}/lib)
target_link_libraries(GIS_gui
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Xml
        Qt5::Sql
        qgis_native
        qgis_core
        qgis_app
        qgis_gui
        gdal
        )

file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/stylesheet/Aqua.qss DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
