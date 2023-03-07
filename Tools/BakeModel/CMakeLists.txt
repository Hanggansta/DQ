add_executable(BakeModel "BakeModel.cpp" "JSON.cpp" "Model.cpp")
target_link_libraries(BakeModel PRIVATE cgltf cxxopts rapidjson stb)

set_target_properties(BakeModel PROPERTIES FOLDER "Tools")