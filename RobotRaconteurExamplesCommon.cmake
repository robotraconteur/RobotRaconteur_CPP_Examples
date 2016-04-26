#Some versions of CMAKE don't search for Boost 1.60
SET (Boost_ADDITIONAL_VERSIONS 1.60.0 1.60)
SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_MULTITHREADED ON)
SET (Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS date_time filesystem system regex chrono atomic thread REQUIRED)
add_definitions(-DBOOST_ALL_NO_LIB)

if(CMAKE_COMPILER_IS_GNUCXX)
	find_package(OpenSSL)	
	set (RobotRaconteur_EXTRA_LIBRARIES ${RobotRaconteur_EXTRA_LIBRARIES} ${OPENSSL_LIBRARIES} pthread rt z)	
endif()

if(CMAKE_GENERATOR STREQUAL Xcode)

	set(OPENSSL_USE_STATIC_LIBS TRUE)
	find_package(OpenSSL)	

	include(CMakeFindFrameworks)
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	CMAKE_FIND_FRAMEWORKS(CoreFoundation)
	CMAKE_FIND_FRAMEWORKS(Security)
	include_directories(${CoreFoundation_FRAMEWORKS}/Headers ${Security_FRAMEWORKS}/Headers)
	set (RobotRaconteur_EXTRA_LIBRARIES ${RobotRaconteur_EXTRA_LIBRARIES} ${CoreFoundation_FRAMEWORKS}/CoreFoundation ${Security_FRAMEWORKS}/Security ${OPENSSL_LIBRARIES} c++)
endif()

include_directories(${Boost_INCLUDE_DIRS})

find_package(RobotRaconteur REQUIRED)

