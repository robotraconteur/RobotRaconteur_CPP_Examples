cmake_minimum_required(VERSION 2.8.12)
project(SimpleWebcamService)

include(../RobotRaconteurExamplesCommon.cmake)

find_package(OpenCV REQUIRED)

add_executable(SimpleWebcamService
	experimental__createwebcam_stubskel.cpp
	SimpleWebcamService.cpp)

target_link_libraries(SimpleWebcamService ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteur_EXTRA_LIBRARIES} )

add_executable(SimpleWebcamService_broadcaster
	experimental__createwebcam_stubskel.cpp
	SimpleWebcamService_broadcaster.cpp)

target_link_libraries(SimpleWebcamService_broadcaster ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteur_EXTRA_LIBRARIES} )
