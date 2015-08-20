MACRO (GENBOOK _language_code)
    kde4_create_handbook(index.docbook
        INSTALL_DESTINATION ${HTML_INSTALL_DIR}/${_language_code} SUBDIR kdesudo)
    ADD_DEPENDENCIES(doc ${_language_code}-handbook)

ENDMACRO (GENBOOK)

ADD_CUSTOM_TARGET(doc ALL)

INSTALL(SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/makelink.cmake)

ADD_SUBDIRECTORY(man)

