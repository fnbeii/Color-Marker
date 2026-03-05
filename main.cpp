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

void PlaceMarker_Android(unsigned int id, unsigned short type, CVector& pos, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned short pulsePeriod, float pulseFraction, short rotateRate, float unkX, float unkY, float unkZ, bool unkBool) {
    if (pPlaceMarker != 0) {
        ((void(*)(unsigned int, unsigned short, CVector&, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned short, float, short, float, float, float, bool))(pPlaceMarker + 1))(id, type, pos, size, r, g, b, a, pulsePeriod, pulseFraction, rotateRate, unkX, unkY, unkZ, unkBool);
    }
}

CVector GetPlayerPos(int playerId = 0) {
    if (pFindPlayerCoors != 0) {
        return ((CVector(*)(int))(pFindPlayerCoors + 1))(playerId);
    }
    return {0.0f, 0.0f, 0.0f};
}

// Hook ke fungsi Idle (Jantung utama game)
DECL_HOOKv(Idle, void* data, bool bUnk) {
    Idle(data, bUnk); // Selalu jalankan fungsi aslinya dulu
    
    // Ambil posisi karakter kita
    CVector markerPos = GetPlayerPos(0);
    
    // Geser marker 2 meter ke samping agar tidak berada di dalam badan CJ
    markerPos.y += 2.0f; 
    markerPos.z -= 0.8f; // Turunkan sedikit ke tanah
    
    // Ambil nilai warna langsung dari konfigurasi .ini
    unsigned char r = (unsigned char)cfgMarkerR->GetInt();
    unsigned char g = (unsigned char)cfgMarkerG->GetInt();
    unsigned char b = (unsigned char)cfgMarkerB->GetInt();
    unsigned char a = (unsigned char)cfgMarkerA->GetInt();
    
    PlaceMarker_Android(1, 1, markerPos, 1.5f, r, g, b, a, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, false); 
}

extern "C" void OnModLoad() {
    logger->SetTag("ColorMarker");
    
    cfgMarkerR = cfg->Bind("Red", 255, "Color");
    cfgMarkerG = cfg->Bind("Green", 0, "Color");
    cfgMarkerB = cfg->Bind("Blue", 0, "Color");
    cfgMarkerA = cfg->Bind("Alpha", 255, "Color"); 
    
    uintptr_t libGTASA = aml->GetLib("libGTASA.so");
    
    if (libGTASA) {
        logger->Info("libGTASA.so ditemukan! Hooking Idle berjalan...");
        pPlaceMarker = libGTASA + 0x66FC60; 
        pFindPlayerCoors = libGTASA + 0x66F5F8; 
        HOOK(Idle, libGTASA + 0x67059C); 
    } else {
        logger->Error("Gagal menemukan libGTASA.so");
    }
}

