#include <Graphics/DX12Texture2D.h>
#include <Graphics/DX12CommandList.h>
#include <Renderer/Renderer.h>

#include "d3dx12.h"

namespace DQ
{
    class Renderer : public IRenderer
    {
    public:
        Renderer(std::shared_ptr<IDevice> device)
        {
            pDevice = device;
            _createInternalResource();
            _createShaderDescriptorHeap(16);
            _createInternalView();
        }

        //void LoadTextureData(const std::vector<std::shared_ptr<Texture2D>>& texture)
        //{
        //    mNumTexture = texture.size();

        //    if (mNumShaderDescriptorHeap < mNumTexture + mNumInternalShaderResource)
        //    {
        //        _createShaderDescriptorHeap(mNumTexture + mNumInternalShaderResource);
        //        _createInternalView();
        //    }

        //    CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(pVisibleShaderResourceDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        //    auto handleSize = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        //    descriptorHandle.Offset(mNumInternalShaderResource, handleSize);

        //    for (uint32_t i = 0; i < mNumTexture; ++i)
        //    {
        //        auto dx12Texture = CreateDX12Texture2D(pDevice->GetDevice(), texture[i]);
        //        mDX12Texture.push_back(dx12Texture);

        //        pInternalCmdList->Open();

        //        dx12Texture->CopyCpuToGpu(pInternalCmdList->GetList());

        //        pInternalCmdList->Close();
        //        pDevice->ExecuteList(pInternalCmdList.get());
        //        pDevice->Wait(pInternalCmdList->GetType());

        //        pDevice->GetDevice()->CreateShaderResourceView(dx12Texture->GetResource(), nullptr, descriptorHandle);
        //        descriptorHandle.Offset(handleSize);
        //    }
        //}

        void Draw()
        {

        }

        void _createInternalResource()
        {
            pInternalCmdList = CreateCommandList(pDevice->GetDevice(), COMMAND_LIST_TYPE::GRAPHICS);

            CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
            CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(256);
            pDevice->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pInternalResource0));
        }

        void _createShaderDescriptorHeap(uint32_t num)
        {
            mNumShaderDescriptorHeap = num;

            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
            descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            descriptorHeapDesc.NodeMask = 0;
            descriptorHeapDesc.NumDescriptors = mNumShaderDescriptorHeap;
            descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            pDevice->GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&pVisibleShaderResourceDescriptorHeap));
        }

        void _createInternalView()
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE descriptorHandle(pVisibleShaderResourceDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
            cbvDesc.SizeInBytes = 256;
            cbvDesc.BufferLocation = pInternalResource0->GetGPUVirtualAddress();
            pDevice->GetDevice()->CreateConstantBufferView(&cbvDesc, descriptorHandle);
        }

        std::shared_ptr<IDevice> pDevice;

        std::shared_ptr<ICommandList> pInternalCmdList;

        std::vector<std::shared_ptr<DX12Texture2D>> mDX12Texture;

        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pVisibleShaderResourceDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> pInternalResource0;

        uint32_t mNumTexture = 0;
        uint32_t mNumShaderDescriptorHeap = 0;
        uint32_t mNumInternalShaderResource = 1;
    };

    std::shared_ptr<IRenderer> CreateRenderer(std::shared_ptr<IDevice> device)
    {
        return std::shared_ptr<IRenderer>(new Renderer(device));
    }
}