cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

## Use -std=c++11 as default.
set(CMAKE_CXX_STANDARD 11)
## Disable C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)
## Require full C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(EpsilonNet LANGUAGES CXX)

set(DCMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall ")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DCMAKE_BUILD_TYPE=Debug")

#set(CMAKE_INSTALL_PREFIX /home/karina/CLionProjects/rbd/HElib-1.0.0/build)

#find_package(helib REQUIRED)
find_package(helib 1.0.0 EXACT REQUIRED
        PATHS /home/karina/CLionProjects/rbd/HElib-1.0.0/)

#add_executable( epsNet
add_executable(epsNetD
        impl/imp1_kmeans.cpp
        impl/KeysServer.cpp
        impl/Point.cpp
        impl/aux.cpp
        impl/yonis/run1meancore.cpp
        impl/properties.h)

#target_link_libraries(epsNet helib)
target_link_libraries(epsNetD helib)

#-DCMAKE_BUILD_TYPE=Debug
#add_executable(testing
#		#src
#		impl/imp1_kmeans.cpp
#		impl/aux.cpp
#		impl/KeysServer.cpp
#		impl/Point.cpp
#
#		#test
#		impl/tests/run_tests.cpp
#		impl/tests/Point_test.cpp
#		impl/tests/KeysServer_test.cpp
#		impl/tests/aux_test.cpp
#		impl/tests/imp1_kmeans_test.cpp
#
##		impl/KeysServer.h
##		impl/tests/KeysServer_test.h
##		impl/tests/imp1_kmeans_test.h
#		)
#target_link_libraries(testing helib)

# DangerZone
# Makes cmake add a file to the "real" directory in the "real" world (not on docker).
# 	without this CMake only copies files on reload, not build
#add_custom_target(copy_res_directory ALL
#		COMMAND ${CMAKE_COMMAND} -E copy_directory
#		"${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
#		COMMENT "Copy res directory to build tree"
#		VERBATIM)
