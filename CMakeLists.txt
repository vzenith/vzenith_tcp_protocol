#F:\code\osc\aliyun_opensearch\aliyun_opensearch\builds\vs2013>cmake ../.. -DCMAK
#E_BUILD_TYPE=DEBUG -DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DJSONCPP_WITH_TE
#STS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF
#  
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(vzentih_tcp_sdk)
set_property(GLOBAL PROPERTY USE_FOLDERS On)

SET(PROJECT_SET_NAME_TEST "test")
SET(PROJECT_SET_NAME_LIB "lib")
SET(PROJECT_SET_NAME_THIRD_PART "third_part")
#SET(PROJECT_MAIN_SRC_LIST "")
option(ENABLE_UNIT_TEST "Set to ON to USE GTest unit testing." ON)
option(COVERALLS "Generate coveralls data" OFF)
##########################################################
## Step 1 
SET(JSON_CPP ${PROJECT_SOURCE_DIR}/third_part/jsoncpp)
SET(LIBEVENT ${PROJECT_SOURCE_DIR}/third_part/libevent)
SET(GTEST ${PROJECT_SOURCE_DIR}/third_part/gtest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} 
	${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake)

if (COVERALLS)
	include(Coveralls)
	coveralls_turn_on_coverage()
endif()

##1. INCLUDE_DIRECTORES
MESSAGE(STATUS "Step 1 :  Add include directores")
SET(TCP_SDK_INCLUDE_DIR
	${JSON_CPP}/include
	${PROJECT_SOURCE_DIR}/src
	${LIBEVENT}/include
	${PROJECT_BINARY_DIR}/third_part/libevent/include
	${GTEST}/googletest/include
)

##2 LIBRARY PATH
MESSAGE(STATUS "Step 2 :  Add library path and public link library")
IF(WIN32)
	MESSAGE(STATUS "       WINDOWS")
	SET(TCP_SDK_LIBRARY_DIR
		${PROJECT_SOURCE_DIR}/lib/win
		)
	SET(TCP_SDK_LINK_LIB
		jsoncpp.lib
		winmm.lib
		libvztcpsdk.lib
		)
ELSE()
	MESSAGE(STATUS "       LINUX")
	SET(TCP_SDK_LIBRARY_DIR
		${PROJECT_SOURCE_DIR}/lib/unix
		)
	SET(TCP_SDK_LINK_LIB
		libjsoncpp.a
		libvztcpsdk.a
		-lpthread
		-lrt
		)
ENDIF()

##3 add defines
MESSAGE(STATUS "Step 3 :  Add More defines")
IF(WIN32)
	 ADD_DEFINITIONS(
		 -D_CRT_SECURE_NO_WARNINGS
		 -D_SCL_SECURE_NO_WARNINGS
	 )
ELSEIF(APPLE)
	 ADD_DEFINITIONS(
		-DPOSIX
		-DOSX
		-framework Foundation
	 )
ELSE()
	 ADD_DEFINITIONS(
		-DPOSIX
	 )
ENDIF()
INCLUDE_DIRECTORIES(${TCP_SDK_INCLUDE_DIR})
LINK_DIRECTORIES(${TCP_SDK_LIBRARY_DIR})
#####################################################################
# Step 3 :Set visual studio runtime type
if(WIN32)
	set(CompilerFlags
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			)
	foreach(CompilerFlag ${CompilerFlags})
	  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	  string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
	  #string(REPLACE "/EDITANDCONTINUE" "/SAFESEH" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	
################################################################
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/win")
	
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/win")
		 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/win")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/win")
else()
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_SOURCE_DIR}/lib/unix")

	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  "${PROJECT_SOURCE_DIR}/lib/unix")
	 
	# With Release properties
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/bin/unix")
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/unix")
	SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  "${PROJECT_SOURCE_DIR}/lib/unix")
ENDIF()
##########################################################
## Setp 3
ADD_SUBDIRECTORY(third_part/jsoncpp)
IF(ENABLE_UNIT_TEST)
	ADD_SUBDIRECTORY(third_part/gtest)
ENDIF()

ADD_SUBDIRECTORY(src)

# TEST
#  ADD_SUBDIRECTORY(test/libevent_test)
ADD_SUBDIRECTORY(test/thead_test)
ADD_SUBDIRECTORY(test/session_test)
ADD_SUBDIRECTORY(test/sdk_test)
ADD_SUBDIRECTORY(test/connected_test)

IF(ENABLE_UNIT_TEST)
	enable_testing()
	ADD_SUBDIRECTORY(test/unit_test)
ENDIF()
