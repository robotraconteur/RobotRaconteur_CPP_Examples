INCLUDE(FindBoost)
#Some versions of CMAKE don't search for Boost 1.60
SET (Boost_ADDITIONAL_VERSIONS 1.60.0 1.60)
SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREADED ON)
SET (Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost COMPONENTS date_time filesystem system regex chrono atomic thread REQUIRED)
add_definitions(-DBOOST_ALL_NO_LIB)

if(WIN32)
	add_definitions( -D_WIN32_WINNT=0x0501 )
	set (ROBOTRACONTEUR_EXTRA_LIBS ${ROBOTRACONTEUR_EXTRA_LIBS} Mswsock.lib ws2_32.lib IPHLPAPI.lib Crypt32.lib Secur32.lib )
endif()

if(CMAKE_COMPILER_IS_GNUCXX)	
	set (ROBOTRACONTEUR_EXTRA_LIBS ${ROBOTRACONTEUR_EXTRA_LIBS} ssl crypto pthread rt z)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -w")	
endif()

if(CMAKE_GENERATOR STREQUAL Xcode)
	include(CMakeFindFrameworks)
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	CMAKE_FIND_FRAMEWORKS(CoreFoundation)
	CMAKE_FIND_FRAMEWORKS(Security)
	include_directories(${CoreFoundation_FRAMEWORKS}/Headers ${Security_FRAMEWORKS}/Headers)
	set (ROBOTRACONTEUR_EXTRA_LIBS ${ROBOTRACONTEUR_EXTRA_LIBS} ${CoreFoundation_FRAMEWORKS}/CoreFoundation ${Security_FRAMEWORKS}/Security ssl crypto c++)
endif()

set(RobotRaconteur_INCLUDE_DIR "" CACHE PATH "Robot Raconteur include path")
set(RobotRaconteur_LIB_DIR "" CACHE PATH "Robot Raconteur lib path")

link_directories(${RobotRaconteur_LIB_DIR})
include_directories(${Boost_INCLUDE_DIRS} ${RobotRaconteur_INCLUDE_DIR})