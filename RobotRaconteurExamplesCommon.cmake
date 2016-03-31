message(${MSVC_VERSION})

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

#set(RobotRaconteur_INCLUDE_DIR "" CACHE PATH "Robot Raconteur include path")
#set(RobotRaconteur_LIB_DIR "" CACHE PATH "Robot Raconteur lib path")

include(FindPackageHandleStandardArgs)

if (NOT ${Boost_MAJOR_VERSION} STREQUAL 1 OR NOT ${Boost_MINOR_VERSION} STREQUAL 60)
message(FATAL_ERROR "Invalid Boost Version")
endif()

find_path(RobotRaconteur_INCLUDE_DIR RobotRaconteur.h HINT "${RobotRaconteur_ROOT}/include" "${RobotRaconteur_ROOT}/out/include")
find_library(RobotRaconteur_LIBRARY_DEBUG NAMES RobotRaconteur2.lib libRobotRaconteur2.a PATHS "${RobotRaconteur_ROOT}/out_debug/lib" "${RobotRaconteur_ROOT}/lib/Debug")
find_library(RobotRaconteur_LIBRARY_RELEASE NAMES RobotRaconteur2.lib libRobotRaconteur2.a PATHS "${RobotRaconteur_ROOT}/out/lib" "${RobotRaconteur_ROOT}/lib/Release")

if (RobotRaconteur_LIBRARY_DEBUG OR RobotRaconteur_LIBRARY_RELEASE)
	#Using Debug/Release library
	find_package_handle_standard_args(RobotRaconteur DEFAULT_MSG RobotRaconteur_LIBRARY_DEBUG RobotRaconteur_LIBRARY_RELEASE RobotRaconteur_INCLUDE_DIR Boost_DATE_TIME_FOUND Boost_FILESYSTEM_FOUND Boost_SYSTEM_FOUND Boost_REGEX_FOUND Boost_CHRONO_FOUND Boost_ATOMIC_FOUND Boost_THREAD_FOUND)
	set(RobotRaconteur_LIBRARY optimized ${RobotRaconteur_LIBRARY_RELEASE} debug ${RobotRaconteur_LIBRARY_DEBUG})
else()
	#Using single library
	find_library(RobotRaconteur_LIBRARY NAMES RobotRaconteur2.lib libRobotRaconteur2.a PATHS "${RobotRaconteur_ROOT}/out/lib" "${RobotRaconteur_ROOT}/lib/Release")
	find_package_handle_standard_args(RobotRaconteur DEFAULT_MSG RobotRaconteur_LIBRARY RobotRaconteur_INCLUDE_DIR Boost_DATE_TIME_FOUND Boost_FILESYSTEM_FOUND Boost_SYSTEM_FOUND Boost_REGEX_FOUND Boost_CHRONO_FOUND Boost_ATOMIC_FOUND Boost_THREAD_FOUND)
endif()




include_directories(${Boost_INCLUDE_DIRS} ${RobotRaconteur_INCLUDE_DIR})