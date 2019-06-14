cmake_minimum_required(VERSION 3.3)
project(SimpleWebcamClient)

set (CMAKE_CXX_STANDARD 11)

find_package(RobotRaconteur REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${RobotRaconteur_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

ROBOTRACONTEUR_GENERATE_THUNK(RR_THUNK_SRCS RR_THUNK_HDRS 
	experimental.createwebcam2.robdef
	MASTER_HEADER
	IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../robdef
	)

add_executable(SimpleWebcamClient
	${RR_THUNK_HDRS}
	${RR_THUNK_SRCS}
	SimpleWebcamClient.cpp)

target_link_libraries(SimpleWebcamClient RobotRaconteurCore ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteurCore_Extra_Libraries} )

add_executable(SimpleWebcamClient_streaming
	${RR_THUNK_HDRS}
	${RR_THUNK_SRCS}
	SimpleWebcamClient_streaming.cpp)

target_link_libraries(SimpleWebcamClient_streaming RobotRaconteurCore ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteurCore_Extra_Libraries} )

add_executable(SimpleWebcamClient_memory
	${RR_THUNK_HDRS}
	${RR_THUNK_SRCS}
	SimpleWebcamClient_memory.cpp)

target_link_libraries(SimpleWebcamClient_memory RobotRaconteurCore ${Boost_LIBRARIES} ${OpenCV_LIBS} ${RobotRaconteurCore_Extra_Libraries} )
