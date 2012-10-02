remove_definitions( -DVD_BUILDING_FRAMEWORK )

file( GLOB VD_TEST_SRC  	
	${VD_TEST_DIR}/*.c 
	${VD_TEST_DIR}/*.m 
	${VD_TEST_DIR}/*.mm 
	${VD_TEST_DIR}/*.cpp)

file( GLOB VD_TEST_INC  	
	${VD_TEST_DIR}/*.h 
	${VD_TEST_DIR}/*.hpp)

include_directories(
	${VD_TEST_DIR}
)

add_executable( ${VD_TEST_NAME}
	${VD_TEST_SRC}
)

target_link_libraries( ${VD_TEST_NAME}  
	VdPlatform
)

message(STATUS "Configured ${VD_TEST_NAME} Test ...")
