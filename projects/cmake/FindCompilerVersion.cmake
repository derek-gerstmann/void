FUNCTION(VD_COMPILER_DUMP_VERSION OUTPUT_VERSION)
  EXECUTE_PROCESS(COMMAND
    ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
    OUTPUT_VARIABLE VD_COMPILER_VERSION
  )
  STRING(REGEX REPLACE "([0-9])\\.([0-9])(\\.[0-9])?" "\\1\\2"
    VD_COMPILER_VERSION ${VD_COMPILER_VERSION})

  SET(${OUTPUT_VERSION} ${VD_COMPILER_VERSION} PARENT_SCOPE)
ENDFUNCTION()
