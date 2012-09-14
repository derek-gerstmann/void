remove_definitions( -DVD_BUILDING_FRAMEWORK )

PROJECT( vdbm )

set( VD_TEST_NAME vdbm )
set( VD_TEST_DIR ${VD_TESTS_DIR}/voldemort )

set( VD_EXT_VD_DIR			${BLDR_VOLDEMORT_CPP_BASE_PATH} )
file( GLOB VD_EXT_VD_LIB	${VD_EXT_VD_DIR}/lib/*.${VD_EXT_LIB_EXT} )
include_directories(		${VD_EXT_VD_DIR}/include )

file( GLOB VD_TEST_SRC  	
	${VD_TEST_DIR}/*.cpp)

file( GLOB VD_TEST_INC  	
	${VD_TEST_DIR}/*.h 
	${VD_TEST_DIR}/*.hpp)

include_directories(
	${VD_TEST_DIR}
	${VD_EXT_GTEST_DIR}/include
	${BLDR_VOLDEMORT_CPP_INCLUDE_PATH}
)

add_executable( ${VD_TEST_NAME}
	${VD_TEST_SRC}
)

target_link_libraries( ${VD_TEST_NAME}  
	${VD_EXT_VD_LIB} 
	${VD_EXT_GTEST_LIB}
)

message(STATUS "Configured ${VD_TEST_NAME} Test ...")
