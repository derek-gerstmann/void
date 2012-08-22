
function(extract_symbols output included scope namespace)
    foreach(VALUE ${ARGN})
        if ("${VALUE}" STREQUAL "${ARGV1}")
            set(filelist ${VALUE})
        else()
            set(filelist ${filelist} ${VALUE})
        endif()
    endforeach(VALUE)

    add_custom_command ( 
        OUTPUT ${output}
        COMMENT "Extracting Static Symbols and Strings to ${output} ..."
        COMMAND vdsymex ${filelist} -i "vd.h" -i "constants/constants.h" -w "${included}" -u "${scope}" -n Symbols -n ${namespace} -o ${output}
        DEPENDS ${filelist} ${VD_PLATFORM_SRC} 
    )

    add_definitions( -DVD_USE_EXTRACTED_SYMBOLS=1 )

endfunction(extract_symbols) 




