# Used to automate setup/building of the BTSDemo project

cmake_minimum_required(VERSION 3.16)

project(WindowsPong)
# by default CMake selects "ALL_BUILD" as the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
	PROPERTY VS_STARTUP_PROJECT WindowsPong)

# code assumes unicode win32 libraries
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

add_executable (WindowsPong winpong.cpp)
