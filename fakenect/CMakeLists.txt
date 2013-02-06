######################################################################################
# Fakenect Mock Library
######################################################################################
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
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
target_link_libraries(fakenect ${MATH_LIB})

install (TARGETS fakenect
  DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}/fakenect")

IF(WIN32)
add_executable(record record.c wintime.h)
ELSE(WIN32)
add_executable(record record.c)
ENDIF(WIN32)

install (TARGETS record
  DESTINATION bin)

CONFIGURE_FILE("fakenect.sh.in"
  "fakenect.sh"
  IMMEDIATE @ONLY)
install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fakenect.sh
  DESTINATION bin
  RENAME fakenect)

