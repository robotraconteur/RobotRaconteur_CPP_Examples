cmake_minimum_required(VERSION 2.8.12)
project(SimpleWebcamClient)

include(../RobotRaconteurExamplesCommon.cmake)

find_package(OpenCV REQUIRED)

add_executable(SimpleWebcamClient
	experimental__createwebcam_stubskel.cpp
	SimpleWebcamClient.cpp)

target_link_libraries(SimpleWebcamClient ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteur_EXTRA_LIBS} )

add_executable(SimpleWebcamClient_streaming
	experimental__createwebcam_stubskel.cpp
	SimpleWebcamClient_streaming.cpp)

target_link_libraries(SimpleWebcamClient_streaming ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteur_EXTRA_LIBS} )

add_executable(SimpleWebcamClient_memory
	experimental__createwebcam_stubskel.cpp
	SimpleWebcamClient_memory.cpp)

target_link_libraries(SimpleWebcamClient_memory ${RobotRaconteur_LIBRARY} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteur_EXTRA_LIBS} )
