#include "BakeModel.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool PreCheckModel(const std::string& filePath, std::string& reason)
{
    cgltf_options options{};
    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, filePath.c_str(), &data);
    if (result != cgltf_result_success)
    {
        reason = "Unknown reason";
        return false;
    }

    if (data->meshes_count == 0)
    {
        cgltf_free(data);
        reason = "Can not find mesh";
        return false;
    }

    for (cgltf_size i = 0; i < data->meshes_count; ++i)
    {
        auto& gltfmesh = data->meshes[i];
        auto primitiveCount = gltfmesh.primitives_count;
        for (cgltf_size j = 0; j < primitiveCount; ++j)
        {
            if (gltfmesh.primitives[j].type != cgltf_primitive_type_triangles)
            {
                cgltf_free(data);
                reason = "Primitive type is not triangle";
                return false;
            }
        }
    }

    for (cgltf_size i = 0; i < data->materials_count; ++i)
    {
        if (!data->materials[i].has_pbr_metallic_roughness)
        {
            cgltf_free(data);
            reason = "Material type is not supported";
            return false;
        }
    }

    cgltf_free(data);
    return true;
}

DirectX::XMFLOAT3 GetNormal(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, const DirectX::XMFLOAT3& c)
{
    DirectX::XMFLOAT3 v0(a.x - b.x, a.y - b.y, a.z - b.z);
    DirectX::XMFLOAT3 v1(a.x - c.x, a.y - c.y, a.z - c.z);
    DirectX::XMFLOAT3 normal(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    return DirectX::XMFLOAT3(normal.x / length, normal.y / length, normal.z / length);
}

uint8_t ConvertColor(const float color)
{
    return static_cast<uint8_t>(round(color * 255));
}

void MakeModelMesh(const cgltf_primitive& primitive, ModelData& modelData)
{
    for (cgltf_size i = 0; i < primitive.attributes_count; ++i)
    {
        if (primitive.attributes[i].type == cgltf_attribute_type_position && primitive.attributes[i].index == 0)
        {
            auto accessor = primitive.attributes[i].data;
            auto bufferView = accessor->buffer_view;
            auto buffer = bufferView->buffer;
            uint8_t* bufferData = (uint8_t*)buffer->data;
            bufferData = &bufferData[bufferView->offset + accessor->offset];
            float* positions = reinterpret_cast<float*>(bufferData);

            for (cgltf_size j = 0; j < accessor->count; ++j)
            {
                modelData.Vertices.push_back(positions[j * 3 + 0]);
                modelData.Vertices.push_back(positions[j * 3 + 1]);
                modelData.Vertices.push_back(positions[j * 3 + 2]);
            }
        }
        else if (primitive.attributes[i].type == cgltf_attribute_type_normal && primitive.attributes[i].index == 0)
        {
            auto accessor = primitive.attributes[i].data;
            auto bufferView = accessor->buffer_view;
            auto buffer = bufferView->buffer;
            uint8_t* bufferData = (uint8_t*)buffer->data;
            bufferData = &bufferData[bufferView->offset + accessor->offset];
            float* normals = reinterpret_cast<float*>(bufferData);

            for (cgltf_size j = 0; j < accessor->count; ++j)
            {
                modelData.Normals.push_back(-normals[j * 3 + 0]);
                modelData.Normals.push_back(-normals[j * 3 + 1]);
                modelData.Normals.push_back(-normals[j * 3 + 2]);
            }
        }
        else if (primitive.attributes[i].type == cgltf_attribute_type_texcoord && primitive.attributes[i].index == 0)
        {
            auto accessor = primitive.attributes[i].data;
            auto bufferView = accessor->buffer_view;
            auto buffer = bufferView->buffer;
            uint8_t* bufferData = (uint8_t*)buffer->data;
            bufferData = &bufferData[bufferView->offset + accessor->offset];
            float* texcoord0 = reinterpret_cast<float*>(bufferData);

            for (cgltf_size j = 0; j < accessor->count; ++j)
            {
                modelData.UV0.push_back(texcoord0[j * 2 + 0]);
                modelData.UV0.push_back(texcoord0[j * 2 + 1]);
            }
        }
    }

    if (primitive.indices)
    {
        auto accessor = primitive.indices;
        auto bufferView = accessor->buffer_view;
        auto buffer = bufferView->buffer;
        uint8_t* bufferData = (uint8_t*)buffer->data;
        bufferData = &bufferData[bufferView->offset + accessor->offset];
        if (accessor->component_type == cgltf_component_type_r_16u)
        {
            uint16_t* indices = reinterpret_cast<uint16_t*>(bufferData);
            for (cgltf_size j = 0; j < accessor->count; ++j)
            {
                modelData.Indices.push_back(indices[j]);
            }
        }
        else if (accessor->component_type == cgltf_component_type_r_32u)
        {
            uint32_t* indices = reinterpret_cast<uint32_t*>(bufferData);
            modelData.Indices.assign(indices, indices + accessor->count);
        }
        else if (accessor->component_type == cgltf_component_type_r_8u)
        {
            uint8_t* indices = bufferData;
            for (cgltf_size j = 0; j < accessor->count; ++j)
            {
                modelData.Indices.push_back(indices[j]);
            }
        }
    }
    else
    {
        size_t vertexCount = modelData.Vertices.size() / 3;
        for (size_t i = 0; i < vertexCount; ++i)
        {
            modelData.Indices.push_back(i);
        }
    }

    if (modelData.Normals.empty())
    {
        modelData.Normals.resize(modelData.Vertices.size(), 0.f);
        size_t triangleCount = modelData.Indices.size() / 3;
        for (size_t i = 0; i < triangleCount; ++i)
        {
            DirectX::XMFLOAT3 a(
                modelData.Vertices[3 * modelData.Indices[i * 3 + 0] + 0],
                modelData.Vertices[3 * modelData.Indices[i * 3 + 0] + 1],
                modelData.Vertices[3 * modelData.Indices[i * 3 + 0] + 2]
            );
            DirectX::XMFLOAT3 b(
                modelData.Vertices[3 * modelData.Indices[i * 3 + 1] + 0],
                modelData.Vertices[3 * modelData.Indices[i * 3 + 1] + 1],
                modelData.Vertices[3 * modelData.Indices[i * 3 + 1] + 2]
            );
            DirectX::XMFLOAT3 c(
                modelData.Vertices[3 * modelData.Indices[i * 3 + 2] + 0],
                modelData.Vertices[3 * modelData.Indices[i * 3 + 2] + 1],
                modelData.Vertices[3 * modelData.Indices[i * 3 + 2] + 2]
            );

            auto normal = GetNormal(a, b, c);
            modelData.Normals[3 * modelData.Indices[i * 3 + 0] + 0] = normal.x;
            modelData.Normals[3 * modelData.Indices[i * 3 + 0] + 1] = normal.y;
            modelData.Normals[3 * modelData.Indices[i * 3 + 0] + 2] = normal.z;
            modelData.Normals[3 * modelData.Indices[i * 3 + 1] + 0] = normal.x;
            modelData.Normals[3 * modelData.Indices[i * 3 + 1] + 1] = normal.y;
            modelData.Normals[3 * modelData.Indices[i * 3 + 1] + 2] = normal.z;
            modelData.Normals[3 * modelData.Indices[i * 3 + 2] + 0] = normal.x;
            modelData.Normals[3 * modelData.Indices[i * 3 + 2] + 1] = normal.y;
            modelData.Normals[3 * modelData.Indices[i * 3 + 2] + 2] = normal.z;
        }
    }

    if (modelData.UV0.empty())
    {
        modelData.UV0.resize(modelData.Vertices.size() / 3 * 2, 0.f);
    }
}

void MakeModelTexture(const cgltf_primitive& primitive, const std::string& filePath, ModelData& modelData)
{
    if (!primitive.material)
    {
        modelData.Basecolor = Texture(16, 16, 255, 0, 0, 255);
        modelData.MetallicRoughness = Texture(16, 16, 0, 255, 0, 255);
        modelData.Normal = Texture(16, 16, ConvertColor(0.5f), ConvertColor(0.5f), 255, 255);
        return;
    }

    std::filesystem::path fileFsPath(filePath);
    auto parentPath = fileFsPath.parent_path();
    auto material = primitive.material;
    if (material->pbr_metallic_roughness.base_color_texture.texture)
    {
        auto uri = material->pbr_metallic_roughness.base_color_texture.texture->image->uri;
        auto uriSize = ::strlen(uri);
        char* pTempUri = new char[uriSize + 1] {};
        strcpy(pTempUri, uri);
        cgltf_decode_uri(pTempUri);
        auto path = parentPath / pTempUri;
        auto pathStr = path.string();
        int width, height, n;
        uint8_t* picData = stbi_load(pathStr.c_str(), &width, &height, &n, 4);
        modelData.Basecolor.SetWidth(width);
        modelData.Basecolor.SetHeight(height);
        modelData.Basecolor.UpdateData(width, height, picData);
        stbi_image_free(picData);
        delete[]pTempUri;
    }
    else
    {
        auto x = material->pbr_metallic_roughness.base_color_factor[0];
        auto y = material->pbr_metallic_roughness.base_color_factor[1];
        auto z = material->pbr_metallic_roughness.base_color_factor[2];
        auto w = material->pbr_metallic_roughness.base_color_factor[3];
        modelData.Basecolor = Texture(16, 16, ConvertColor(x), ConvertColor(y), ConvertColor(z), ConvertColor(w));
    }

    if (material->pbr_metallic_roughness.metallic_roughness_texture.texture)
    {
        auto uri = material->pbr_metallic_roughness.metallic_roughness_texture.texture->image->uri;
        auto uriSize = ::strlen(uri);
        char* pTempUri = new char[uriSize + 1] {};
        strcpy(pTempUri, uri);
        cgltf_decode_uri(pTempUri);
        auto path = parentPath / pTempUri;
        auto pathStr = path.string();
        int width, height, n;
        uint8_t* picData = stbi_load(pathStr.c_str(), &width, &height, &n, 4);
        modelData.MetallicRoughness.SetWidth(width);
        modelData.MetallicRoughness.SetHeight(height);
        modelData.MetallicRoughness.UpdateData(width, height, picData);
        stbi_image_free(picData);
        delete[]pTempUri;
    }
    else
    {
        auto x = material->pbr_metallic_roughness.metallic_factor;
        auto y = material->pbr_metallic_roughness.roughness_factor;
        modelData.MetallicRoughness = Texture(16, 16, ConvertColor(x), ConvertColor(y), 0, 255);
    }

    if (material->normal_texture.texture)
    {
        auto uri = material->normal_texture.texture->image->uri;
        auto uriSize = ::strlen(uri);
        char* pTempUri = new char[uriSize + 1] {};
        strcpy(pTempUri, uri);
        cgltf_decode_uri(pTempUri);
        auto path = parentPath / pTempUri;
        auto pathStr = path.string();
        int width, height, n;
        uint8_t* picData = stbi_load(pathStr.c_str(), &width, &height, &n, 4);
        modelData.Normal.SetWidth(width);
        modelData.Normal.SetHeight(height);
        modelData.Normal.UpdateData(width, height, picData);
        stbi_image_free(picData);
        delete[]pTempUri;
    }
    else
    {
        modelData.Normal = Texture(16, 16, 127, 127, 255, 255);
    }
}

std::vector<ModelData> GetModelData(const std::string& modelPath)
{
    std::vector<ModelData> modelData;
    cgltf_options options{};
    cgltf_data* data = nullptr;
    cgltf_parse_file(&options, modelPath.data(), &data);
    cgltf_load_buffers(&options, data, modelPath.data());
    for (cgltf_size i = 0; i < data->meshes_count; ++i)
    {
        auto& gltfmesh = data->meshes[i];
        auto primitiveCount = gltfmesh.primitives_count;
        for (cgltf_size j = 0; j < primitiveCount; ++j)
        {
            ModelData model;
            auto primitive = gltfmesh.primitives[j];
            MakeModelMesh(primitive, model);
            MakeModelTexture(primitive, modelPath.data(), model);
            modelData.push_back(model);
        }
    }
    return modelData;
}