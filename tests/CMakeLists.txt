cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(TARGET_NAME Tests)
project(${TARGET_NAME})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/$<CONFIG>)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB TEST_FILES ${SOURCE_DIR}/*.*)
add_executable(${TARGET_NAME} ${TEST_FILES})

option(INSTALL_GTEST OFF)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)	
	if(MSVC)
		foreach(_tgt gtest gtest_main gmock gmock_main)
		target_compile_definitions(${_tgt}
			PRIVATE
				"_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
		)
		endforeach()
	endif()
endif()

set_target_properties(
    gtest gtest_main gmock gmock_main
    PROPERTIES FOLDER "gtest"
)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/LNMesh/public)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/LNData/public)
target_include_directories(${TARGET_NAME} PUBLIC ${LNLib_DIR}/include)

target_link_libraries(${TARGET_NAME} LNMesh gtest gtest_main)
target_link_libraries(${TARGET_NAME} LNData gtest gtest_main)

add_dependencies(${TARGET_NAME} LNMesh)
add_dependencies(${TARGET_NAME} LNData)

if(MSVC)

	add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LNLib_DIR}/bin/$<CONFIG>/LNLib.dll ${CMAKE_BINARY_DIR}/$<CONFIG>
    )

	file(GLOB OCC_DLLS ${OCC_DIR}/bin/*.dll)
    foreach(Current IN LISTS OCC_DLLS)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>/OCCT 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Current} ${CMAKE_BINARY_DIR}/$<CONFIG>/OCCT)
    endforeach()

	file(GLOB TEST_DATA_FILES ${SOURCE_DIR}/TFiles/*.*)
	foreach(Current IN LISTS TEST_DATA_FILES)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$<CONFIG>/TFILES 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Current} ${CMAKE_BINARY_DIR}/$<CONFIG>/TFILES)
    endforeach()

endif()