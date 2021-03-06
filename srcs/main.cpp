#define STB_IMAGE_IMPLEMENTATION

#include "Application.h"

int main(void)
{
    Application* app = new Application;
    int appResult = app->AppRun();
    
    delete app;
    app = nullptr;

    _CrtDumpMemoryLeaks();
    return appResult;
}
