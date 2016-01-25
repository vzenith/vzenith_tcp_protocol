cmake_minimum_required(VERSION 2.8)
#########################################################################

# Basic environment setting
SET(BUILD_PROJECT_NAME "connected_test")

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

 if(WIN32)
	 ADD_DEFINITIONS(
	 )
 else()
	 ADD_DEFINITIONS(
	 )
 endif()
#########################################################################
#Step 3 : Add code source
SET(SRC_LIST 
	${CMAKE_CURRENT_SOURCE_DIR}/connected_test_main.cpp
	)
SOURCE_GROUP(${BUILD_PROJECT_NAME} FILES
	${CMAKE_CURRENT_SOURCE_DIR}/connected_test_main.cpp
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
	)
ELSE()
	TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
		${TCP_SDK_LINK_LIB}
	)
ENDIF()