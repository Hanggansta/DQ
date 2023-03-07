#pragma once

#include <PCH/PCH.hpp>

namespace DQ
{
    class Texture2D
    {
    protected:
        Texture2D(uint32_t width, uint32_t height, uint8_t* data, uint32_t dataSize)
        {
            mWidth = width;
            mHeight = height;

            for (uint32_t i = 0; i < dataSize; ++i)
            {
                mData.push_back(data[i]);
            }
        }
    public:

        uint32_t GetWidth() const
        {
            return mWidth;
        }

        uint32_t GetHeight() const
        {
            return mHeight;
        }

        void* GetData()
        {
            return mData.data();
        }

    private:
        uint32_t mWidth;
        uint32_t mHeight;
        std::vector<uint8_t> mData;

        friend std::shared_ptr<Texture2D> CreateTexture2DFromFile(std::string_view path);
    };

    std::shared_ptr<Texture2D> CreateTexture2DFromFile(std::string_view path);
}