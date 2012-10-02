remove_definitions( -DVD_BUILDING_FRAMEWORK )

PROJECT( vdbm )

set( VD_TRIAL_TEST_NAME 				vdbm )
set( VD_TRIAL_TEST_DIR 					${VD_TRIAL_TESTS_DIR}/voldemort )

execute_process(COMMAND 				module load voldemort-cpp/latest)
set( VD_EXT_VOLDEMORT_CPP_DIR			$ENV{BLDR_VOLDEMORT_CPP_BASE_PATH} )
file( GLOB VD_EXT_VOLDEMORT_CPP_LIBS	$ENV{BLDR_VOLDEMORT_CPP_LIB_PATH}/*.${VD_EXT_LIB_EXT}  )
include_directories(					$ENV{BLDR_VOLDEMORT_CPP_INCLUDE_PATH} )

execute_process(COMMAND 				module load protobuf/latest)
file( GLOB VD_EXT_PROTOBUF_LIBS			$ENV{BLDR_PROTOBUF_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
include_directories(					$ENV{BLDR_PROTOBUF_INCLUDE_PATH} )

execute_process(COMMAND 				module load boost/latest)
file( GLOB VD_EXT_BOOST_LIBS			$ENV{BLDR_BOOST_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
include_directories(					$ENV{BLDR_BOOST_INCLUDE_PATH} )

execute_process(COMMAND 				module load expat/latest)
file( GLOB VD_EXT_EXPAT_LIBS			$ENV{BLDR_EXPAT_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
include_directories(					$ENV{BLDR_EXPAT_INCLUDE_PATH} )

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
	${VD_EXT_VOLDEMORT_CPP_LIBS}
	${VD_EXT_PROTOBUF_LIBS}
	${VD_EXT_BOOST_LIBS}
	${VD_EXT_EXPAT_LIBS}
	${VD_PLATFORM_LIBS}
)

message(STATUS "Configured ${VD_TRIAL_TEST_NAME} Test ...")

