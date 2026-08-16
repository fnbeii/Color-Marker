#include <mod/amlmod.h>
#include <mod/config.h>
#include <mod/logger.h>
#include <stdint.h>

MYMODCFG(mnfy.custom3dmarker, Custom 3D Marker, 1.0, mnfy)

uint8_t markerRed = 255;
uint8_t markerGreen = 0;
uint8_t markerBlue = 0;
uint8_t markerAlpha = 150;

DECL_HOOKv(C3dMarker_Render, void* self)
{
    uint8_t* pColorR = (uint8_t*)((uintptr_t)self + 0x58);
    uint8_t* pColorG = (uint8_t*)((uintptr_t)self + 0x59);
    uint8_t* pColorB = (uint8_t*)((uintptr_t)self + 0x5A);
    uint8_t* pColorA = (uint8_t*)((uintptr_t)self + 0x5B);

    *pColorR = markerRed;
    *pColorG = markerGreen;
    *pColorB = markerBlue;
    *pColorA = markerAlpha;

    C3dMarker_Render(self);
}

extern "C" void OnModLoad()
{
    logger->SetTag("Custom3dMarker");
    cfg->Bind("Author", "", "About")->SetString("mnfy"); cfg->ClearLast();
    
    uintptr_t pGame = aml->GetLib("libGTASA.so");
    if(pGame)
    {
        uintptr_t sym_MarkerRender = aml->GetSym(pGame, "_ZN9C3dMarker6RenderEv");
        
        if(sym_MarkerRender) {
            HOOK(C3dMarker_Render, sym_MarkerRender);
        } else {
            HOOK(C3dMarker_Render, pGame + 0x5C3160 + 0x1);
        }
        
        logger->Info("Custom3dMarker V1.0 successfully loaded!");
    }
    
    markerRed   = cfg->Bind("Color_R", 255, "Colors")->GetInt();
    markerGreen = cfg->Bind("Color_G", 0, "Colors")->GetInt();
    markerBlue  = cfg->Bind("Color_B", 0, "Colors")->GetInt();
    markerAlpha = cfg->Bind("Alpha", 150, "Colors")->GetInt();
}
