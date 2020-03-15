cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

## Use -std=c++11 as default.
set(CMAKE_CXX_STANDARD 11)
## Disable C++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)
## Require full C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(try
		LANGUAGES CXX)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

find_package(helib 1.0.0 EXACT REQUIRED)

#add_executable(run impl/imp1_kmeans.cpp
#		impl/Binary.cpp impl/Point.cpp impl/aux.cpp impl/Skeys.cpp impl/Skeys.h)
#target_link_libraries(run helib)

#add_executable(run old/HEcmp.h  old/compare2numbers.cpp  old/Contextor.cpp )
#add_executable(run old/scratch.cpp)
add_executable(testing
		#src
#		impl/aux.cpp
		impl/KeysServer.cpp
		impl/Point.cpp
		impl/PointExtended_old.cpp

		#test
		impl/tests/run_tests.cpp
		impl/tests/Point_test.cpp
		impl/tests/KeysServer_test.cpp

		impl/KeysServer.h  impl/tests/KeysServer_test.h impl/PointExtended_old.h)
target_link_libraries(testing helib)

# DangerZone
# Makes cmake add a file to the "real" directory in the "real" world (not on docker).
# 	without this CMake only copies files on reload, not build
#add_custom_target(copy_res_directory ALL
#		COMMAND ${CMAKE_COMMAND} -E copy_directory
#		"${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}"
#		COMMENT "Copy res directory to build tree"
#		VERBATIM)
