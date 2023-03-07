#include <Graphics/DX12Texture2D.h>

#include "d3dx12.h"

namespace DQ
{
    DX12Texture2D::DX12Texture2D(ID3D12Device* device, std::shared_ptr<Texture2D> texture)
    {
        pDevice = device;
        D3D12_RESOURCE_DESC textureDesc{};
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.Width = texture->GetWidth();
        textureDesc.Height = texture->GetHeight();
        textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        textureDesc.DepthOrArraySize = 1;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        CD3DX12_HEAP_PROPERTIES textureProps(D3D12_HEAP_TYPE_DEFAULT);
        pDevice->CreateCommittedResource(&textureProps, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&pResource));

        const UINT64 uploadBufferSize = GetRequiredIntermediateSize(pResource, 0, 1);
        CD3DX12_HEAP_PROPERTIES uploadProps(D3D12_HEAP_TYPE_UPLOAD);
        CD3DX12_RESOURCE_DESC uploadResDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
        pDevice->CreateCommittedResource(&uploadProps, D3D12_HEAP_FLAG_NONE, &uploadResDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pUploadResource));
    }

    DX12Texture2D::~DX12Texture2D()
    {
        pResource->Release();
        pUploadResource->Release();
    }

    void DX12Texture2D::CopyCpuToGpu(ID3D12GraphicsCommandList* list)
    {
        auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
        list->ResourceBarrier(1, &barrier);

        D3D12_SUBRESOURCE_DATA textureData{};
        textureData.pData = pTextureData->GetData();
        textureData.RowPitch = pTextureData->GetWidth() * 4;
        textureData.SlicePitch = textureData.RowPitch * pTextureData->GetHeight();
        UpdateSubresources(list, pResource, pUploadResource, 0, 0, 1, &textureData);

        barrier = CD3DX12_RESOURCE_BARRIER::Transition(pResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON);
        list->ResourceBarrier(1, &barrier);
    }

    std::shared_ptr<DX12Texture2D> CreateDX12Texture2D(ID3D12Device* device, std::shared_ptr<Texture2D> texture)
    {
        return std::shared_ptr<DX12Texture2D>(new DX12Texture2D(device, texture));
    }
}