# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(GenericCryptanalyzer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
    src
    src/box
    src/helpers
)

set (SRCS
    examples/simplecipher.cpp
    src/box/abstractbitshiftbox.cpp
    src/box/abstractbitshiftbox.h
    src/box/abstractbox.cpp
    src/box/abstractbox.h
    src/box/ebox.cpp
    src/box/ebox.h
    src/box/identitybox.cpp
    src/box/identitybox.h
    src/box/pbox.cpp
    src/box/pbox.h
    src/box/sbox.cpp
    src/box/sbox.h
    src/box/xorbox.cpp
    src/box/xorbox.h
    src/helpers/helpers.cpp
    src/helpers/helpers.h
    src/cipheranalyzer.cpp
    src/cipheranalyzer.h
    src/roundfunction.cpp
    src/roundfunction.h
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
