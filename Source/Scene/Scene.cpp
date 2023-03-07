#include <Scene/Scene.h>

namespace DQ
{
    class Scene : public IScene
    {
    public:
        Scene(uint32_t width, uint32_t height)
        {

        }

        void AddCustomData(std::string_view path)
        {
            auto jsonStr = _getFileJSON(path);


        }

        std::string _getFileJSON(std::string_view path)
        {
            std::ifstream ifs(path.data());
            std::stringstream buffer;
            buffer << ifs.rdbuf();
            return buffer.str();
        }
    };

    std::shared_ptr<IScene> CreateScene(uint32_t width, uint32_t height)
    {
        return std::shared_ptr<IScene>(new Scene(width, height));
    }
}