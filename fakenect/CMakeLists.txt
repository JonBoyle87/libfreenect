######################################################################################
# Fakenect Mock Library
######################################################################################
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/utils)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/fakenect)
IF(WIN32)
add_library (fakenect SHARED fakenect.c wintime.h)
ELSE(WIN32)
add_library (fakenect SHARED fakenect.c)
ENDIF(WIN32)
set_target_properties ( fakenect PROPERTIES
  VERSION ${PROJECT_VER}
  SOVERSION ${PROJECT_APIVER}
  OUTPUT_NAME freenect)

install (TARGETS fakenect
  DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}/fakenect")

IF(WIN32)
add_executable(record record.c wintime.h)
ELSE(WIN32)
add_executable(record record.c)
ENDIF(WIN32)
target_link_libraries(record freenect)
