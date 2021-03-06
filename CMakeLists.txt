cmake_minimum_required (VERSION 3.13)
project(BENLIB VERSION 0.0.5 LANGUAGES CXX C)

#   ==================================================
#   ================    CMAKE CONF    ================
#   ==================================================


set(CMAKE_INCLUDE_CURRENT_DIR 1)
set(CMAKE_VERBOSE_MAKEFILE 1)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
    set(CMAKE_UNITY_BUILD 1)
    message(STATUS "CMAKE_UNITY_BUILD : 1")
else()
    set(CMAKE_UNITY_BUILD 0)
    message(STATUS "CMAKE_UNITY_BUILD : 0")
endif()

set(CMAKE_DEBUG_POSTFIX "_d")

#set(Var1 "This is variable 1" CACHE INTERNAL "")
#set(Var1 "This is variable 1" PARENT_SCOPE)

#   ================================================
#   ================    COMPILER    ================
#   ================================================

if(NOT CMAKE_C_COMPILER)
#    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_C_COMPILER /usr/bin/gcc)
endif()
if(NOT CMAKE_CXX_COMPILER)
#    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()

#=== C VERSION CHECK ===

if(NOT CMAKE_C_STANDARD)
    include(CheckCCompilerFlag)
    CHECK_C_COMPILER_FLAG("-std=c11" COMPILER_SUPPORTS_C11)
    CHECK_C_COMPILER_FLAG("-std=c99" COMPILER_SUPPORTS_C99)
    CHECK_C_COMPILER_FLAG("-std=c90" COMPILER_SUPPORTS_C90)
    
    if(COMPILER_SUPPORTS_C11)
        message(STATUS "C11: OK")
        set(CMAKE_C_STANDARD 11)
    elseif(COMPILER_SUPPORTS_C99)
        message(STATUS "C99: OK")
        set(CMAKE_C_STANDARD 99)
    elseif(COMPILER_SUPPORTS_C90)
        message(STATUS "C90: OK")
        set(CMAKE_C_STANDARD 90)
    else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 or above support. Please use a different C++ compiler.")
    endif()
endif()

#=== C++ VERSION CHECK ===
if(NOT CMAKE_CXX_STANDARD)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++23" COMPILER_SUPPORTS_CXX23)
    CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
    CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
    CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++03" COMPILER_SUPPORTS_CXX03)
    CHECK_CXX_COMPILER_FLAG("-std=c++98" COMPILER_SUPPORTS_CXX98)

    if(COMPILER_SUPPORTS_CXX20)
        set(CMAKE_CXX_STANDARD 20)
        message(STATUS "C++20: OK")
    elseif(COMPILER_SUPPORTS_CXX17)
        set(CMAKE_CXX_STANDARD 17)
        message(STATUS "C++17: OK")
    elseif(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_STANDARD 14)
        message(WARNING "C++14: Error")
        message(FATAL_ERROR "C++14 is old, please use newer compiler.")
    elseif(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_STANDARD 11)
        message(WARNING "C++11: OK")
        message(FATAL_ERROR "C++11 is old, please use newer compiler.")
    elseif(COMPILER_SUPPORTS_CXX03)
        set(CMAKE_CXX_STANDARD 03)
        message(WARNING "C++03: OK")
        message(FATAL_ERROR "C++03 is old, please use newer compiler.")
    elseif(COMPILER_SUPPORTS_CXX98)
        set(CMAKE_CXX_STANDARD 98)
        message(WARNING "C++98: OK")
        message(FATAL_ERROR "C++98 is old, please use newer compiler.")
    else()
        message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 or above support. Please use a different C++ compiler.")
    endif()

    # Compiler-specific C++11 activation.
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # require at least gcc 4.8
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
            message(FATAL_ERROR "GCC version must be at least 4.8!")
            set(CMAKE_CXX_STANDARD 14)
        endif()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # require at least clang 3.2
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
            message(FATAL_ERROR "Clang version must be at least 3.2!")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "14.0")
        message(FATAL_ERROR "Insufficient msvc version")
        endif()
    else()
        message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
    endif()
endif()



set(CMAKE_CXX_STANDARD_REQUIRED 1)
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS 1)
endif()

#=== OPTIMIZATION FLAGS ===
#-mavx2 -flto
set(OPTI_COMPILER_REL "-march=native -s -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,--strip-all")

#=== SECURITY RELEASE FLAGS ===

set(SEC_COMPILER_REL "-fstack-clash-protection -fstack-protector-all -Werror=format-security -fstack-protector-strong -fexceptions -D_FORTIFY_SOURCE=2 -fPIE")
#For Ubuntu 18.04 and 16.04
#CHECK_CXX_COMPILER_FLAG("-fstack-clash-protection" COMPILER_SUPPORTS_SEC_FLAG)
#if(NOT COMPILER_SUPPORTS_SEC_FLAG)
#    list(APPEND CHECK_CXX_COMPILER_FLAG "/usr/share/SFML/cmake/Modules")
#endif()

set(SEC_COMPILER_LINK_REL "-fPIC -Wl,-z,now -Wl,-z,relro -Wl,--sort-common,--as-needed -fuse-ld=gold")
#-flto

set(WARNINGS_COMPILER_C "-Wall -Wpedantic -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wstrict-overflow=5 -Wswitch-default -Wunreachable-code -Wcast-align")
#-std=c99

set(CMAKE_C_FLAGS                "${WARNINGS_COMPILER_C} -pipe -fuse-ld=gold")
set(CMAKE_C_FLAGS_RELEASE        "-O3 ${OPTI_COMPILER_REL} ${SEC_COMPILER_REL}")
set(CMAKE_C_FLAGS_MINSIZEREL     "-Os ${OPTI_COMPILER_REL} ${SEC_COMPILER_REL}")
set(CMAKE_C_FLAGS_DEBUG          "-g3 -Og -ggdb3 -v -march=native")
set(CMAKE_C_FLAGS_COVERAGE "${CMAKE_C_FLAGS_DEBUG} --coverage")

#-Wold-style-cast -Wdouble-promotion -fstack-usage
set(WARNINGS_COMPILER_CXX "-Wpedantic -Wall -Wextra -Wshadow -Wstrict-overflow=5 -Wmissing-declarations -Wundef -Wstack-protector -Wno-unused-parameter")
set(WARNINGS_COMPILER_CXX "${WARNINGS_COMPILER_CXX} -Wmissing-include-dirs -Wmissing-noreturn -Wimport -Winit-self -Winvalid-pch -Wstrict-aliasing=2 -Wswitch-default -Wunreachable-code -Wunused")
set(WARNINGS_COMPILER_CXX "${WARNINGS_COMPILER_CXX} -Woverloaded-virtual  -Wdisabled-optimization -Winline -Wredundant-decls -Wsign-conversion -Wformat-nonliteral -Wformat-security")
set(WARNINGS_COMPILER_CXX "${WARNINGS_COMPILER_CXX} -Wwrite-strings -Wcast-align -Wcast-qual -Wfloat-equal -Wvariadic-macros -Wpacked  -Wpadded -Wpointer-arith -Weffc++ -Wformat=2 -Wfloat-equal -Wabi=11")
set(WARNINGS_COMPILER_CXX "${WARNINGS_COMPILER_CXX} -Wduplicated-branches -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference")

# Removed : -Wconversion -Wuseless-cast
#-ftime-report -static -lrt -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -flto

#Add -lstdc++fs for old compiler
set(CMAKE_CXX_FLAGS                "${WARNINGS_COMPILER_CXX} -fuse-ld=gold -pipe -pthread -lstdc++fs -Wl,-rpath,../lib -Wl,-rpath,../external/lib -pthread")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os ${OPTI_COMPILER_REL} ${SEC_COMPILER_REL} -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O3 ${OPTI_COMPILER_REL} ${SEC_COMPILER_REL} -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG          "-g3 -Og -ggdb3 -v -march=native")
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage")

set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=gold")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${SEC_COMPILER_LINK_REL}")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${SEC_COMPILER_LINK_REL}")
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")

#set(BUILD_SHARED_LIBRARIES 0)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(PROJECT_BINARY_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set(CTEST_BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#   ================    COMPILER DEF    ================

add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")

add_definitions(-DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS})
add_definitions(-DCMAKE_C_EXTENSIONS=${CMAKE_C_EXTENSIONS})
add_definitions(-DCMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
add_definitions(-DCMAKE_C_COMPILER_ID="${CMAKE_C_COMPILER_ID}")
add_definitions(-DCMAKE_CXX_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
add_definitions(-DCMAKE_C_COMPILER_VERSION="${CMAKE_C_COMPILER_VERSION}")


if(CMAKE_SIZEOF_VOID_P EQUAL 2)
    message(STATUS "16 bit mode")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "32 bit mode")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64 bit mode")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 16)
    message(STATUS "128 bit mode")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 32)
    message(STATUS "256 bit mode")
else()
    message(STATUS "unknow size")
endif()

#   =================================================
#   ==============    EXTERNAL LIBS    ==============
#   =================================================

include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

#ExternalProject_Add(opencv_lib
#    GIT_REPOSITORY https://github.com/boostorg/boost
#    GIT_TAG 4.4.0
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
    #CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
#)

#   ================    SFML    ================
#set(SFML_USE_STATIC_STD_LIBS 0)
#set(SFML_STATIC_LIBRARIES 0)

set(SFML_ROOT "C:\\Project Files (x86)\\SFML")

find_package(SFML 2.5 COMPONENTS graphics audio network system QUIET)

if(SFML_FOUND)
    message(STATUS "SFML: FOUND")
    add_compile_definitions(SFML_VERSION="${SFML_VERSION}")
else()
    message(STATUS "SFML: NOT FOUND")
    if(NOT EXTERNAL_LIB)
        set(EXTERNAL_LIB "")
    endif()
    string(APPEND EXTERNAL_LIB "sfml_lib")
    ExternalProject_Add(sfml_lib
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        #CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    )
endif()

#   ================    BOOST    ================
if (WIN32)
    set(Boost_USE_STATIC_LIBS TRUE)
    set(Boost_USE_MULTITHREADED TRUE)
    set(Boost_USE_STATIC_RUNTIME FALSE)
else ()
    set(Boost_USE_STATIC_LIBS FALSE)
    set(Boost_USE_MULTITHREADED TRUE)
    set(Boost_USE_STATIC_RUNTIME FALSE)
endif (WIN32)

#find_package( Boost 1.72.0 COMPONENTS thread system fiber context program_options filesystem REQUIRED)
find_package(Boost 1.67.0 COMPONENTS thread filesystem system unit_test_framework QUIET)

if(Boost_FOUND)
    message(STATUS "Boost FOUND")
    include_directories( ${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARIES})
else()
    message(STATUS "Boost: NOT FOUND")
endif()

#   ================    OPENCV    ================
find_package(OpenCV REQUIRED QUIET)

if (OPENCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OPENCV: FOUND")
else()
    message(STATUS "OPENCV: NOT FOUND")
    if(NOT EXTERNAL_LIB)
        set(EXTERNAL_LIB "")
    endif()
    string(APPEND EXTERNAL_LIB "opencv_lib")
    ExternalProject_Add(opencv_lib
        GIT_REPOSITORY https://github.com/opencv/opencv.git
        GIT_TAG 4.4.0
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        #CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    )
endif()


#   ================    OPENGL    ================
find_package(OpenGL REQUIRED QUIET)
find_package(GLUT REQUIRED QUIET)

if (OPENGL_FOUND)
    include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )
    link_directories(${OPENGL_LIBRARIES})
    message(STATUS "OPENCL: FOUND")
else()
    message(STATUS "OPENCL: NOT FOUND")
endif()

#   ================    VULKAN    ================
find_package(Vulkan QUIET)

if (VULKAN_FOUND)
    include_directories( ${Vulkan_INCLUDE_DIR})
    link_directories(${Vulkan_LIBRARY})
    message(STATUS "VULKAN: FOUND")
else()
    message(STATUS "VULKAN: NOT FOUND")
endif()

#   ================    OPENSSL    ================

find_package(OpenSSL QUIET)

if (OPENSSL_FOUND)
    include_directories( ${OPENSSL_INCLUDE_DIR})
    link_directories(${OPENSSL_LIBRARIES})
    message(STATUS "OPENSSL: FOUND")
else()
    message(STATUS "OPENSSL: NOT FOUND")
endif()

#   ================    Qt    ================

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Multimedia)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Multimedia)
find_package(Qt${QT_VERSION_MAJOR}QuickCompiler)
#qtquick_compiler_add_resources(RESOURCES example.qrc)
#qt5_use_modules(myapp Quick Widgets Core Gui Multimedia Network)

#find_package(Qt5 COMPONENTS Quick Widgets Core Gui Multimedia Network REQUIRED)

if (QT_FOUND)
    message(STATUS "QT: FOUND")
    include_directories( ${QT_INCLUDES})
else()
    message(STATUS "QT: NOT FOUND")
endif()

#   ================    OGRE3D    ================
#if(UNIX)
#    set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake;${CMAKE_MODULE_PATH}")
#endif(UNIX)

#if(WIN32)
#	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
#	set(OGRE_SAMPLES_INCLUDEPATH
#		$ENV{OGRE_HOME}/Samples/include
#	)
#endif(WIN32)

#find_package(OGRE QUIET)

#if (OGRE_FOUND)
#    include_directories( ${ogre_INCLUDE_DIR})
#    link_directories(${OGRE_LIBRARIES})
#    message(STATUS "OGRE: FOUND")
#else()
#    message(STATUS "OGRE: NOT FOUND")
#endif()

#   ================    OIS    ================

#   =================================================
#   =============    INTERNAL LIBS    ===============
#   =================================================
#set(BASEPATH "${CMAKE_SOURCE_DIR}")
#include_directories("${BASEPATH}")

set(lib_path "")

include_directories("${lib_path}lib/")
include_directories("${lib_path}lib/C/")



# C libs
add_subdirectory(${lib_path}lib/C/libhello "src/lib/lib/C/libhello")
add_subdirectory(${lib_path}lib/C/string_c "src/lib/lib/C/string_c")

add_subdirectory(${lib_path}lib/C/string_is "src/lib/lib/C/string_is_c")
add_subdirectory(${lib_path}lib/C/pointer "src/lib/lib/C/pointer_c")
add_subdirectory(${lib_path}lib/C/display "src/lib/lib/C/display_c")
add_subdirectory(${lib_path}lib/C/integer "src/lib/lib/C/integer_c")


# C++ libs
add_subdirectory(${lib_path}lib/convert_utils "src/lib/lib/convert_utils")

add_subdirectory(${lib_path}lib/texture "src/lib/lib/texture")

add_subdirectory(${lib_path}lib/math "src/lib/lib/math")

add_subdirectory(${lib_path}lib/utils/opencv "src/lib/lib/utils/opencv")

add_subdirectory(${lib_path}lib/utils/opengl "src/lib/lib/utils/opengl")

#add_subdirectory(src/lib/utils/sfml "src/lib/lib/utils/sfml")

add_subdirectory(${lib_path}lib/time "src/lib/lib/time")

add_subdirectory(${lib_path}lib/cpu "src/lib/lib/cpu")

add_subdirectory(${lib_path}lib/vector "src/lib/lib/vector")

add_subdirectory(${lib_path}lib/thread "src/lib/lib/thread")

add_subdirectory(${lib_path}lib/filesystem "src/lib/lib/filesystem")

add_subdirectory(${lib_path}lib/string_lib "src/lib/lib/string_lib")

add_subdirectory(${lib_path}lib/crypto "src/lib/lib/crypto")

add_subdirectory(${lib_path}lib/std "src/lib/lib/std")

if(EXTERNAL_LIB)
    add_dependencies(hello ${EXTERNAL_LIB})
    add_dependencies(convert_utils ${EXTERNAL_LIB})
    add_dependencies(texture ${EXTERNAL_LIB})
    add_dependencies(math ${EXTERNAL_LIB})
    add_dependencies(utils_opencv ${EXTERNAL_LIB})
    add_dependencies(utils_opengl ${EXTERNAL_LIB})
#   add_dependencies(utils_sfml ${EXTERNAL_LIB})
    add_dependencies(time ${EXTERNAL_LIB})
    add_dependencies(cpu ${EXTERNAL_LIB})
    add_dependencies(vector ${EXTERNAL_LIB})
    add_dependencies(thread ${EXTERNAL_LIB})
    add_dependencies(filesystem ${EXTERNAL_LIB})
    add_dependencies(my_crypto ${EXTERNAL_LIB})
    add_dependencies(std ${EXTERNAL_LIB})
endif()

#   =================================================
#   =============    SOURCES FILES    ===============
#   =================================================

#   ================    FILES    ================
set(MAIN src/main.cpp)

set(SRCS
    )

set(HEADERS
    )

set(VIRUS 
    )

set(BENCH_ISPRIME ${lib_path}src/bench/bench_isprime.cpp)

set(BENCH_VECTOR_COPY ${lib_path}src/bench/bench_vector_copy.cpp)

set(BENCH_VECTOR_SORT ${lib_path}src/bench/bench_vector_sort.cpp)

set(BENCH_DIGITS_COUNT ${lib_path}src/bench/bench_digits_count.cpp)

set(BENCH_FIND_MAX ${lib_path}src/bench/bench_find_max.cpp)

set(BENCH_PC ${lib_path}src/bench/benchmark_pc.cpp)

set(LIST_ALL_FILE ${lib_path}src/exemple/list_all_file.cpp)

set(PARSE_CSV ${lib_path}src/exemple/parse_csv.cpp)

set(GTA_SA_CHEAT ${lib_path}src/exemple/GTA_SA_cheat.cpp)


if (OPENSSL_FOUND)
    set(LIST_ALL_FILE_HASH ${lib_path}src/exemple/list_all_file_hash.cpp)
    set(CRYPTO_AES ${lib_path}src/exemple/crypto_AES256.cpp)
    set(CRYPTO_RSA ${lib_path}src/exemple/crypto_RSA.cpp)
endif()

set(COUNT_ALL_FILE ${lib_path}src/exemple/count_all_file.cpp)

if (OPENCV_FOUND)
    set(IMAGE_DIFF ${lib_path}src/exemple/image_diff.cpp)
    set(LIST_ALL_FILE_OPENCV ${lib_path}src/exemple/list_all_file_opencv.cpp)
endif()

#   ====================================================
#   ====================    LIBS    ====================
#   ====================================================

#set(MAIN_LLIBS Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Multimedia sfml-graphics sfml-window sfml-audio sfml-network sfml-system ${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OpenCV_LIBS})
#set(MAIN_LLIBS sfml-graphics sfml-window sfml-audio sfml-network sfml-system ${Boost_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OpenCV_LIBS})

set(MAIN_LLIBS "")

if(Boost_FOUND)
    set(MAIN_LLIBS ${MAIN_LLIBS} ${Boost_LIBRARIES})
endif()

if(SFML_FOUND)
    set(MAIN_LLIBS ${MAIN_LLIBS} sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
endif()

if (OPENCV_FOUND)
    set(MAIN_LLIBS ${MAIN_LLIBS} ${OpenCV_LIBS})
endif()

if (OPENGL_FOUND)
    set(MAIN_LLIBS ${MAIN_LLIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif()

if (QT_FOUND)
    set(MAIN_LLIBS ${MAIN_LLIBS} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Multimedia)
endif()

set(CUSTOM_LIBS texture convert_utils math utils_opencv utils_opengl time cpu vector thread filesystem my_crypto std string_lib)

#   ==================================================
#   ===============    EXEC FILES    =================
#   ==================================================


#add_executable(game ${MAIN} ${SRCS} ${HEADERS})
#if(EXTERNAL_LIB)
#    add_dependencies(game ${EXTERNAL_LIB})
#endif()

# stdc++fs is needed for Debian
#target_link_libraries(game PRIVATE ${MAIN_LLIBS} ${CUSTOM_LIBS} stdc++fs)


#add_executable(virus ${VIRUS})
#if(EXTERNAL_LIB)
#    add_dependencies(virus ${EXTERNAL_LIB})
#endif()

# stdc++fs is needed for Debian
#target_link_libraries(virus PRIVATE ${MAIN_LLIBS} ${CUSTOM_LIBS} stdc++fs)

add_executable(bench_isprime ${BENCH_ISPRIME})
target_link_libraries(bench_isprime PRIVATE thread vector math time stdc++fs)
add_dependencies(bench_isprime ${CUSTOM_LIBS})

add_executable(bench_vector_copy ${BENCH_VECTOR_COPY})
target_link_libraries(bench_vector_copy PRIVATE thread vector math time stdc++fs)
add_dependencies(bench_vector_copy ${CUSTOM_LIBS})

add_executable(bench_vector_sort ${BENCH_VECTOR_SORT})
target_link_libraries(bench_vector_sort PRIVATE thread vector math time stdc++fs)
add_dependencies(bench_vector_sort ${CUSTOM_LIBS})

add_executable(bench_digits_count ${BENCH_DIGITS_COUNT})
target_link_libraries(bench_digits_count PRIVATE thread vector math time stdc++fs)
add_dependencies(bench_digits_count ${CUSTOM_LIBS})

add_executable(bench_find_max ${BENCH_FIND_MAX})
target_link_libraries(bench_find_max PRIVATE thread vector math time)
add_dependencies(bench_find_max ${CUSTOM_LIBS})

add_executable(bench_pc ${BENCH_PC})
target_link_libraries(bench_pc PRIVATE time vector)
add_dependencies(bench_pc ${CUSTOM_LIBS})

add_executable(gta_sa_cheat ${GTA_SA_CHEAT})
target_link_libraries(gta_sa_cheat PRIVATE thread thread)
add_dependencies(gta_sa_cheat ${CUSTOM_LIBS})


if(Boost_FOUND)
    add_executable(list_all_file ${LIST_ALL_FILE})
    target_link_libraries(list_all_file PRIVATE ${Boost_LIBRARIES} filesystem stdc++fs)
    add_dependencies(list_all_file ${CUSTOM_LIBS})
endif()

add_executable(parse_csv ${PARSE_CSV})
target_link_libraries(parse_csv PRIVATE string_lib)
add_dependencies(parse_csv ${CUSTOM_LIBS})

if (OPENSSL_FOUND)
    add_executable(list_all_file_hash ${LIST_ALL_FILE_HASH})
    target_link_libraries(list_all_file_hash PRIVATE ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} filesystem stdc++fs my_crypto thread time)
    add_dependencies(list_all_file_hash ${CUSTOM_LIBS})

    add_executable(crypto_AES ${CRYPTO_AES})
    target_link_libraries(crypto_AES PRIVATE ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} filesystem stdc++fs my_crypto thread time)
    add_dependencies(crypto_AES ${CUSTOM_LIBS})

    add_executable(crypto_RSA ${CRYPTO_RSA})
    target_link_libraries(crypto_RSA PRIVATE ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} filesystem stdc++fs my_crypto thread time)
    add_dependencies(crypto_RSA ${CUSTOM_LIBS})
endif()

if (OPENCV_FOUND)
    add_executable(list_all_file_opencv ${LIST_ALL_FILE_OPENCV})
    target_link_libraries(list_all_file_opencv PRIVATE filesystem ${OpenCV_LIBS} stdc++fs)
    add_dependencies(list_all_file_opencv ${CUSTOM_LIBS})

    add_executable(image_diff ${IMAGE_DIFF})
    target_link_libraries(image_diff PRIVATE filesystem ${OpenCV_LIBS} stdc++fs utils_opencv)
    add_dependencies(image_diff ${CUSTOM_LIBS})
endif()

enable_testing()

#Test getSchwarzschild
set(TEST_SCHWARZSCHILD test_getSchwarzschild)
add_executable(${TEST_SCHWARZSCHILD} ${lib_path}src/test/math/test_getSchwarzschild.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_SCHWARZSCHILD} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_SCHWARZSCHILD} ${Boost_LIBRARIES})
add_test(NAME ${TEST_SCHWARZSCHILD} COMMAND ${CMAKE_COMMAND} --build . --target ${TEST_SCHWARZSCHILD})

#Test getGravitationalAttraction
set(TEST_GA test_getGravitationalAttraction)
add_executable(${TEST_GA} ${lib_path}src/test/math/test_getGravitationalAttraction.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_GA} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_GA} ${Boost_LIBRARIES})
add_test(NAME ${TEST_GA} COMMAND ${TEST_GA})

#Test sphere
set(TEST_SPHERE test_sphere)
add_executable(${TEST_SPHERE} ${lib_path}src/test/math/test_sphere.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_SPHERE} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_SPHERE} ${Boost_LIBRARIES} quadmath)
add_test(NAME ${TEST_SPHERE} COMMAND ${TEST_SPHERE})

#Test cylinder
set(TEST_CYLINDER test_cylinder)
add_executable(${TEST_CYLINDER} ${lib_path}src/test/math/test_cylinder.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_CYLINDER} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_CYLINDER} ${Boost_LIBRARIES})
add_test(NAME ${TEST_CYLINDER} COMMAND ${TEST_CYLINDER})

#Test vector sort
set(TEST_VECTOR_SORT test_vector_sort)
add_executable(${TEST_VECTOR_SORT} ${lib_path}src/test/vector/test_vector_sort.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_VECTOR_SORT} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_VECTOR_SORT} ${Boost_LIBRARIES} vector)
add_test(NAME ${TEST_VECTOR_SORT} COMMAND ${TEST_VECTOR_SORT})

#Test vector copy
set(TEST_VECTOR_COPY test_vector_copy)
add_executable(${TEST_VECTOR_COPY} ${lib_path}src/test/vector/test_vector_copy.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_VECTOR_COPY} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_VECTOR_COPY} ${Boost_LIBRARIES} vector)
add_test(NAME ${TEST_VECTOR_COPY} COMMAND ${TEST_VECTOR_COPY})

#Test vector copy
set(TEST_VECTOR_MAX_SIMD test_vector_max_simd)
add_executable(${TEST_VECTOR_MAX_SIMD} ${lib_path}src/test/vector/test_vector_max_simd.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_VECTOR_MAX_SIMD} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_VECTOR_MAX_SIMD} ${Boost_LIBRARIES} vector)
add_test(NAME ${TEST_VECTOR_MAX_SIMD} COMMAND ${TEST_VECTOR_MAX_SIMD})

if (OPENSSL_FOUND)
    #Test crypto
    set(TEST_CRYPTO test_crypto)
    add_executable(${TEST_CRYPTO} ${lib_path}src/test/crypto/test_crypto.cpp)
    if(EXTERNAL_LIB)
        add_dependencies(${TEST_CRYPTO} ${EXTERNAL_LIB})
    endif()
    target_link_libraries(${TEST_CRYPTO} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} my_crypto)
    add_test(NAME ${TEST_CRYPTO} COMMAND ${TEST_CRYPTO})

    #Test crypto AES
    set(TEST_CRYPTO_AES test_crypto_aes)
    add_executable(${TEST_CRYPTO_AES} ${lib_path}src/test/crypto/test_crypto_AES.cpp)
    if(EXTERNAL_LIB)
        add_dependencies(${TEST_CRYPTO_AES} ${EXTERNAL_LIB})
    endif()
    target_link_libraries(${TEST_CRYPTO_AES} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} my_crypto)
    add_test(NAME ${TEST_CRYPTO_AES} COMMAND ${TEST_CRYPTO_AES})
endif()

#Test C string
set(TEST_REV_STR test_rev_str)
add_executable(${TEST_REV_STR} ${lib_path}src/test/C/string_c/test_string.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_REV_STR} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_REV_STR} ${Boost_LIBRARIES} string_c)
add_test(NAME ${TEST_REV_STR} COMMAND ${TEST_REV_STR})

set(TEST_IS_NUM test_my_isnum)
add_executable(${TEST_IS_NUM} ${lib_path}src/test/C/string_is/test_my_isnum.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_IS_NUM} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_IS_NUM} ${Boost_LIBRARIES} string_is_c)
add_test(NAME ${TEST_IS_NUM} COMMAND ${TEST_IS_NUM})

set(TEST_STR_IS_NUM test_my_str_isnum)
add_executable(${TEST_STR_IS_NUM} ${lib_path}src/test/C/string_is/test_my_str_isnum.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_STR_IS_NUM} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_STR_IS_NUM} ${Boost_LIBRARIES} string_is_c)
add_test(NAME ${TEST_STR_IS_NUM} COMMAND ${TEST_STR_IS_NUM})


#Test power
set(TEST_POWER test_power)
add_executable(${TEST_POWER} ${lib_path}src/test/math/test_power.cpp)
if(EXTERNAL_LIB)
    add_dependencies(${TEST_POWER} ${EXTERNAL_LIB})
endif()
target_link_libraries(${TEST_POWER} ${Boost_LIBRARIES})
add_test(NAME ${TEST_POWER} COMMAND ${TEST_POWER})
#add_test(NAME ${TEST_POWER} COMMAND ${CMAKE_COMMAND} --build . --target ${TEST_POWER})

#add_custom_command(
#     TARGET test_power
#     COMMENT "Run tests"
#     POST_BUILD 
#     WORKING_DIRECTORY .
#     COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failures --extra-verbose
#)

#   ================================================
#   ===============    COPY FILES    ===============
#   ================================================

#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/screenshot)

#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})
#configure_file(font/Almond_Caramel.ttf ${CMAKE_BINARY_DIR}/font/Almond_Caramel.ttf COPYONLY)



#add_custom_command(
#    TARGET game
#    COMMENT "CP Texture"
#    POST_BUILD 
#    WORKING_DIRECTORY .
#    COMMAND cp -R -u -p "../texture/PIPOYA FREE RPG Character Sprites 32x32" "texture/PIPOYA FREE RPG Character Sprites 32x32"
#)

#   ================================================
#   ===============    UNSET VALUE    ===============
#   ================================================


#unset(CMAKE_BUILD_TYPE CACHE)

#   ================================================
#   ===============    COPY FILES    ===============
#   ================================================

#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/screenshot)

#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})
#configure_file(font/Almond_Caramel.ttf ${CMAKE_BINARY_DIR}/font/Almond_Caramel.ttf COPYONLY)


#add_custom_command(
#    TARGET game
#    COMMENT "CP Texture"
#    POST_BUILD 
#    WORKING_DIRECTORY .
#    COMMAND cp -R -u -p "../texture/PIPOYA FREE RPG Character Sprites 32x32" "texture/PIPOYA FREE RPG Character Sprites 32x32"
#)


#   ================================================
#   ===============    UNSET VALUE    ===============
#   ================================================


#unset(CMAKE_BUILD_TYPE CACHE)