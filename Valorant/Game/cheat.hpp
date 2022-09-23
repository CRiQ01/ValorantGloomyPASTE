#pragma once
#include "sdk.hpp"
#include <iostream>
#include "globals.hpp"
#include "../player.h"

using namespace Globals;
using namespace camera;

static std::vector<EnemyPtr> enemy_collection;

typedef struct _FNlEntity {
	uint64_t Actor;
	uint64_t MeshComponent;
	uintptr_t root_component;
}FNlEntity;
std::vector<FNlEntity> entityList;

int LocalUID;
DWORD_PTR PlayerState, RootComponent, LocalPawn;

/*
FINLINE std::string GetCurrentWeapon()
{
	auto Inventory = Read<intptr_t>(actor_ptr + offsets::Inventory);
	intptr_t CurrentEquip = Read<intptr_t>(Inventory + offsets::CurrentWeapon);
	int weaponObj = Read<int>(CurrentEquip + offsets::ObjID);
	return GetWeaponName(weaponObj);
}

FINLINE std::string GetCharacter()
{
	int charObj = Read<int>(actor_ptr + offsets::ObjID);
	auto characterName = GetCharacterName(charObj);
	return characterName;
}
*/




auto CACHETHREAD() -> void
{
	while (true)
	{
		std::vector<FNlEntity> tmpList;

		auto Uworld = decryptWorld(image_base);
		
		auto GameInstance = Read<DWORD_PTR>(Uworld + offsets::GameInstance);
		
		auto PersistentLevel = Read<uintptr_t>(Uworld + offsets::PersistentLevel);
		
		auto LocalPlayerArray = Read<DWORD_PTR>(GameInstance + offsets::LocalPlayerArray);
		auto LocalPlayer = Read<DWORD_PTR>(LocalPlayerArray);



		PlayerController = Read<DWORD_PTR>(LocalPlayer + offsets::PlayerController);
		PlayerCameraManager = Read<DWORD_PTR>(PlayerController + offsets::PlayerCameraManager);

		LocalPawn = Read<DWORD_PTR>(PlayerController + offsets::AcknowledgedPawn);

		if (LocalPawn != 0)
		{
			LocalUID = Read<int>(LocalPawn + offsets::UniqueID);
			PlayerState = Read<DWORD_PTR>(LocalPawn + offsets::PlayerState);
			RootComponent = Read<DWORD_PTR>(LocalPawn + offsets::RootComponent);
		}

		auto ActorArray = Read<uintptr_t>(PersistentLevel + offsets::ActorArray);
		int ActorCount = Read<int>(PersistentLevel + offsets::ActorCount);

		for (int i = 0; i < ActorCount; i++)
		{
			auto CurrentActor = Read<DWORD_PTR>(ActorArray + (i * 0x8));
			if (CurrentActor == 0) continue;

			auto ActorUID = Read<int>(CurrentActor + offsets::UniqueID);
			if (LocalUID != ActorUID) continue;

			auto MeshComponent = Read<uint64_t>(CurrentActor + offsets::MeshComponent);
			if (MeshComponent == 0) continue;

			auto BoneCount = Read<int>(MeshComponent + offsets::BoneCount);
			if (BoneCount < 90) continue;

			FNlEntity fnlEntity{ };
			fnlEntity.Actor = CurrentActor;
			//fnlEntity.MeshComponent = MeshComponent;
			tmpList.push_back(fnlEntity);
		}
		entityList.clear();
		entityList = tmpList;
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}



std::string GetCharacterName(int id)
{

	switch (id)
	{
	case 12814502: return "ASTRA";
		break;
	case 14230510: return "JETT";
		break;
	case 14211583: return "CHAMBER";
		break;
	case 14219028: return "KILLJOY";
		break;
	case 14209948: return "RAZE"; //ok
		break;
	case 14228884: return "REYNA"; //ok
		break;
	case 14228027: return "SAGE"; //ok
		break;
	case 12807475: return "SKYE";
		break;
	case 14225108: return "NEON";
		break;
	case 14220733: return "VIPER";
		break;
	case 12801214: return "BREACH";
		break;
	case 14224190: return "BRIMSTONE";
		break;
	case 14216462: return "CYPHER";
		break;
	case 14229275: return "OMEN";
		break;
	case 14221852: return "PHOENIX"; //ok
		break;
	case 14218064: return "SOVA"; //ok
		break;
	case 12818495: return "YORU";
		break;
	case 12806381: return "KAY/O";
		break;
	case 14207406: return "FADE";
		break;
	case 14228837: return  "BOT"; //ok
		break;
	case 14228831: return "Train BOT"; //ok
		break;
	default:
		return std::to_string(id);
		break;
	}
}




std::string GetWeaponName(int id)
{
	switch (id)
	{
	default: return std::to_string(id);
		break;
	case 14391538:  return "Knife";
		break;
	case 14228826:  return "Classic";
		break;
	case 14344465:  return "Shorty";
		break;
	case 14337836:  return "Sheriff";
		break;
	case 14318008:  return "Frenzy";
		break;
	case 14332155:  return "Ghost";
		break;
	case 14381675:  return "Stinger";
		break;
	case 14372761:  return "Spectre";
		break;
	case 14311523:  return "Bucky";
		break;
	case 14307093:  return "Judge";
		break;
	case 14291226:  return "Bulldog";
		break;
	case 14359498:  return "Guardian";
		break;
	case 14296107:  return "Phantom";
		break;
	case 14282114:  return "Vandal";
		break;
	case 14367503:  return "Marshal";
		break;
	case 14348192:  return "Operator";
		break;
	case 14277573:  return "Ares";
		break;
	case 14273931:  return "Odin";
		break;
	case 14246916: return "Spike";
		break;
	case 14246939: return "Defuser";
		break;
	case 14231466: return "Orb";
		break;
		//omen
	case 14229727: return "Dark Cover";
		break;
	case 14230055: return "Paranoia";
		break;
	case 14229906: return "Shrouded Step";
		break;
	case 14230204: return "Omen Ult";
		break;
		//PH
	case 14222191: return "Curveball";
		break;
	case 14222400: return "Blaze";
		break;
	case 14222142: return "Hot Hands";
		break;
		//raze
	case 14210204: return "Boom Bot";
		break;
	case 14210462: return "Blast Pack";
		break;
	case 14210020: return "Paint Shells";
		break;
	case 14210631: return "Raze Ult";
		break;
		//Rey
	case 14228942: return "Leer";
		break;
		//sage
	case 14228246: return "Barrier Orb";
		break;
	case 14228091: return "Slow Orb";
		break;
	case 14228481: return "Healing Orb";
		break;
	case 14228592: return "Sage Revive";
		break;
		//sky
	case 14215701: return "Regrowth";
		break;
	case 14216133: return "Trailblazer";
		break;
	case 14215825: return "Guiding Light";
		break;
	case 14216541: return "Skye Ult";
		break;
		//sova
	case 14218662: return "Owl Drone";
		break;
	case 14218154: return "Shock Bolt";
		break;
	case 14218787: return "Recon Bolt";
		break;
	case 14219051: return "Sova Ult";
		break;
		//viper
	case 14221364: return "Snake Bite";
		break;
	case 14220902: return "Poison Cloud";
		break;
	case 14221113: return "Toxic Screen";
		break;
	case 14221516: return "Viper Ult";
		break;
		//yuro
	case 14227009: return "Fakeout";
		break;
	case 14227646: return "Blindside";
		break;
	case 14227289: return "Gatecrash";
		break;
	case 14227750: return "Yuro Ult";
		break;
		//Astra
	case 14223982: return "Astra Form";
		break;
	case 14223161: return "Nebula";
		break;
	case 14223358: return "Gravity Well";
		break;
	case 14223036: return "Nova Pulse";
		break;
		//Breach
	case 14209448: return "Aftershock";
		break;
	case 14209703: return "Flashpoint";
		break;
	case 14209591: return "Fault Line";
		break;
	case 14209806: return "Breach Ult";
		break;
		//Brim
	case 14224718: return "Stim Beacon";
		break;
	case 14224457: return "Incendiary";
		break;
	case 14224248: return "Sky Smoke";
		break;
	case 14224597: return "Brim Ult";
		break;
		//Chamb
	case 14212087: return "Trademark";
		break;
	case 14212948: return "Headhunter";
		break;
	case 14212492: return "Redezvous";
		break;
	case 14213404: return "TOUR DE FORCE";
		break;
		//Cy
	case 14217018: return "Trapwire";
		break;
	case 14217913: return "Cyber Cage";
		break;
	case 14217398: return "Spycam";
		break;
	case 14217406: return "Spycam (Active)";
		break;
		//fade
	case 14207896: return "PROWLER";
		break;
	case 14208676: return "SEIZE";
		break;
	case 14208386: return "HAUNT";
		break;
	case 14209041: return "FADE ULT";
		break;
		//Jett
	case 14230583: return "Cloudburst";
		break;
	case 14230842: return "Updraft";
		break;
	case 14230880: return "Jett Ult";
		break;
		//ko
	case 14215013: return "Fragment";
		break;
	case 14214617: return "Flashdrive";
		break;
	case 14214765: return "Zeropoint";
		break;
		//KJ
	case 14219574: return "Nanoswarm";
		break;
	case 14220186: return "Alarmbot";
		break;
	case 14219779: return "Turret";
		break;
	case 14220454: return "KJ Ult";
		break;
		//neon
	case 14225174: return "Fast Lane";
		break;
	case 14225484: return "Relay Bolt";
		break;
	case 14225329: return "High Gear";
		break;
	case 14225817: return "Neon Ult";
		break;


	case 0: return "AFK";
		break;
	}
}

enum class EAresEquippableState : uint8_t
{
	Unknown = 0,
	Idle = 1,
	Equipping = 2,
	Inspecting = 3,
	Attacking = 4,
	Firing = 5,
	Reloading = 6,
	ADS_Idle = 7,
	ADS_Firing = 8,
	Count = 9,
	EAresEquippableState_MAX = 10,
};

std::string GetEquippableVFXState(EAresEquippableState STATE) {
	switch (STATE) {
	case EAresEquippableState::Reloading:
		return "Reloading..";
		break;
	case EAresEquippableState::Inspecting:
		return "Inspecting";
		break;
	case EAresEquippableState::Firing:
		return "Shooting";
		break;
	case EAresEquippableState::ADS_Firing:
		return "Shooting";
		break;
	case EAresEquippableState::Attacking:
		return "Shooting";
	break;
	default:
		return "No Action";
	}
}

void drawRecoil() {
	float screen_width = GetSystemMetrics(SM_CXSCREEN);
	float screen_height = GetSystemMetrics(SM_CYSCREEN);

	Vector3 punchAngle = (CameraRotation - CameraRotation);

	float centerX = screen_width / 2; // may need to add or subtract to make perfect
	float centerY = screen_height / 2;

	int dx = screen_width / 90;
	int dy = screen_height / 90;

	centerX += (dx * (punchAngle.y));
	centerY -= (dy * (punchAngle.x));

	ImColor White = { 255, 255, 255, 255 };
	auto draw = ImGui::GetOverlayDrawList();
	draw->AddCircle({ centerX, centerY }, 5, White, 0, 2);
}

void DrawCrossNazi(int buyukluk, DWORD color)
{
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	int crosspozisyon = window_pos.x + 900;
	int crosspozisyony = window_pos.y + 479;
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon, crosspozisyony - buyukluk), ImVec2(crosspozisyon, crosspozisyony + buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon - buyukluk, crosspozisyony), ImVec2(crosspozisyon + buyukluk, crosspozisyony), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon, crosspozisyony + buyukluk), ImVec2(crosspozisyon - buyukluk, crosspozisyony + buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon, crosspozisyony - buyukluk), ImVec2(crosspozisyon + buyukluk, crosspozisyony - buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon - buyukluk, crosspozisyony), ImVec2(crosspozisyon - buyukluk, crosspozisyony - buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon + buyukluk, crosspozisyony), ImVec2(crosspozisyon + buyukluk, crosspozisyony + buyukluk), ImColor(color));
}

void DrawCrossNazi1(int buyukluk, DWORD color)
{
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	int crosspozisyon = window_pos.x + window_size.x * 0.5f;
	int crosspozisyony = window_pos.y + window_size.y * 0.5f;
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon, crosspozisyony - buyukluk), ImVec2(crosspozisyon, crosspozisyony + buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon - buyukluk, crosspozisyony), ImVec2(crosspozisyon + buyukluk, crosspozisyony), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon, crosspozisyony + buyukluk), ImVec2(crosspozisyon - buyukluk, crosspozisyony + buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon, crosspozisyony - buyukluk), ImVec2(crosspozisyon + buyukluk, crosspozisyony - buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon - buyukluk, crosspozisyony), ImVec2(crosspozisyon - buyukluk, crosspozisyony - buyukluk), ImColor(color));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(crosspozisyon + buyukluk, crosspozisyony), ImVec2(crosspozisyon + buyukluk, crosspozisyony + buyukluk), ImColor(color));
}

void DrawLineSimple(Vector3 first_bone_position, Vector3 second_bone_position) {

	auto first_bone_screen_position = ProjectWorldToScreen(first_bone_position);
	if (first_bone_screen_position.x == 0 && first_bone_screen_position.y == 0) return;

	auto second_bone_screen_position = ProjectWorldToScreen(second_bone_position);
	if (second_bone_screen_position.x == 0 && second_bone_screen_position.y == 0) return;

	ImColor White = { 68, 255, 0, 255 };

	DrawLine(ImVec2(first_bone_screen_position.x, first_bone_screen_position.y), ImVec2(second_bone_screen_position.x, second_bone_screen_position.y), White, 0.5f);
}



void DrawSkeleton(uintptr_t Mesh) {
	Vector3 NeckPos{};
	Vector3 LeftUpperArm{};
	Vector3 LeftElbow{};
	Vector3 LeftHand{};
	Vector3 RightUpperArm{};
	Vector3 RightElbow{};
	Vector3 RightHand{};
	Vector3 Pelvis{};
	Vector3 LeftThigh{};
	Vector3 LeftKnee{};
	Vector3 LeftFoot{};
	Vector3 RightThigh{};
	Vector3 RightKnee{};
	Vector3 RightFoot{};

	int BoneCount = Read<int>(Mesh + 0x560);

	if (BoneCount == 101) {
		NeckPos = GetBone(Mesh, 7);
		LeftUpperArm = GetBone(Mesh, 45);
		LeftElbow = GetBone(Mesh, 24);
		LeftHand = GetBone(Mesh, 29);
		RightUpperArm = GetBone(Mesh, 70);
		RightElbow = GetBone(Mesh, 67);
		RightHand = GetBone(Mesh, 50);
		Pelvis = GetBone(Mesh, 3);
		LeftThigh = GetBone(Mesh, 75);
		LeftKnee = GetBone(Mesh, 76);
		LeftFoot = GetBone(Mesh, 78);
		RightThigh = GetBone(Mesh, 82);
		RightKnee = GetBone(Mesh, 83);
		RightFoot = GetBone(Mesh, 85);
	}
	else {
		NeckPos = GetBone(Mesh, 19);
		LeftUpperArm = GetBone(Mesh, 21);
		LeftElbow = GetBone(Mesh, 22);
		LeftHand = GetBone(Mesh, 23);
		RightUpperArm = GetBone(Mesh, 47);
		RightElbow = GetBone(Mesh, 48);
		RightHand = GetBone(Mesh, 49);
		Pelvis = GetBone(Mesh, 3);
		LeftThigh = GetBone(Mesh, 75);
		LeftKnee = GetBone(Mesh, 76);
		LeftFoot = GetBone(Mesh, 78);
		RightThigh = GetBone(Mesh, 82);
		RightKnee = GetBone(Mesh, 83);
		RightFoot = GetBone(Mesh, 85);
	}

	DrawLineSimple(NeckPos, Pelvis);
	DrawLineSimple(NeckPos, LeftUpperArm);
	DrawLineSimple(LeftUpperArm, LeftElbow);
	DrawLineSimple(LeftElbow, LeftHand);
	DrawLineSimple(NeckPos, RightUpperArm);
	DrawLineSimple(RightUpperArm, RightElbow);
	DrawLineSimple(RightElbow, RightHand);
	DrawLineSimple(Pelvis, LeftThigh);
	DrawLineSimple(LeftThigh, LeftKnee);
	DrawLineSimple(LeftKnee, LeftFoot);
	DrawLineSimple(Pelvis, RightThigh);
	DrawLineSimple(RightThigh, RightKnee);
	DrawLineSimple(RightKnee, RightFoot);
}

struct FString
{
	wchar_t szChar[64];
};

static std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}


auto HACK() -> void
{
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;

	if (settings::aimbot::drawfov)
	{
		DrawCircle(ScreenCenterX, ScreenCenterY, settings::aimbot::fov, &Col.white, 10000);
	}

	if (settings::aimbot::filledfov)
	{
		DrawCircleFilled(ScreenCenterX, ScreenCenterY, settings::aimbot::fov, &Col.filled, 10000);
	}


	if (settings::aimbot::rcscrosshair)
	{
		drawRecoil();
	}

	if (settings::aimbot::nazicrosshair)
	{
		ImColor Red = { 255, 0, 0, 255 };
		DrawCrossNazi(10, Red);
	}

	auto entityListCopy = entityList;
	std::vector<EnemyPtr> local_enemy_collection = enemy_collection;
	for (int i = 0; i < entityListCopy.size(); ++i)
	{

		EnemyPtr enemy = local_enemy_collection[i];
		FNlEntity entity = entityListCopy.at(i);

		auto ActorRootComponent = Read<uintptr_t>(entity.Actor + offsets::RootComponent);
		auto RelativeLocation = Read<Vector3>(ActorRootComponent + offsets::RelativeLocation);

		auto LocalRootComponent = Read<uintptr_t>(LocalPawn + offsets::RootComponent);
		auto LocalRelativeLocation = Read<Vector3>(LocalRootComponent + offsets::RelativeLocation);

		auto MeshComponent = Read<uint64_t>(entity.Actor + offsets::MeshComponent);


		uintptr_t playerstate = Read<uintptr_t>(LocalPawn + 0x3f0);
		uintptr_t teamComponent = Read<uintptr_t>(playerstate + 0x628);
		int teamID = Read<int>(teamComponent + offsets::TeamID);

		Vector3 vBaseBone = GetBone(MeshComponent, 0);
		Vector3 vBaseBoneOut = ProjectWorldToScreen(vBaseBone);
		Vector3 vBaseBoneOut2 = ProjectWorldToScreen(Vector3(vBaseBone.x, vBaseBone.y, vBaseBone.z - 15));
		Vector3 vHeadBone = GetBone(MeshComponent, 8);
		Vector3 vHeadBoneOut = ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z));
		Vector3 vHeadBoneOut2 = ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z + 15));

		auto distance = LocalRelativeLocation.Distance(RelativeLocation);

		float BoxHeight = abs(vHeadBoneOut2.y - vBaseBoneOut.y);
		float BoxWidth = BoxHeight * 0.55;


		FVector camera_position = Read<FVector>(globals::localcameramanager + offsets::camera_position);
		FVector camera_rotation = Read<FVector>(globals::localcameramanager + offsets::camera_rotation);
		float camera_fov = Read<float>(globals::localcameramanager + offsets::camera_fov);


		auto ActorPlayerState = Read<DWORD_PTR>(entity.Actor + offsets::PlayerState);
		uintptr_t ActorTeamComponent = Read<uintptr_t>(ActorPlayerState + offsets::TeamComponent);
		auto ActorTeamID = Read<int>(ActorTeamComponent + offsets::TeamID);

		uintptr_t LocalTeamComponent = Read<uintptr_t>(PlayerState + offsets::TeamComponent);
		int LocalTeamID = Read<int>(LocalTeamComponent + offsets::TeamID);

		bool dormant = Read<bool>(entity.Actor + offsets::bIsDormant);
		auto dmgh = Read<uintptr_t>(entity.Actor + offsets::DamageHandler);
		auto Health = Read<float>(dmgh + offsets::Health);

		auto Shield = Read<float>(dmgh + offsets::Shield);

		auto IsVisible = isVisible(MeshComponent);

		ImColor ESPColor; if (IsVisible) { ESPColor = ImColor(0, 255, 0); }
		else { ESPColor = ImColor(255, 0, 0); }

		auto Inventory = Read<intptr_t>(entity.Actor + offsets::Inventory);
		intptr_t CurrentEquip = Read<intptr_t>(Inventory + offsets::CurrentWeapon);
		int weaponObj = Read<int>(CurrentEquip + offsets::ObjID);
		auto currentweapon = GetWeaponName(weaponObj);

		uintptr_t MagazineAmmo = Read<uintptr_t>(CurrentEquip + 0xFB0);
		int32_t AuthResourceAmount = Read<int32_t>(MagazineAmmo + 0x100);
		int32_t MaxAmmo = Read<int32_t>(MagazineAmmo + 0x11c);
		std::string DisplayAmmoA = "A: " + std::to_string(AuthResourceAmount) + " / " + std::to_string(MaxAmmo);


		FString PlayerName = Read<FString>(Read<uint64_t>(PlayerState + offsets::playername));
		std::string PlayerNameStr = utf8_encode(PlayerName.szChar);

		auto BoneCount = Read<int>(MeshComponent + offsets::BoneCount);

		int charObj1 = Read<int>(entity.Actor + offsets::ObjID);
		auto characterName = GetCharacterName(charObj1);


		float SpikeTimer = Read<int>(entity.Actor + offsets::Spiketime);


		auto CameraManager = Read<uintptr_t>(PlayerController + 0x478);
		auto FieldOfView = Read<float>(CameraManager + 0x1258);
		int fov = 103;

		int Snaplinepos = 0; //top = -1080, bottom = -70, middle = -540;

		if (Health <= 0) continue;
		if (!dormant) continue;

		if (LocalTeamID)
		{
			if (settings::visualsteam::teambox)
			{
				DrawNormalBox(vBaseBoneOut.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, ESPColor);
			}

			if (settings::visualsteam::snaplineteam)
			{


				if (settings::visualsteam::snaplineposteam == 0)
				{
					DrawLine(ImVec2(Width / 2, Height - 1075), ImVec2(vHeadBoneOut2.x, vHeadBoneOut2.y - 30), ESPColor, 0.5f);
				}

				else if (settings::visualsteam::snaplineposteam == 1)
				{
					DrawLine(ImVec2(Width / 2, Height - 540), ImVec2(vBaseBoneOut.x, vBaseBoneOut.y), ESPColor, 0.5f);
				}

				else if (settings::visualsteam::snaplineposteam == 2)
				{
					DrawLine(ImVec2(Width / 2, Height), ImVec2(vBaseBoneOut.x, vBaseBoneOut.y), ESPColor, 0.5f);
				}
			}

			if (settings::visualsteam::skeleton)
			{

				DrawSkeleton(entity.MeshComponent);
			}
		}


		if (Health <= 0) continue;
		if (!dormant) continue;

		if (ActorTeamID != LocalTeamID || BoneCount == 103)
		{
			if (distance < 400)
			{
				if (settings::visuals::snapline)
				{


					if (settings::visuals::snaplinepos == 0)
					{
						DrawLine(ImVec2(Width / 2, Height - 1075), ImVec2(vHeadBoneOut2.x, vHeadBoneOut2.y - 30), ESPColor, 0.5f);
					}

					else if (settings::visuals::snaplinepos == 1)
					{
						DrawLine(ImVec2(Width / 2, Height - 540), ImVec2(vBaseBoneOut.x, vBaseBoneOut.y), ESPColor, 0.5f);
					}

					else if (settings::visuals::snaplinepos == 2)
					{
						DrawLine(ImVec2(Width / 2, Height), ImVec2(vBaseBoneOut.x, vBaseBoneOut.y), ESPColor, 0.5f);
					}
				}

				if (settings::visuals::box)
				{
					DrawNormalBox(vBaseBoneOut.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, ESPColor);
				}

				if (settings::visuals::box3d)
				{
					DrawCorneredBox(vHeadBoneOut2.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, ESPColor, 1.5);
				}


				if (settings::visuals::headcircle)
				{
					if (settings::visuals::headboxtype == 0)
					{
						DrawCircle(vBaseBoneOut.x, vHeadBoneOut.y, BoxHeight / 10, &Col.white, 100);
					}

					else if (settings::visuals::headboxtype == 1)
					{
						DrawCircle(vBaseBoneOut.x, vHeadBoneOut.y, BoxHeight / 10, &Col.white, 100);
						DrawCircleFilled(vBaseBoneOut.x, vHeadBoneOut.y, BoxHeight / 10, &Col.filled, 100);
					}

					else if (settings::visuals::headboxtype == 2)

					{
						DrawCircleFilled(vBaseBoneOut.x, vHeadBoneOut.y, BoxHeight / 20, &Col.white, 100);
						DrawCircle(vBaseBoneOut.x, vHeadBoneOut.y, BoxHeight / 20, &Col.white, 100);
					}

					//DrawCircle(vBaseBoneOut.x, vHeadBoneOut.y, BoxHeight / 10, &Col.white, 100);
				}

				if (settings::visuals::skeleton)
				{

					DrawSkeleton(entity.MeshComponent);
				}


				if (settings::visuals::teamswap)
				{
					teamID = !(bool)teamID;
					Write<int>(teamComponent + offsets::TeamID, teamID);
				}

				if (settings::visuals::nameesp)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(PlayerNameStr.c_str());
					ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 20 - TextSize.y / 2), ImColor({ 255, 0, 0, 255 }), PlayerNameStr.c_str());
				}

				if (settings::visuals::distance)
				{
					//char dist[64];
					//sprintf_s(dist, "[%.fm] ", distance);
					//std::string Out = dist;

					std::string Out = "[" + std::to_string(static_cast<int32_t>(distance)) + "m]";
					ImVec2 TextSize = ImGui::CalcTextSize(Out.c_str());
					ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - TextSize.x / 2, vHeadBoneOut2.y - 20 - TextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), Out.c_str());
				}

				if (settings::visuals::weaponstate)
				{
					uintptr_t InventoryE = Read<uintptr_t>(entity.Actor + 0x948);
					if (InventoryE) {
						uintptr_t CurrentEqippableE = Read<uintptr_t>(InventoryE + 0x218);
						if (CurrentEqippableE) {
							EAresEquippableState CurrentEquippableVFXState = Read<EAresEquippableState>(CurrentEqippableE + 0xCC0);
							std::string Out1 = GetEquippableVFXState(CurrentEquippableVFXState);
							ImVec2 TextSize = ImGui::CalcTextSize(Out1.c_str());
							ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - TextSize.x / 2, vHeadBoneOut2.y - 43 - TextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), Out1.c_str());
						}
					}
				}

				if (settings::visuals::visibleesp)
				{
					if (IsVisible)
					{
						char Text[64];
						sprintf_s(Text, "Visible");
						std::string Out133777 = Text;
						ImVec2 TextSize = ImGui::CalcTextSize(Out133777.c_str());


						ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - TextSize.x / 2, vHeadBoneOut2.y - 30 - TextSize.y / 2), ESPColor, Out133777.c_str());

					}
					else
					{
						char Text[64];
						sprintf_s(Text, "Not Visible");
						std::string Out1337777 = Text;
						ImVec2 TextSize = ImGui::CalcTextSize(Out1337777.c_str());


						ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - TextSize.x / 2, vHeadBoneOut2.y - 30 - TextSize.y / 2), ESPColor, Out1337777.c_str());
					}
				}

				if (settings::visuals::textesp)
				{
					char Text[64];
					sprintf_s(Text, "Virgin");
					std::string Out13377 = Text;
					ImVec2 TextSize = ImGui::CalcTextSize(Out13377.c_str());


					ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y - 20 - TextSize.y / 2), ImGui::GetColorU32({ 239, 0, 255, 255 }), Out13377.c_str());

				}

				if (settings::visuals::rendercount)
				{
					ShowWatermark(&settings::visuals::rendercount, "Player", ImVec4(255, 255, 255, 255));
				}

				if (settings::visuals::filledbox)
				{
					ImColor filled = { 210, 210, 210, 25 };
					DrawFilledRect2(vHeadBoneOut2.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, filled);
				}

				if (settings::visuals::radar) {
					ImColor White = { 255, 255, 255, 255 };

					pRadar.x = (1920 / 2) + 450;
					pRadar.y = 1920 / 2 - 1080 / 2 - 300;
					DrawRect(pRadar.x, pRadar.y, 270, 270, &Col.black, 1);
					DrawBorder(pRadar.x, pRadar.y, 270, 270, 1, &Col.SilverWhite);
					auto radar_posX = pRadar.x + 135;
					auto radar_posY = pRadar.y + 135;
					DrawLine1(radar_posX, radar_posY, radar_posX, radar_posY + 135, &Col.SilverWhite, 1);
					DrawLine1(radar_posX, radar_posY, radar_posX, radar_posY - 135, &Col.SilverWhite, 1);
					DrawLine1(radar_posX, radar_posY, radar_posX + 135, radar_posY, &Col.SilverWhite, 1);
					DrawLine1(radar_posX, radar_posY, radar_posX - 135, radar_posY, &Col.SilverWhite, 1);
					DrawCircleFilled1(radar_posX - 0.5f, radar_posY - 0.5f, 3, &Col.yellow);
					FVector pos = Read<FVector>(entity.MeshComponent + offsets::root_position);
					DrawRadar(pos);
				}



				if (settings::visuals::glow)
				{
					Write<float>(entity.Actor + offsets::FresnelIntensity, 100);
				}



				if (settings::visuals::dorment)
				{
					float last_render_time = Read<float>(entity.MeshComponent + offsets::last_render_time);
					float last_submit_time = Read<float>(entity.MeshComponent + offsets::last_submit_time);
					bool is_visible = last_render_time + 0.06F >= last_submit_time;
					bool dormant = Read<bool>(entity.Actor + offsets::dormant);
					if (!dormant || !is_visible) {
						continue;
					}
				}

				if (settings::visuals::weaponesp)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(currentweapon.c_str());
					ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 20 - TextSize.y / 2), ImColor({ 255, 0, 0, 255 }), currentweapon.c_str());
				}

				if (settings::visuals::ammoesp)
				{
					ImVec2 TextSize = ImGui::CalcTextSize(DisplayAmmoA.c_str());
					ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 35 - TextSize.y / 2), ImColor({ 0, 255, 205, 255 }), DisplayAmmoA.c_str());
				}

				if (settings::visuals::skeleton)
				{
					DrawSkeleton(MeshComponent);
				}


				/*
				if (settings::visuals::spiketimer)
				{



					char text[256];
					ImColor White = { 255, 0, 0, 255 };

					sprintf(text, ("Dropped Spike [%.fm]"), (distance / 100));
					DrawString(ScreenCenterX - (strlen(text) * 7 / 2), ScreenCenterY - 5, White, text);

					/*
					float timer = Read<float>(entity.Actor + offsets::Spiketime);

					if (timer < 0.2)
					{
						auto it = std::find(entityList.begin(), entityList.end(), entity);
						if (it != entityList.end())
						{
							entityList.erase(it);
						}
						continue;
					}

					char c_timer[64];


					float dis = (distance / 100);

					sprintf(c_timer, ("Spike [%.2fs]\n[%.fm]"), timer, dis);

					ImColor yellow = { 255, 255, 0, 255 };
					ImColor red = { 255, 0, 0, 255 };
					DrawString(ScreenCenterX - (strlen(c_timer) * 7 / 2), ScreenCenterY - 5, (timer > 7 ? yellow : red), c_timer);



				}
				*/


				if (settings::visuals::agentesp)
				{

					ImVec2 TextSize1 = ImGui::CalcTextSize(characterName.c_str());

					ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - TextSize1.x / 2, vHeadBoneOut2.y - 10 - TextSize1.y / 4), ImColor({ 255, 222, 0, 255 }), characterName.c_str());
				}




				if (settings::visuals::health)
				{

					int G1 = (255 * Health / 100);
					int R1 = 255 - G1;
					ImColor healthcol = { R1, G1, 0, 255 };

					//DrawRightProgressBar(vBaseBoneOut.x + 40, vHeadBoneOut.y - 20, BoxWidth, BoxHeight, 5, Health);
					//char health[64];
					//sprintf_s(health, "[H: %.f] ", Health);
					//std::string Out69 = health;

					std::string Out69 = "[H: " + std::to_string(static_cast<int32_t>(Health)) + "]";
					ImVec2 TextSize = ImGui::CalcTextSize(Out69.c_str());
					//ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 5 - TextSize.y / 2), healthcol, Out69.c_str());

					if (settings::visuals::healthtype == 0)
					{
						ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 5 - TextSize.y / 2), healthcol, Out69.c_str());
					}

					else if (settings::visuals::healthtype == 1)
					{
						DrawRightProgressBar(vBaseBoneOut.x + 40, vHeadBoneOut.y - 20, BoxWidth, BoxHeight, 5, Health);
					}

					else if (settings::visuals::healthtype == 2)

					{
						DrawRightProgressBar(vBaseBoneOut.x + 40, vHeadBoneOut.y - 20, BoxWidth, BoxHeight, 5, Health);
						ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 5 - TextSize.y / 2), healthcol, Out69.c_str());
					}
				}

				if (settings::visuals::shield)
				{

					DrawRightProgressBar1(vBaseBoneOut.x + 40, vHeadBoneOut.y - 20, BoxWidth, BoxHeight, 5, Shield);
				}

			}


			if (settings::fovchanger)
			{
				Write<float>(CameraManager + 0x3f4, settings::fovchangervalue);
			}

			if (settings::aimbot::rapidfire)
			{
				//Write<float>(Local + 0x1A98, 0.0003f);
			}
		

			/*
			if (settings::visuals::agentesp)
			{
				char ag[64];
				sprintf_s(ag, "%s [%s]", characterName);
				std::string Out1 = ag;
				ImVec2 TextSize = ImGui::CalcTextSize(Out1.c_str());
				ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y - TextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), Out1.c_str());
			}
			*/

			/*
			if (settings::visuals::agentesp)
			{
				char tttt[64];
				sprintf(tttt, ("%s [%s]"), GetCharacter(), GetCurrentWeapon());

				std::string Out1 = tttt;
				ImVec2 TextSize = ImGui::CalcTextSize(Out1.c_str());
				ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y - TextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), Out1.c_str());
			}
			*/




			auto dx = vHeadBoneOut.x - (Width / 2);
			auto dy = vHeadBoneOut.y - (Height / 2);
			auto dist = sqrtf(dx * dx + dy * dy);

			if (dist < settings::aimbot::fov && dist < closestDistance)
			{
				if (settings::aimbot::vischeck)
				{
					if (IsVisible)
					{
						closestDistance = dist;
						closestPawn = entity.Actor;
					}
				}
				else
				{
					closestDistance = dist;
					closestPawn = entity.Actor;
				}
			}
		}
	}

	if (settings::aimbot::aimbot)
	{
		if (closestPawn && GetAsyncKeyState(hotkeys::aimkey) < 0)
		{
			CallAimbot(closestPawn);
		}
	}

}

