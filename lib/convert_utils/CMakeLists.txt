cmake_minimum_required(VERSION 3.11)

# THANK https://stackoverflow.com/questions/43260990/cmake-linking-against-a-library-linked-to-qt5
# THANK https://scm.gforge.inria.fr/anonscm/svn/gratin/trunk/cmake/FindQt5.cmake

project(convert_utils CXX C)

#set(CMAKE_INCLUDE_CURRENT_DIR ON) 

#set(CMAKE_AUTOMOC ON)  
#set(CMAKE_AUTOUIC ON)     
#set(CMAKE_INCLUDE_CURRENT_DIR ON) 
#REQUIRED
#find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Multimedia)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Multimedia)
#Qt5_FOUND
if (QT_FOUND)
    message(STATUS "QT: FOUND on convert_utils")
    include_directories(${QT_INCLUDES})
else()
    message(STATUS "QT: NOT FOUND on convert_utils")
endif()

set(SRCS
	binary.cpp
	)
	
set(HEADERS
	binary.hpp
	binary_imp.hpp
	)
if (QT_FOUND)
	set(SRCS ${SRCS} qt_utils.cpp)
	set(HEADERS ${HEADERS} qt_utils.hpp)
endif()
if(SFML_FOUND)
	set(SRCS ${SRCS} sfml_utils.cpp)
	set(HEADERS ${HEADERS} sfml_utils.hpp)
endif()
#add_library(convert_utils_static STATIC ${SRCS} ${HEADERS})
add_library(convert_utils SHARED ${SRCS} ${HEADERS})

target_include_directories(convert_utils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
if (QT_FOUND)
	target_link_libraries(convert_utils PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Multimedia)
endif()
#export(TARGETS testLib FILE testLib-exports.cmake)