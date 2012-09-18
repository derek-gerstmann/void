remove_definitions( -DVD_BUILDING_FRAMEWORK )

PROJECT( vdbm )

set( VD_TEST_NAME 				vdbm )
set( VD_TEST_DIR 				${VD_TESTS_DIR}/voldemort )

set( VD_EXT_VD_CPP_DIR			$ENV{BLDR_VOLDEMORT_CPP_BASE_PATH} )
file( GLOB VD_EXT_VD_CPP_LIBS		$ENV{BLDR_VOLDEMORT_CPP_LIB_PATH}/*.${VD_EXT_LIB_EXT}  )
file( GLOB VD_EXT_PB_LIBS		$ENV{BLDR_PROTOBUF_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
file( GLOB VD_EXT_BOOST_LIBS		$ENV{BLDR_BOOST_LIB_PATH}/*.${VD_EXT_LIB_EXT} )
file( GLOB VD_EXT_EXPAT_LIBS		$ENV{BLDR_EXPAT_LIB_PATH}/*.${VD_EXT_LIB_EXT} )

include_directories(			$ENV{BLDR_VOLDEMORT_CPP_INCLUDE_PATH} )

file( GLOB VD_TEST_SRC  	
	${VD_TEST_DIR}/*.cpp)

file( GLOB VD_TEST_INC  	
	${VD_TEST_DIR}/*.h 
	${VD_TEST_DIR}/*.hpp)

include_directories(
	${VD_TEST_DIR}
)

add_executable( 
	${VD_TEST_NAME}
	${VD_TEST_SRC}
)

target_link_libraries( 
	${VD_TEST_NAME}  
	${VD_EXT_VD_CPP_LIBS}
	${VD_EXT_PB_LIBS}
	${VD_EXT_BOOST_LIBS}
	${VD_EXT_EXPAT_LIBS}
	${VD_PLATFORM_LIBS}
)

message(STATUS "Configured ${VD_TEST_NAME} Test ...")

