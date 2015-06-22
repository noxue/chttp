cmake_minimum_required (VERSION 2.8)
project(chttp)

set(HTTPS 1)

#定义编译选项
add_definitions("-Wall -ansi -g -std=c99 -DBUILD")


#头文件查找目录
include_directories(./include/chttp/)

#添加源代码
aux_source_directory(./src/ CHTTP_SRC)

#输出库的路径
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

#生成静态库名称
add_library(chttp STATIC ${CHTTP_SRC})

if(WIN32)
	find_library(WS2_32_LIBRARY ws2_32)
	if(WS2_32_LIBRARY)
		target_link_libraries(chttp ${WS2_32_LIBRARY})
	endif(WS2_32_LIBRARY)

	message(STATUS "win32 平台")
elseif(LINUX)
	message(STATUS "LINUX 平台")
	target_link_libraries(chttp -lsocket)
endif(WIN32)


if(HTTPS)
	add_definitions("-D_HTTPS")
	target_link_libraries(chttp ssl crypto)
endif(HTTPS)
add_subdirectory(test)