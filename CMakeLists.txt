cmake_minimum_required(VERSION 3.0)

project(toggle-nic)

add_executable(${CMAKE_PROJECT_NAME} "main.cpp" "DevicesHandler.cpp" "DevicesHandler.h")

add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE "WIN32_LEAN_AND_MEAN" "UNICODE")

find_library(LIB_SETUPAPI "setupapi")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIB_SETUPAPI})

if(MINGW)
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()
