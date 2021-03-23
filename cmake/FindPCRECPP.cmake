
# - Find PCRECPP
# Find the native PCRECPP headers and libraries.
#
#  PCRECPP_INCLUDE_DIRS - where to find pcre.h, etc.
#  PCRECPP_LIBRARIES    - List of libraries when using pcrecpp
#  PCRECPP_FOUND        - True if pcrecpp found.

find_path(PCRECPP_INCLUDE_DIR NAMES pcrecpp.h)
mark_as_advanced(PCRECPP_INCLUDE_DIR)

find_library(PCRECPP_LIBRARY NAMES libpcrecpp.a)
mark_as_advanced(PCRECPP_LIBRARY)

find_library(PCRE_LIBRARY NAMES libpcre.a)
mark_as_advanced(PCRE_LIBRARY)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCRECPP DEFAULT_MSG PCRECPP_LIBRARY PCRECPP_INCLUDE_DIR PCRE_LIBRARY)

if(PCRECPP_FOUND)
  set(PCRECPP_LIBRARIES ${PCRECPP_LIBRARY} ${PCRE_LIBRARY})
  set(PCRECPP_INCLUDE_DIRS ${PCRECPP_INCLUDE_DIR})
  include_directories(${PCRECPP_INCLUDE_DIRS})
  set(EXT_LIBS ${EXT_LIBS} ${PCRECPP_LIBRARIES})
else(PCRECPP_FOUND)
  message(SEND_ERROR "Cannot find pcrecpp libraries. Terminating.")
endif(PCRECPP_FOUND)