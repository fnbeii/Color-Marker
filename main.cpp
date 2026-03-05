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

// -------------------------------------------------------------------------
// HOOK DIREVISI: Menggunakan PlaceMarkerCone (11 Parameter) sesuai crashlog!
// Sandi: j (uint), R7CVector (CVector&), f (float), hhhh (4 uchar), t (ushort), f (float), s (short), h (uchar)
// -------------------------------------------------------------------------
DECL_HOOKv(PlaceMarkerCone, int id, CVector& pos, float size, int r, int g, int b, int a, int pulsePeriod, float pulseFraction, int rotateRate, int unkChar) {
    
    // Ambil warna paksaan dari file konfigurasi .ini
    int newR = cfgMarkerR->GetInt();
    int newG = cfgMarkerG->GetInt();
    int newB = cfgMarkerB->GetInt();
    int newA = cfgMarkerA->GetInt();
    
    // Lanjutkan fungsi asli, TAPI ganti r, g, b, a dengan warna dari .ini kita!
    PlaceMarkerCone(id, pos, size, newR, newG, newB, newA, pulsePeriod, pulseFraction, rotateRate, unkChar);
}

extern "C" void OnModLoad() {
    logger->SetTag("ColorMarker");
    
    // Konfigurasi bawaan: Merah Solid
    cfgMarkerR = cfg->Bind("Red", 255, "Color");
    cfgMarkerG = cfg->Bind("Green", 0, "Color");
    cfgMarkerB = cfg->Bind("Blue", 0, "Color");
    cfgMarkerA = cfg->Bind("Alpha", 255, "Color"); 
    
    uintptr_t libGTASA = aml->GetLib("libGTASA.so");
    
    if (libGTASA) {
        logger->Info("libGTASA.so ditemukan! Hooking PlaceMarkerCone...");
        
        // Hook alamat 0x5C3620 dengan fungsi PlaceMarkerCone yang baru
        HOOK(PlaceMarkerCone, libGTASA + 0x5C3620 + 0x1); 
    } else {
        logger->Error("Gagal menemukan libGTASA.so");
    }
}


