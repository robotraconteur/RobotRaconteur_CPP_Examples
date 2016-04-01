cmake_minimum_required(VERSION 2.8.12)
project(iRobotCreateService)

include(../RobotRaconteurExamplesCommon.cmake)

add_executable(iRobotCreateService
	experimental__create_stubskel.cpp
	iRobotCreateService.cpp)

target_link_libraries(iRobotCreateService ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${RobotRaconteur_EXTRA_LIBS} )

add_executable(iRobotCreateService_broadcaster
	experimental__create_stubskel.cpp
	iRobotCreateService_broadcaster.cpp)

target_link_libraries(iRobotCreateService_broadcaster ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${RobotRaconteur_EXTRA_LIBS} )
