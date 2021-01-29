file(GLOB LANGRULES_SOURCES "langrules/*pp")

add_library(pisah STATIC pisah.cpp processor.cpp utils.cpp ${LANGRULES_SOURCES})
target_link_libraries(pisah ${EXT_LIBS})
set_target_properties(pisah PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(pisah PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

if (STANDALONE_COMPILE)
  add_executable(pisah-bin main.cpp)
  target_link_libraries(pisah-bin pisah)
  set_target_properties(pisah-bin PROPERTIES OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(pisah-bin PROPERTIES OUTPUT_NAME "${CMAKE_BINARY_DIR}/pisah")
endif(STANDALONE_COMPILE)
