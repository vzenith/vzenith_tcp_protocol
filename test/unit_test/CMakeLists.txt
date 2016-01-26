cmake_minimum_required(VERSION 2.8)
#########################################################################

# Basic environment setting
SET(BUILD_PROJECT_NAME "unit_test")

#########################################################################
#INCLUDE_DIRECTORIES(${LIBVZNET_INCLUDE_DIR})
MESSAGE(STATUS "Print the include directores")
get_property(inc_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(inc_dir ${inc_dirs})
  message(STATUS "    include directory='${inc_dir}'")
endforeach()

#########################################################################
#Step 2 : Add library directories
MESSAGE(STATUS "Step 2 : Add library directories")
#LINK_DIRECTORIES(${LIBVZNET_LIBRARY_DIR})
MESSAGE(STATUS "Print the link directores")
get_property(link_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(link_dir ${link_dirs})
  message(STATUS "    link directory='${link_dir}'")
endforeach()

IF(WIN32)
	SET(PLATFORM_STR_LIST
		${CMAKE_CURRENT_SOURCE_DIR}/win32_unittest.cpp
		)
ELSEIF(APPLE)
	SET(PLATFORM_STR_LIST
		)
ELSE()
	SET(PLATFORM_STR_LIST
		)
ENDIF()
#########################################################################
#Step 3 : Add code source
SET(SRC_LIST 
	${CMAKE_CURRENT_SOURCE_DIR}/unittest_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gunit.h
	${CMAKE_CURRENT_SOURCE_DIR}/base64_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/basictypes_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/buffer_unittest.cpp
	#
	#${CMAKE_CURRENT_SOURCE_DIR}/bytebuffer_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/byteorder_unittest.cpp
	#
	#${CMAKE_CURRENT_SOURCE_DIR}/sigslot_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/stream_unittest.cpp
	#
	#${CMAKE_CURRENT_SOURCE_DIR}/stringencode_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/stringutils_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/timeutils_unittest.cpp
	#${PLATFORM_STR_LIST}
	)
SOURCE_GROUP(${BUILD_PROJECT_NAME} FILES
	${CMAKE_CURRENT_SOURCE_DIR}/unittest_main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/gunit.h
	${CMAKE_CURRENT_SOURCE_DIR}/base64_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/basictypes_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/buffer_unittest.cpp
	#
	#${CMAKE_CURRENT_SOURCE_DIR}/bytebuffer_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/byteorder_unittest.cpp
	#
	#${CMAKE_CURRENT_SOURCE_DIR}/sigslot_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/stream_unittest.cpp
	#
	#${CMAKE_CURRENT_SOURCE_DIR}/stringencode_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/stringutils_unittest.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/timeutils_unittest.cpp
	#${PLATFORM_STR_LIST}
)
#########################################################################
#Step 4 : Add PROJECT define 
MESSAGE(STATUS "Step 4 : Add code source")

#########################################################################
#Step 5 : Add executable or library target
MESSAGE(STATUS "Step 5 : Add executable or library target")
ADD_executable(${BUILD_PROJECT_NAME} ${SRC_LIST})
set_property(TARGET ${BUILD_PROJECT_NAME} PROPERTY FOLDER ${PROJECT_SET_NAME_TEST})
#########################################################################
#Step 6 : link with other library
MESSAGE(STATUS "Step 6 : link with other library")

IF(WIN32)
	TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
		${TCP_SDK_LINK_LIB}
		Ws2_32.lib
		gtest.lib
	)
ELSE()
	TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
		gtest.a
		${TCP_SDK_LINK_LIB}
	)
ENDIF()

add_test(${BUILD_PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_PROJECT_NAME})