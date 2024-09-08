cmake_minimum_required(VERSION 3.27)
project(TableData)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()

if(NOT DEFINED ENV{QT_HOME})
    if (MSVC)
        set(CMAKE_PREFIX_PATH E:/IDE/Qt/6.4.3/msvc2019_64)
    else()
    endif()
else()
    if (MSVC)
        set(CMAKE_PREFIX_PATH $ENV{QT_HOME}/msvc2019_64)
    else()
        set(CMAKE_PREFIX_PATH $ENV{QT_HOME}/mingw_64/lib/cmake)
    endif()
endif()


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(TABLE_DATA_SOURCE
        ${DATA_SOURCE}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/table_model.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/table_view.cpp
)

add_executable(TableData main.cpp ${TABLE_DATA_SOURCE})
target_link_libraries(TableData
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# 添加整体快速构建
#add_custom_target(build_all
#        COMMAND ${CMAKE_COMMAND} --build . --target TestPluginInterface
#        && ${CMAKE_COMMAND} --build . --target CutBoardPlugin
#        && ${CMAKE_COMMAND} --build . --target PluginSystem
#)

#if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
#    set(DEBUG_SUFFIX)
#    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
#        set(DEBUG_SUFFIX "d")
#    endif ()
#    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
#    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
#        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
#        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
#            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
#        endif ()
#    endif ()
#    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E make_directory
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
#    endif ()
#    foreach (QT_LIB Core Gui Widgets)
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy
#                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
#                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
#    endforeach (QT_LIB)
#endif ()

