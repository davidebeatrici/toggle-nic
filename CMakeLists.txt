cmake_minimum_required(VERSION 3.12)

project(toggle-nic
  VERSION 1.0.0.1
  DESCRIPTION "A simple command-line tool that allows you to easily toggle (enable/disable) a network interface."
  LANGUAGES CXX
)

string(TIMESTAMP CURRENT_YEAR "%Y")

configure_file("resources/toggle-nic.rc.in" "toggle-nic.rc")

add_executable(${CMAKE_PROJECT_NAME}
  "main.cpp"

  "DevicesHandler.cpp"
  "DevicesHandler.h"
  "WinUtils.cpp"
  "WinUtils.h"

  "toggle-nic.rc"
)

add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE "WIN32_LEAN_AND_MEAN" "UNICODE")

find_library(LIB_SETUPAPI "setupapi")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_SETUPAPI})

if(MINGW)
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()
