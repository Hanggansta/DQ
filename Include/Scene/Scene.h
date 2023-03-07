#pragma once

#include <PCH/PCH.hpp>

namespace DQ
{
    class IScene
    {
    public:
        // Load Custom Data.
        virtual void AddCustomData(std::string_view path) = 0;
        
    };

    std::shared_ptr<IScene> CreateScene(uint32_t width, uint32_t height);
}