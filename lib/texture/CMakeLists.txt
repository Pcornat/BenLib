cmake_minimum_required(VERSION 3.11)

project(texture CXX C)

set(SRCS
	sfml_screenshot.cpp
	load_titlemap.cpp
	load_texture.cpp
	load_texturemap.cpp
	load_title.cpp
	texture.cpp
	)
	
set(HEADERS
	sfml_screenshot.hpp
	load_titlemap.hpp
	load_texture.hpp
	load_texturemap.hpp
	load_title.hpp
	texture.hpp
	)
	

#add_library(texture_static STATIC ${SRCS} ${HEADERS})
add_library(texture SHARED ${SRCS} ${HEADERS})
