#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h> 

MYMODCFG(net.namakamu.colormarker, Color Marker SAMP, 1.0, NamaKamu)

ConfigEntry* cfgMarkerR;
ConfigEntry* cfgMarkerG;
ConfigEntry* cfgMarkerB;
ConfigEntry* cfgMarkerA;

struct CVector {
    float x, y, z;
};

uintptr_t pPlaceMarker = 0;
uintptr_t pFindPlayerCoors = 0;

void PlaceMarker_Android(unsigned int id, unsigned short type, CVector& pos, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned short pulsePeriod, float pulseFraction, short rotateRate) {
    if (pPlaceMarker != 0) {
        ((void(*)(unsigned int, unsigned short, CVector&, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned short, float, short))(pPlaceMarker + 1))(id, type, pos, size, r, g, b, a, pulsePeriod, pulseFraction, rotateRate);
    }
}

CVector GetPlayerPos(int playerId = 0) {
    if (pFindPlayerCoors != 0) {
        return ((CVector(*)(int))(pFindPlayerCoors + 1))(playerId);
    }
    return {0.0f, 0.0f, 0.0f};
}

DECL_HOOKv(GameProcess) {
    GameProcess(); 
    
    CVector markerPos = GetPlayerPos(0);
    markerPos.y += 2.0f; 
    markerPos.z -= 0.8f; 
    
    // Mengambil nilai warna dari config secara dinamis tiap frame
    unsigned char r = (unsigned char)cfgMarkerR->GetInt();
    unsigned char g = (unsigned char)cfgMarkerG->GetInt();
    unsigned char b = (unsigned char)cfgMarkerB->GetInt();
    unsigned char a = (unsigned char)cfgMarkerA->GetInt();
    
    // Memanggil marker silinder
    PlaceMarker_Android(1, 1, markerPos, 1.5f, r, g, b, a, 0, 0.0f, 0); 
}

extern "C" void OnModLoad() {
    logger->SetTag("ColorMarker");
    cfgMarkerR = cfg->Bind("Red", 255, "Color");
    cfgMarkerG = cfg->Bind("Green", 0, "Color");
    cfgMarkerB = cfg->Bind("Blue", 0, "Color");
    cfgMarkerA = cfg->Bind("Alpha", 255, "Color"); 
    
    uintptr_t libGTASA = aml->GetLib("libGTASA.so");
    
    if (libGTASA) {
        logger->Info("libGTASA.so ditemukan!");
        
        pPlaceMarker = libGTASA + 0x5C3620; 
        pFindPlayerCoors = libGTASA + 0x40B5DC; 
        
        HOOK(GameProcess, libGTASA + 0x3F3FB0); 
    } else {
        logger->Error("Gagal menemukan libGTASA.so");
    }
}
