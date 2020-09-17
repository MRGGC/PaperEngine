cmake_minimum_required(VERSION 3.12)
project(PaperEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC -nostartfiles -Wall -O3 -Werror")

set(exe_name "engine")
set(lib_name "pengine")
set(src_dir "${PROJECT_SOURCE_DIR}/src/")
set(lib_dir "${PROJECT_SOURCE_DIR}/lib")
set(inc_dir "${PROJECT_SOURCE_DIR}/include/")

macro(SUBDIRLIST result curdir)
	file(GLOB children RELATIVE ${curdir} ${curdir}/*)
	set(dirlist "")
	foreach(child ${children})
		if(IS_DIRECTORY ${curdir}/${child})
			list(APPEND dirlist ${child})
		endif()
	endforeach()
	set(${result} ${dirlist})
endmacro()

SUBDIRLIST(SUBDIRS ${lib_dir})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE src_files "${src_dir}/*.cpp" "${src_dir}/*.c" "${src_dir}/*.hpp" "${src_dir}/*.h")

add_executable(${exe_name} ${src_files})
add_library(${lib_name} SHARED ${src_files})

#set_property(TARGET ${lib_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/$<CONFIG>/bin)
add_definitions(-DPENGINE_BUILD_SO)

target_include_directories(${exe_name} PUBLIC ${inc_dir})

set(link_libs "")

find_package(OpenGL REQUIRED)
list(APPEND link_libs "OpenGL::GL")

foreach(subdir ${SUBDIRS})
	add_subdirectory("lib/${subdir}")
	list(APPEND link_libs ${subdir})
endforeach()

target_link_libraries(${exe_name} PRIVATE ${link_libs})

set_target_properties(${lib_name} PROPERTIES VERSION ${PROJECT_VERSION})
