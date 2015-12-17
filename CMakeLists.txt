PROJECT(KDESUDO)

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckSymbolExists)

check_include_files("sys/prctl.h" HAVE_SYS_PRCTL_H)
check_symbol_exists(PR_SET_DUMPABLE "sys/prctl.h" HAVE_PR_SET_DUMPABLE)
add_feature_info("prctl-dumpable" HAVE_PR_SET_DUMPABLE "Used to disallow ptracing")

include_directories(${KDE4_INCLUDES})

add_definitions(${KDE4_ENABLE_EXCEPTIONS} ${KDE4_DEFINITIONS})

ADD_SUBDIRECTORY(kdesudo)
ADD_SUBDIRECTORY(doc)

find_package(Gettext REQUIRED)
add_subdirectory( po )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
