######################################################################################
# Fakenect Mock Library
######################################################################################
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/fakenect)
IF(MSVC)
add_library (fakenect SHARED fakenect.c wintime.h)
ELSE(MSVC)
add_library (fakenect SHARED fakenect.c)
ENDIF(MSVC)
set_target_properties ( fakenect PROPERTIES
  VERSION ${PROJECT_VER}
  SOVERSION ${PROJECT_APIVER}
  OUTPUT_NAME freenect)
target_link_libraries(fakenect ${MATH_LIB})

install (TARGETS fakenect
  DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}/fakenect")


  
IF(MSVC)
option(BUILD_RECORD "Build Record Program" off)
if(BUILD_RECORD)
add_executable(record record.c wintime.h)
endif(BUILD_RECORD)
ELSE(MSVC)
option(BUILD_RECORD "Build Record Program" on)
if(BUILD_RECORD)
add_executable(record record.c)
endif(BUILD_RECORD)
ENDIF(MSVC)

if(BUILD_RECORD)

install (TARGETS record
  DESTINATION bin)

endif(BUILD_RECORD)
  
CONFIGURE_FILE("fakenect.sh.in"
  "fakenect.sh"
  IMMEDIATE @ONLY)
install (PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/fakenect.sh
  DESTINATION bin
  RENAME fakenect)

