# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(player LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/multimedia/player")

find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets Network)

qt_add_executable(OdisseyRadio
    main.cpp
    mainwindow.cpp mainwindow.h





)

set_target_properties(OdisseyRadio PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(OdisseyRadio PUBLIC
    Qt::MultimediaWidgets
    Qt::Network
)

install(TARGETS OdisseyRadio
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
