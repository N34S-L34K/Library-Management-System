# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\GUIDSA_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\GUIDSA_autogen.dir\\ParseCache.txt"
  "GUIDSA_autogen"
  )
endif()
