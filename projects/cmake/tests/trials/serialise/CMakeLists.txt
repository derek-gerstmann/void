cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project( vd_serialise_trial )

set( VD_TRIAL_TEST_NAME 	vd_serialise_trial )
set( VD_TRIAL_TEST_DIR 		${VD_TRIAL_TESTS_DIR}/serialise )

file( GLOB VD_TRIAL_TEST_SRC  	
	${VD_TRIAL_TEST_DIR}/*.cpp)

file( GLOB VD_TRIAL_TEST_INC  	
	${VD_TRIAL_TEST_DIR}/*.h 
	${VD_TRIAL_TEST_DIR}/*.hpp)

include_directories(
	${VD_TRIAL_TEST_DIR}
)

add_executable( 
	${VD_TRIAL_TEST_NAME}
	${VD_TRIAL_TEST_SRC}
)

target_link_libraries( 
	${VD_TRIAL_TEST_NAME}  
	${VD_PLATFORM_LIBS}
)

message(STATUS "Configured ${VD_TRIAL_TEST_NAME} Test ...")
