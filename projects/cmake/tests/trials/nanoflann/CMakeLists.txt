remove_definitions( -DVD_BUILDING_FRAMEWORK )

PROJECT( vd_nanoflann_trial )

set( VD_TRIAL_TEST_NAME 		vd_nanoflann_trial )
set( VD_TRIAL_TEST_DIR 			${VD_TRIAL_TESTS_DIR}/nanoflann )

execute_process(COMMAND 		module load nanoflann/latest)
set( VD_EXT_FLANN_DIR			$ENV{BLDR_NANOFLANN_BASE_PATH} )
include_directories(			$ENV{BLDR_FLANN_INCLUDE_PATH} )

execute_process(COMMAND 		module load eigen/latest)
set( VD_EXT_EIGEN_PATH			$ENV{BLDR_EIGEN_BASE_PATH} )
include_directories(			$ENV{BLDR_EIGEN_INCLUDE_PATH} )

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
