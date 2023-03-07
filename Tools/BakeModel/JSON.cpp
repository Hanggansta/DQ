#include "BakeModel.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace rapidjson;

void LoadVertex(Writer<StringBuffer> &writer, const ModelData& model)
{
    writer.Key("vertices");
    writer.StartArray();
    for (const auto& val : model.Vertices)
    {
        float _v = val;
        uint32_t* pVal = reinterpret_cast<uint32_t*>(&_v);
        writer.Uint(*pVal);
    }
    writer.EndArray();
}

void LoadNormals(Writer<StringBuffer>& writer, const ModelData& model)
{
    writer.Key("normals");
    writer.StartArray();
    for (const auto& val : model.Normals)
    {
        float _v = val;
        uint32_t* pVal = reinterpret_cast<uint32_t*>(&_v);
        writer.Uint(*pVal);
    }
    writer.EndArray();
}

void LoadUV0(Writer<StringBuffer>& writer, const ModelData& model)
{
    writer.Key("uv0");
    writer.StartArray();
    for (const auto& val : model.UV0)
    {
        float _v = val;
        uint32_t* pVal = reinterpret_cast<uint32_t*>(&_v);
        writer.Uint(*pVal);
    }
    writer.EndArray();
}

void LoadIndices(Writer<StringBuffer>& writer, const ModelData& model)
{
    writer.Key("indices");
    writer.StartArray();
    for (const auto& val : model.Indices)
    {
        writer.Uint(val);
    }
    writer.EndArray();
}

void LoadBasecolorTexture(Writer<StringBuffer>& writer, const ModelData& model, const std::string& outputPath)
{
    std::filesystem::path filePath(outputPath);
    auto fileParentPath = filePath.parent_path();
    auto fileStem = filePath.stem();
    auto fileStemString = fileStem.string();

    writer.Key("basecolor_texture");
    auto basecolorFileName = fileStemString + "_basecolor.tga";
    auto basecolorFilePath = fileParentPath / basecolorFileName;
    auto basecolorFilePathStr = basecolorFilePath.string();
    stbi_write_tga(basecolorFilePathStr.c_str(), model.Basecolor.GetWidth(), model.Basecolor.GetHeight(), 4, model.Basecolor.GetData());
    writer.String(basecolorFileName.c_str());
}

void LoadMetallicRoughnessTexture(Writer<StringBuffer>& writer, const ModelData& model, const std::string& outputPath)
{
    std::filesystem::path filePath(outputPath);
    auto fileParentPath = filePath.parent_path();
    auto fileStem = filePath.stem();
    auto fileStemString = fileStem.string();

    writer.Key("metallic_roughness_texture");
    auto metallicRoughnessFileName = fileStemString + "_metallic_roughness.tga";
    auto metallicRoughnessFilePath = fileParentPath / metallicRoughnessFileName;
    auto metallicRoughnessFilePathStr = metallicRoughnessFilePath.string();
    stbi_write_tga(metallicRoughnessFilePathStr.c_str(), model.MetallicRoughness.GetWidth(), model.MetallicRoughness.GetHeight(), 4, model.MetallicRoughness.GetData());
    writer.String(metallicRoughnessFileName.c_str());
}

void LoadNormalTexture(Writer<StringBuffer>& writer, const ModelData& model, const std::string& outputPath)
{
    std::filesystem::path filePath(outputPath);
    auto fileParentPath = filePath.parent_path();
    auto fileStem = filePath.stem();
    auto fileStemString = fileStem.string();

    writer.Key("normal_texture");
    auto normalFileName = fileStemString + "_normal.tga";
    auto normalFilePath = fileParentPath / normalFileName;
    auto normalFilePathStr = normalFilePath.string();
    stbi_write_tga(normalFilePathStr.c_str(), model.Normal.GetWidth(), model.Normal.GetHeight(), 4, model.Normal.GetData());
    writer.String(normalFileName.c_str());
}


std::string GetJSONString(const std::string& outputPath, const std::vector<ModelData>& modelData)
{
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    writer.StartArray();
    writer.StartObject();
    for (const auto& model : modelData)
    {
        writer.Key("type");
        writer.String("model");
        LoadVertex(writer, model);
        LoadNormals(writer, model);
        LoadUV0(writer, model);
        LoadIndices(writer, model);
        LoadBasecolorTexture(writer, model, outputPath);
        LoadMetallicRoughnessTexture(writer, model, outputPath);
        LoadNormalTexture(writer, model, outputPath);
    }
    writer.EndObject();
    writer.EndArray();
    return std::string(buffer.GetString());
}

