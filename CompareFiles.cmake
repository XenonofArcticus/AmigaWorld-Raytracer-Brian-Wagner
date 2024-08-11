# CompareFiles.cmake
if(NOT EXISTS "${GENERATED_FILE}")
  message(FATAL_ERROR "Generated file ${GENERATED_FILE} does not exist.")
endif()

if(NOT EXISTS "${REFERENCE_FILE}")
  message(FATAL_ERROR "Reference file ${REFERENCE_FILE} does not exist.")
endif()

# Use CMake's compare_files command to compare binary contents
file(COMPARE "${GENERATED_FILE}" "${REFERENCE_FILE}" RESULT)

if(NOT RESULT EQUAL 0)
  message(FATAL_ERROR "Files ${GENERATED_FILE} and ${REFERENCE_FILE} differ.")
endif()
