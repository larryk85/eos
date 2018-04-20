file(GLOB RT_SOURCES "*.c")

add_wast_library(TARGET rt
   #NOWARNINGS
  SOURCE_FILES ${RT_SOURCES}
  INCLUDE_FOLDERS ${CMAKE_SOURCE_DIR}/contracts/musl/upstream/include
                  ${CMAKE_SOURCE_DIR}/contracts/
  DESTINATION_FOLDER ${CMAKE_CURRENT_BINARY_DIR}
)
