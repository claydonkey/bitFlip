# findShlwapi.cmake
# - Try to find shlwapi
#
# The following variables are optionally searched for defaults
#  shlwapi_ROOT_DIR:  Base directory where all shlwapi components are found
#
# Once done this will define
#  shlwapi_FOUND - System has shlwapi
#  shlwapi_LIBRARIES - The libraries needed to use shlwapi

set(shlwapi_ROOT_DIR "" CACHE PATH "Folder containing shlwapi")

find_library(shlwapi_LIBRARY NAMES "shlwapi"
        PATHS ${shlwapi_ROOT_DIR}
        PATH_SUFFIXES lib lib64
        NO_DEFAULT_PATH REQUIRED)
find_library(shlwapi_LIBRARY NAMES "shlwapi")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set benchmark_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(shlwapi FOUND_VAR shlwapi_FOUND
        REQUIRED_VARS shlwapi_LIBRARY        )

IF(shlwapi_FOUND)
    set(shlwapi_LIBRARIES ${shlwapi_LIBRARY})
ENDIF(shlwapi_FOUND)

mark_as_advanced(shlwapi_LIBRARY)