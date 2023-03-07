#pragma once

#include <DirectXMath.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Texture
{
public:
    Texture()
    {
        Width = 0;
        Height = 0;
    }

    Texture(uint32_t width, uint32_t height, uint8_t x, uint8_t y, uint8_t z, uint8_t w)
    {
        Width = width;
        Height = height;
        uint32_t numPixel = Width * Height;
        for (uint32_t i = 0; i < numPixel; ++i)
        {
            Data.push_back(x);
            Data.push_back(y);
            Data.push_back(z);
            Data.push_back(w);
        }
    }

    bool Empty() const
    {
        return Width == 0 && Height == 0 && Data.empty();
    }

    void SetWidth(uint32_t width)
    {
        Width = width;
    }

    void SetHeight(uint32_t height)
    {
        Height = height;
    }

    uint32_t GetWidth() const
    {
        return Width;
    }

    uint32_t GetHeight() const
    {
        return Height;
    }

    void UpdateData(uint32_t w, uint32_t h, void* data)
    {
        auto dataSize = w * h * 4;
        Data.assign(reinterpret_cast<uint8_t*> (data), reinterpret_cast<uint8_t*> (data) + dataSize);
    }

    const uint8_t* GetData() const
    {
        return Data.data();
    }

    uint8_t* GetData()
    {
        return Data.data();
    }

private:
    std::vector<uint8_t> Data;
    uint32_t Width;
    uint32_t Height;
};

struct ModelData
{
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> UV0;
    std::vector<uint32_t> Indices;
    Texture Basecolor;
    Texture MetallicRoughness;
    Texture Normal;
};

bool PreCheckModel(const std::string& filePath, std::string& reason);
std::vector<ModelData> GetModelData(const std::string& modelPath);
std::string GetJSONString(const std::string& outputPath, const std::vector<ModelData>& modelData);
