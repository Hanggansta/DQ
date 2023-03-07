if (NOT TARGET cgltf)
    add_library(cgltf INTERFACE)
    target_include_directories(cgltf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cgltf)
endif()

if (NOT TARGET cxxopts)
    add_library(cxxopts INTERFACE)
    target_include_directories(cxxopts INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts)
endif()

if (NOT TARGET DirectX-Headers)
    add_library(DirectX-Headers INTERFACE)
    target_include_directories(DirectX-Headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/DirectX-Headers)
endif()

if (NOT TARGET rapidjson)
    add_library(rapidjson INTERFACE)
    target_include_directories(rapidjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)
endif()

if (NOT TARGET stb)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)
endif()