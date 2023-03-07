#pragma once

#include <PCH/PCH.hpp>
#include <Texture/Texture2D.h>

namespace DQ
{
    class DX12Texture2D
    {
    protected:
        DX12Texture2D(ID3D12Device* device, std::shared_ptr<Texture2D> texture);

    public:

        ~DX12Texture2D();

        std::shared_ptr<Texture2D> GetTexture() const
        {
            return pTextureData;
        }

        ID3D12Resource* GetResource() const
        {
            return pResource;
        }

        void CopyCpuToGpu(ID3D12GraphicsCommandList* list);

    private:
        std::shared_ptr<Texture2D> pTextureData;
        ID3D12Device* pDevice;
        ID3D12Resource* pResource;
        ID3D12Resource* pUploadResource;

        friend std::shared_ptr<DX12Texture2D> CreateDX12Texture2D(ID3D12Device* device, std::shared_ptr<Texture2D> texture);
    };

    std::shared_ptr<DX12Texture2D> CreateDX12Texture2D(ID3D12Device* device, std::shared_ptr<Texture2D> texture);
}