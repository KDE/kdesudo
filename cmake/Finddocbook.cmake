FIND_PROGRAM( KDE3_MEINPROC_EXECUTABLE NAME meinproc PATHS ${KDE3PRFIX}/bin )

IF( KDE3_MEINPROC_EXECUTABLE )
    SET( DOCBOOK_FOUND TRUE )
ELSE( KDE3_MEINPROC_EXECUTABLE )
    MESSAGE( STATUS "Failed to find meinproc" )
ENDIF( KDE3_MEINPROC_EXECUTABLE )

IF( DOCBOOK_FOUND )
   IF( NOT docbook_FIND_QUIETLY )
      MESSAGE( STATUS "Found meinproc: ${KDE3_MEINPROC_EXECUTABLE}" )
   ENDIF( NOT docbook_FIND_QUIETLY )
ELSE( DOCBOOK_FOUND )
   IF( docbook_FIND_REQUIRED )
      MESSAGE( FATAL_ERROR "Could not find meinproc" )
   ENDIF( docbook_FIND_REQUIRED )
ENDIF( DOCBOOK_FOUND )

#This macro was taken from KDE4Macros.cmake and adjusted slightly
macro (KDE3_CREATE_HANDBOOK _docbook)
    get_filename_component(_input ${_docbook} ABSOLUTE)
    set(_doc ${CMAKE_CURRENT_BINARY_DIR}/index.cache.bz2)

    file(GLOB _docs *.docbook)
    add_custom_command(OUTPUT ${_doc}
        COMMAND ${KDE3_MEINPROC_EXECUTABLE} --cache ${_doc} ${_input}
        DEPENDS ${_docs}
    )
    add_custom_target(handbook ALL DEPENDS ${_doc})

    set(_htmlDoc ${CMAKE_CURRENT_SOURCE_DIR}/index.html)
    add_custom_command(OUTPUT ${_htmlDoc}
        COMMAND ${KDE3_MEINPROC_EXECUTABLE} -o ${_htmlDoc} ${_input}
        DEPENDS ${_input}
        )
    add_custom_target(htmlhandbook DEPENDS ${_htmlDoc})

    set(_args ${ARGN})

    set(_installDest)
    if(_args)
        list(GET _args 0 _tmp)
        if("${_tmp}" STREQUAL "INSTALL_DESTINATION")
            list(GET _args 1 _installDest )
            list(REMOVE_AT _args 0 1)
        endif("${_tmp}" STREQUAL "INSTALL_DESTINATION")
    endif(_args)

    if(_args)
        list(GET _args 0 _tmp)
        if("${_tmp}" STREQUAL "SUBDIR")
            list(GET _args 1 dirname )
            list(REMOVE_AT _args 0 1)
        endif("${_tmp}" STREQUAL "SUBDIR")
    endif(_args)

    if(_installDest)
        file(GLOB _images *.png)
        install(FILES ${_doc} ${_docs} ${_images} DESTINATION ${_installDest} )
    endif(_installDest)

endmacro (KDE3_CREATE_HANDBOOK)
