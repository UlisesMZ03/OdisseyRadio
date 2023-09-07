# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/OdisseyRadio_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/OdisseyRadio_autogen.dir/ParseCache.txt"
  "OdisseyRadio_autogen"
  )
endif()
