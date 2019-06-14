cmake_minimum_required(VERSION 3.3)
project(iRobotCreateService)

set (CMAKE_CXX_STANDARD 11)

find_package(RobotRaconteur REQUIRED)

include_directories(${RobotRaconteur_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ROBOTRACONTEUR_GENERATE_THUNK(RR_THUNK_SRCS RR_THUNK_HDRS 
	experimental.create2.robdef
	MASTER_HEADER
	IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../robdef
	)

add_executable(iRobotCreateService
	${RR_THUNK_HDRS}
	${RR_THUNK_SRCS}
	iRobotCreateService.h
	iRobotCreateService.cpp)

target_link_libraries(iRobotCreateService RobotRaconteurCore ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteurCore_Extra_Libraries} )
