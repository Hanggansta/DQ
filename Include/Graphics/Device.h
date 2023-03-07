#pragma once

#include <PCH/PCH.hpp>
#include <Graphics/DX12CommandList.h>

namespace DQ
{
    class IDevice
    {
    public:
        virtual ID3D12Device8* GetDevice() const = 0;
        virtual void Present() = 0;
        virtual void ExecuteList(ICommandList* list) = 0;
        virtual void Wait(COMMAND_LIST_TYPE type) = 0;
    };

    std::shared_ptr<IDevice> CreateDevice(HWND hwnd, uint32_t width, uint32_t height);
}

