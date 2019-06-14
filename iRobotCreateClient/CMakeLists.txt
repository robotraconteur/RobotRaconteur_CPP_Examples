cmake_minimum_required(VERSION 3.3)
project(iRobotCreateClient)

set (CMAKE_CXX_STANDARD 11)

find_package(RobotRaconteur REQUIRED)

include_directories(${RobotRaconteur_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ROBOTRACONTEUR_GENERATE_THUNK(RR_THUNK_SRCS RR_THUNK_HDRS 
	experimental.create2.robdef
	MASTER_HEADER
	IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../robdef
	)


add_executable(iRobotCreateClient
	${RR_THUNK_HDRS}
	${RR_THUNK_SRCS}
	iRobotCreateClient.cpp)

target_link_libraries(iRobotCreateClient RobotRaconteurCore ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteurCore_Extra_Libraries} )

add_executable(FindiRobotCreateServiceNode
	${RR_THUNK_HDRS}
	${RR_THUNK_SRCS}
	FindiRobotCreateServiceNode.cpp)

target_link_libraries(FindiRobotCreateServiceNode RobotRaconteurCore ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteurCore_Extra_Libraries} )
