#pragma once

#include <PCH/PCH.hpp>
#include <Graphics/Device.h>

namespace DQ
{
    class IRenderer
    {
    public:
        virtual void Draw() = 0;
    };

    std::shared_ptr<IRenderer> CreateRenderer(std::shared_ptr<IDevice> device);
}