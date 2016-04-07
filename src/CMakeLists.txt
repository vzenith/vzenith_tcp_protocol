cmake_minimum_required(VERSION 2.8)
#########################################################################

# Basic environment setting

if(WIN32)
	SET(BUILD_PROJECT_NAME "libvztcpsdk")
elseif(UNIX)
	SET(BUILD_PROJECT_NAME "vztcpsdk")
endif()

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

#########################################################################
#Step 3 : Add code source
IF(WIN32)
	SET(PLATFORM_STR_LIST
		${CMAKE_CURRENT_SOURCE_DIR}/base/win32.h
		${CMAKE_CURRENT_SOURCE_DIR}/base/win32.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/base/win32socketinit.h
		${CMAKE_CURRENT_SOURCE_DIR}/base/win32socketinit.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/base/winping.h
		${CMAKE_CURRENT_SOURCE_DIR}/base/winping.cpp
		)
ELSEIF(APPLE)
	SET(PLATFORM_STR_LIST
		${CMAKE_CURRENT_SOURCE_DIR}/base/maccocoathreadhelper.h
		${CMAKE_CURRENT_SOURCE_DIR}/base/maccocoathreadhelper.mm
		${CMAKE_CURRENT_SOURCE_DIR}/base/scoped_autorelease_pool.h
		${CMAKE_CURRENT_SOURCE_DIR}/base/scoped_autorelease_pool.mm
		)
ELSE()
	SET(PLATFORM_STR_LIST
		)
ENDIF()
SET(SRC_LIST 
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncfile.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncfile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncsocket.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncsocket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/base64.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/base64.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/basictypes.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/buffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/bytebuffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/bytebuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/common.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/constructormagic.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/criticalsection.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/helpmethods.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/helpmethods.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/ipaddress.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/ipaddress.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/logging.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/logging.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/noncopyable.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/nethelpers.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/nethelpers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/scoped_ptr.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/scoped_ref_ptr.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/sha1.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/sha1.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/shared_ptr.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/signalthread.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/signalthread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/sigslot.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socket.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketaddress.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketaddress.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketfactory.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketserver.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringencode.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringencode.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringutils.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringutils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/timeutils.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/timeutils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/vzfile.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/vzfile.cpp
	${PLATFORM_STR_LIST}
	
	# event
	${CMAKE_CURRENT_SOURCE_DIR}/event/thread.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/thread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagequeue.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagequeue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagehandler.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagehandler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/event/physicalsocketserver.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/physicalsocketserver.cpp
	
	
	${CMAKE_CURRENT_SOURCE_DIR}/json/json.h
	${CMAKE_CURRENT_SOURCE_DIR}/json/jsoncpp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/json/json-forwards.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkbase.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/internalmessage.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/internalmessage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/queuelayer.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/queuelayer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/session.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/session.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/sessionmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/sessionmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/task.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/task.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushhandle.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushhandle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkservice.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkservice.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/commandanalysis.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/commandanalysis.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzconnectdev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzconnectdev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vziodev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vziodev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmaintendev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmaintendev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmodulebase.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmodulebase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzrecognition.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzrecognition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkbase.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkdefines.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzserialdev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzserialdev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vztcpdevicemanage.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vztcpdevicemanage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzwlistdev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzwlistdev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzlprtcpsdkdefine.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzclientsdk_lpdefine.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzlprtcpsdk.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzlprtcpsdk.cpp
	)

SOURCE_GROUP(base FILES
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncfile.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncfile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncsocket.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/asyncsocket.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/base64.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/base64.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/basictypes.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/buffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/bytebuffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/bytebuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/common.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/constructormagic.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/criticalsection.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/helpmethods.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/helpmethods.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/ipaddress.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/ipaddress.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/logging.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/logging.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/noncopyable.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/nethelpers.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/nethelpers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/scoped_ptr.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/scoped_ref_ptr.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/sha1.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/sha1.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/shared_ptr.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/signalthread.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/signalthread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/sigslot.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socket.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketaddress.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketaddress.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketfactory.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/socketserver.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stream.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stream.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringencode.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringencode.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringutils.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/stringutils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/timeutils.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/timeutils.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/base/vzfile.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/vzfile.cpp
	${PLATFORM_STR_LIST}
	)

SOURCE_GROUP(event FILES
	${CMAKE_CURRENT_SOURCE_DIR}/event/thread.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/thread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagequeue.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagequeue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagehandler.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/messagehandler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/event/physicalsocketserver.h
	${CMAKE_CURRENT_SOURCE_DIR}/event/physicalsocketserver.cpp
	)

SOURCE_GROUP(json FILES
	${CMAKE_CURRENT_SOURCE_DIR}/json/json.h
	${CMAKE_CURRENT_SOURCE_DIR}/json/jsoncpp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/json/json-forwards.h
	)

SOURCE_GROUP(vzsdk FILES
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkbase.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/internalmessage.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/internalmessage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/queuelayer.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/queuelayer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/session.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/session.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/sessionmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/sessionmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/task.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/task.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushhandle.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushhandle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkpushmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkservice.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkservice.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/commandanalysis.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/commandanalysis.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzconnectdev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzconnectdev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vziodev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vziodev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmaintendev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmaintendev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmodulebase.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzmodulebase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzrecognition.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzrecognition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkbase.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzsdkdefines.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzserialdev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzserialdev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vztcpdevicemanage.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vztcpdevicemanage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzwlistdev.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzwlistdev.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzlprtcpsdkdefine.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzclientsdk_lpdefine.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzlprtcpsdk.h
	${CMAKE_CURRENT_SOURCE_DIR}/vzsdk/vzlprtcpsdk.cpp	
	)

SET(CONVERALLS_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/base/base64.h
	${CMAKE_CURRENT_SOURCE_DIR}/base/base64.cpp
	${PLATFORM_STR_LIST}
	)

if (COVERALLS)
	coveralls_setup(
        "${CONVERALLS_SRC}"
        ${COVERALLS_UPLOAD}                 
        "${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake")
endif()
#########################################################################
#Step 4 : Add PROJECT define 
MESSAGE(STATUS "Step 4 : Add code source")

#########################################################################
#Step 5 : Add executable or library target
MESSAGE(STATUS "Step 5 : Add executable or library target")
#ADD_executable(${BUILD_PROJECT_NAME} ${SRC_LIST})
ADD_LIBRARY(${BUILD_PROJECT_NAME} STATIC ${SRC_LIST})
ADD_LIBRARY(vztcpsdk_dynamic SHARED ${SRC_LIST})
set_property(TARGET ${BUILD_PROJECT_NAME} PROPERTY FOLDER ${PROJECT_SET_NAME_LIB})
#########################################################################
#Step 6 : link with other library
MESSAGE(STATUS "Step 6 : link with other library")

TARGET_LINK_LIBRARIES(${BUILD_PROJECT_NAME} 
)