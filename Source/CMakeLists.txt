set(GraphicsFile "Graphics/Device.cpp" "Graphics/DX12Texture2D.cpp")
set(RendererFile "Renderer/Renderer.cpp")
set(SceneFile "Scene/Scene.cpp")
set(TextureFile "Texture/Texture2D.cpp")
set(UtilityFile "Utility/Window.cpp")

source_group(Graphics FILES ${GraphicsFile})
source_group(Renderer FILES ${RendererFile})
source_group(Scene FILES ${SceneFile})
source_group(Texture FILES ${TextureFile})
source_group(Utility FILES ${UtilityFile})

add_library(DQ STATIC
    ${GraphicsFile}
    ${RendererFile}
    ${SceneFile}
    ${TextureFile}
    ${UtilityFile}
)

target_include_directories(DQ PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_link_libraries(DQ PUBLIC d3d12.lib dxgi.lib dxguid.lib)
target_link_libraries(DQ PRIVATE DirectX-Headers rapidjson stb)

set_target_properties(DQ PROPERTIES FOLDER "DQ")