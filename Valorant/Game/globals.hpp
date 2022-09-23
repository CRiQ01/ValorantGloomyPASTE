#pragma once
#include <Windows.h>


int process_id;
DWORD64 image_base;


bool InLobby = false;
std::string string21337;

namespace globals {
    static HANDLE handleOffset;
    static uintptr_t pid;
    static uintptr_t base;
    static UINT_PTR
        localplayercontroller,
        localplayerpawn,
        localdamagehandler,
        localcameramanager;
    static int localteamid;


}

namespace settings
{
    inline bool show_menu = true;
    inline bool fovchanger = false;
    inline float fovchangervalue = 100.0f;

    namespace aimbot 
    {
        inline bool aimbot = false;
        inline float fov = 150;
        inline float smoothness = 1.0f;
        inline bool smooth = false;
        inline bool drawfov = false;
        inline bool vischeck = false;
        inline bool filledfov = false;
        inline  int selectedhitbox = 0;
        inline  const char* hitbox[] = {("Head") ,("Neck"), ("Body") };
        inline bool silent = false;
        inline bool rcs = false;
        inline bool rcscrosshair = false;
        inline bool nazicrosshair = false;
        inline bool rapidfire = false;
    }

    namespace visuals 
    {
        inline bool skeleton = false;
        inline bool snapline = false;

        inline  int snaplinepos = 2;
        inline  const char* snappos[] = { ("Top") ,("Crosshair"), ("Bottom") };

        inline  int healthtype = 0;
        inline  const char* healtht[] = { ("Text") ,("Bar"), ("Both") };

        inline  int headboxtype = 0;
        inline  const char* headboxt[] = { ("Circle") ,("Filled Circle"), ("Dot") };


        inline bool distance = false;
        inline bool box = false;
        inline bool box3d = false;
        inline bool d3box = false;
        inline bool weapon = true;
        inline bool glow = false;
        int glowintensity = 100;
        inline bool radar = false;
        inline bool dorment = false;
        inline bool health = false;
        inline bool filledbox = false;
        inline bool headcircle = false;
        inline bool rendercount = false;
        inline bool agentesp = false;
        inline bool weaponesp = false;
        inline bool ammoesp = false;
        inline bool spiketimer = false;
        inline bool weaponstate = false;
        inline bool textesp = false;
        inline bool shield = false;
        inline bool jointesp = false;
        inline bool visibleesp = false;
        inline bool nameesp = false;
        inline bool teamswap = false;
        inline bool chams = false;
    }

    namespace visualsteam
    {
        inline bool teambox = false;
        inline bool snaplineteam = false;
        inline  int snaplineposteam = 2;
        inline  const char* snapposteam[] = { ("Top") ,("Crosshair"), ("Bottom") };
        inline bool skeleton = false;
    }
}

namespace offsets
{
    inline DWORD
        World_Key = 0x8E0C4B8,
        World_State = 0x8E0C480,
        GameInstance = 0x1A0,
        PersistentLevel = 0x38,
        LocalPlayerArray = 0x40,
        PlayerController = 0x38,
        PlayerCameraManager = 0x478,
        //PlayerCameraManager = 0x1fa0,
        AcknowledgedPawn = 0x460,
        PlayerState = 0x3F0,
        TeamComponent = 0x628,
        TeamID = 0xF8,
        GameState = 0x150,
        ActorArray = 0xA0,
        ActorCount = 0xB8,
        root_position = 0x164,
        RootComponent = 0x230,
        RelativeLocation = 0x164,
        camera_position = 0x1260,
        camera_rotation = 0x126C,
        MeshComponent = 0x430,
        DamageHandler = 0x9A8,
        bIsDormant = 0x100,
        Health = 0x1b0,
        UniqueID = 0x38,
        BoneCount = 0x5b0 + 0x8,
        FresnelIntensity = 0x6b8,
        last_submit_time = 0x364 + 0x4,
        last_render_time = last_submit_time + 0x4,
        component_to_world = 0x250,
        camera_fov = 0x1278,
        ObjID = 0x18,
        Inventory = 0x948,
        CurrentWeapon = 0x218,
        CurrentAmmo = 0xFB0,
        MaxAmmo = 0x11C,
        Spiketime = 0x4D4,
        Shield = 0x25DC6B0,
        dormant = 0x100,
        camera_cache = 0x1220 + 0x20,
        playername = 0x4b8;
}