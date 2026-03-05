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
DECL_HOOKv(PlaceMarker, unsigned int id, unsigned short type, CVector& pos, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned short pulsePeriod, float pulseFraction, short rotateRate, float unkX, float unkY, float unkZ, bool unkBool) {
    
    // 1. Ambil warna paksaan dari file konfigurasi .ini kamu
    unsigned char newR = (unsigned char)cfgMarkerR->GetInt();
    unsigned char newG = (unsigned char)cfgMarkerG->GetInt();
    unsigned char newB = (unsigned char)cfgMarkerB->GetInt();
    unsigned char newA = (unsigned char)cfgMarkerA->GetInt();
    PlaceMarker(id, type, pos, size, newR, newG, newB, newA, pulsePeriod, pulseFraction, rotateRate, unkX, unkY, unkZ, unkBool);
}

extern "C" void OnModLoad() {
    logger->SetTag("ColorMarker");
    cfgMarkerR = cfg->Bind("Red", 0, "Color");
    cfgMarkerG = cfg->Bind("Green", 0, "Color");
    cfgMarkerB = cfg->Bind("Blue", 255, "Color");
    cfgMarkerA = cfg->Bind("Alpha", 255, "Color"); 
    
    uintptr_t libGTASA = aml->GetLib("libGTASA.so");
    
    if (libGTASA) {
        logger->Info("libGTASA.so ditemukan! Color Marker aktif...");
        HOOK(PlaceMarker, libGTASA + 0x5C3620); 
    } else {
        logger->Error("Gagal menemukan libGTASA.so");
    }
}
