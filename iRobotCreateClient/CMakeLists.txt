cmake_minimum_required(VERSION 2.8.12)
project(iRobotCreateClient)

include(../RobotRaconteurExamplesCommon.cmake)

add_executable(iRobotCreateClient
	experimental__create_stubskel.cpp
	iRobotCreateClient.cpp)

target_link_libraries(iRobotCreateClient RobotRaconteur2 ${Boost_LIBRARIES} ${ROBOTRACONTEUR_EXTRA_LIBS} )

add_executable(FindiRobotCreateServiceNode
	experimental__create_stubskel.cpp
	FindiRobotCreateServiceNode.cpp)

target_link_libraries(FindiRobotCreateServiceNode RobotRaconteur2 ${Boost_LIBRARIES} ${ROBOTRACONTEUR_EXTRA_LIBS} )
