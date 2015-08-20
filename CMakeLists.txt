PROJECT(KDESUDO)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(CheckIncludeFiles)
include(CheckFunctionExists)

include_directories(${KDE4_INCLUDES})

add_definitions(${KDE4_ENABLE_EXCEPTIONS} ${KDE4_DEFINITIONS})

ADD_SUBDIRECTORY(kdesudo)
ADD_SUBDIRECTORY(doc)

find_package(Gettext REQUIRED)
add_subdirectory( po )
