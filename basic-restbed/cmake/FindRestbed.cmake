include(FindPackageHandleStandardArgs)

find_path(Restbed_INCLUDE_DIR NAMES restbed)
find_library(Restbed_LIBRARY NAMES restbed)

find_package_handle_standard_args(Restbed REQUIRED_VARS Restbed_LIBRARY Restbed_INCLUDE_DIR)

if(Restbed_FOUND)
    set(Restbed_LIBS ${Restbed_LIBRARY})
    set(Restbed_INCLUDE_DIRS ${Restbed_INCLUDE_DIR})
endif()
