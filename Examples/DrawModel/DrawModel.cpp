#include <Graphics/Device.h>
#include <Renderer/Renderer.h>
#include <Scene/Scene.h>
#include <Utility/Window.h>

class DrawModel : public DQ::IApp
{
public:
    bool Init()
    {
        pDevice = DQ::CreateDevice(mHwnd.value(), mSettings.mWidth, mSettings.mHeight);
        pScene = DQ::CreateScene(mSettings.mWidth, mSettings.mHeight);
        pRenderer = DQ::CreateRenderer(pDevice);

        pScene->AddCustomData("Model/Model.dq");
        return true;
    }

    void Exit() {}

    void Update(float deltaTime) {}

    void Draw()
    {
        pRenderer->Draw();
        pDevice->Present();
    }

    const char* GetName()
    {
        return "DrawModel";
    }

    std::shared_ptr<DQ::IDevice> pDevice;
    std::shared_ptr<DQ::IScene> pScene;
    std::shared_ptr<DQ::IRenderer> pRenderer;
};

DEFINE_APPLICATION_MAIN(DrawModel)