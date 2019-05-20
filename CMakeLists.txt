cmake_minimum_required(VERSION 3.0)

project(toggle_nic)

add_executable(toggle_nic "main.cpp" "DevicesHandler.cpp" "DevicesHandler.h")

add_compile_definitions(toggle_nic PRIVATE "WIN32_LEAN_AND_MEAN" "UNICODE")

find_library(LIB_SETUPAPI "setupapi")

target_link_libraries(toggle_nic PRIVATE ${LIB_SETUPAPI})

if(MINGW)
	set_target_properties(toggle_nic PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()
