#pragma once

#include <PCH/PCH.hpp>

namespace DQ
{
    enum class COMMAND_LIST_TYPE
    {
        GRAPHICS,
        COPY,
        COMPUTE
    };

    class ICommandList
    {
    protected:
        ICommandList(ID3D12Device4* device, COMMAND_LIST_TYPE type)
        {
            mType = type;
            D3D12_COMMAND_LIST_TYPE dxType = D3D12_COMMAND_LIST_TYPE_DIRECT;
            if (type == COMMAND_LIST_TYPE::COPY)
            {
                dxType = D3D12_COMMAND_LIST_TYPE_COPY;
            }
            else if (type == COMMAND_LIST_TYPE::COMPUTE)
            {
                dxType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            }

            device->CreateCommandList1(0, dxType, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&pList));
            device->CreateCommandAllocator(dxType, IID_PPV_ARGS(&pAllocator));
        }
    public:
        ~ICommandList()
        {
            pAllocator->Release();
            pList->Release();
        }

        ID3D12GraphicsCommandList* GetList() const
        {
            return pList;
        }

        COMMAND_LIST_TYPE GetType() const
        {
            return mType;
        }

        void Open()
        {
            pAllocator->Reset();
            pList->Reset(pAllocator, nullptr);
        }

        void Close()
        {
            pList->Close();
        }

    private:
        COMMAND_LIST_TYPE mType;
        ID3D12CommandAllocator* pAllocator;
        ID3D12GraphicsCommandList* pList;

        friend std::shared_ptr<ICommandList> CreateCommandList(ID3D12Device4* device, COMMAND_LIST_TYPE type);
    };

    inline std::shared_ptr<ICommandList> CreateCommandList(ID3D12Device4* device, COMMAND_LIST_TYPE type)
    {
        return std::shared_ptr<ICommandList>(new ICommandList(device, type));
    }
}