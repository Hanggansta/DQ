add_executable(DrawModel WIN32 DrawModel.cpp)
target_link_libraries(DrawModel PRIVATE DQ)

set_target_properties(DrawModel PROPERTIES FOLDER "Examples")
set_target_properties(DrawModel PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(TARGET DrawModel POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/Model"
     "$<TARGET_FILE_DIR:DrawModel>/Model" 
  COMMENT "Copying to output directory"
)