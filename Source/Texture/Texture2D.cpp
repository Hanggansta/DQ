#include <Texture/Texture2D.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace DQ
{
    std::shared_ptr<Texture2D> CreateTexture2DFromFile(std::string_view path)
    {
        int w = 0, h = 0, n = 0;
        auto data = stbi_load(path.data(), &w, &h, &n, 0);
        
        std::vector<uint8_t> texData;
        uint32_t dataSize = w * h * n;
        if (n == 1)
        {
            for (uint32_t i = 0;i < dataSize; ++i)
            {
                texData.push_back(data[i]);
                texData.push_back(0);
                texData.push_back(0);
                texData.push_back(255);
            }
        }
        else if(n == 2)
        {
            for (uint32_t i = 0; i < dataSize; i += 2)
            {
                texData.push_back(0);
                texData.push_back(data[i]);
                texData.push_back(0);
                texData.push_back(data[i + 1]);
            }
        }
        else if (n == 3)
        {
            for (uint32_t i = 0; i < dataSize; i += 3)
            {
                texData.push_back(data[i]);
                texData.push_back(data[i + 1]);
                texData.push_back(data[i + 2]);
                texData.push_back(255);
            }
        }
        else if (n == 4)
        {
            for (uint32_t i = 0; i < dataSize; i += 4)
            {
                texData.push_back(data[i]);
                texData.push_back(data[i + 1]);
                texData.push_back(data[i + 2]);
                texData.push_back(data[i + 3]);
            }
        }

        stbi_image_free(data);
        return std::shared_ptr<Texture2D>(new Texture2D(w, h, texData.data(), texData.size()));
    }
}