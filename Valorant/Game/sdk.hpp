#pragma once

#include <Windows.h>
#include <map>
#include <d3d9types.h>

#include "../Overlay/render.hpp"
#include "../Driver/driver.h"
#include "../../Includes/strings.hpp"
#include "structs.hpp"
#include "globals.hpp"


#include <vector>

namespace Globals
{
	DWORD_PTR PlayerCameraManager, PlayerController;

	namespace camera
	{
		Vector3 CameraLocation, CameraRotation;
		float FovAngle;
	}
}

using namespace Globals;
using namespace camera;

auto GetBone(DWORD_PTR mesh, int id) -> Vector3
{
	DWORD_PTR array = Read<uintptr_t>(mesh + 0x5b0);
	if (array == NULL)
		array = Read<uintptr_t>(mesh + 0x568);

	FTransform bone = Read<FTransform>(array + (id * 0x30));

	FTransform ComponentToWorld = Read<FTransform>(mesh + 0x250);
	D3DMATRIX Matrix;

	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

auto ProjectWorldToScreen(Vector3 WorldLocation) -> Vector3
{
	Vector3 Screenlocation = Vector3(0, 0, 0);

	CameraLocation = Read<Vector3>(PlayerCameraManager + offsets::camera_position);
	CameraRotation = Read<Vector3>(PlayerCameraManager + offsets::camera_rotation);
	float camera_fov = Read<float>(PlayerCameraManager + offsets::camera_fov);

	D3DMATRIX tempMatrix = Matrix(CameraRotation, Vector3(0, 0, 0));

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]), 
			vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]), 
			vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - CameraLocation;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f) vTransformed.z = 1.f;

	FovAngle = camera_fov;

	float ScreenCenterX = Width / 2.0f;
	float ScreenCenterY = Height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}

auto isVisible(DWORD_PTR mesh) -> bool
{
	float fLastSubmitTime = Read<float>(mesh + offsets::last_submit_time);
	float fLastRenderTimeOnScreen = Read<float>(mesh + offsets::last_render_time);

	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;

	return bVisible;
}

Vector3 VectorToRotation(Vector3 relativeLocation)
{
	constexpr auto radToUnrRot = 57.2957795f;

	return Vector3(
		atan2(relativeLocation.z, sqrt((relativeLocation.x * relativeLocation.x) + (relativeLocation.y * relativeLocation.y))) * radToUnrRot,
		atan2(relativeLocation.y, relativeLocation.x) * radToUnrRot,
		0.f);
}

Vector3 AimAtVector(Vector3 targetLocation, Vector3 cameraLocation)
{
	return VectorToRotation(targetLocation - cameraLocation);
}

Vector3 SmoothAngles(Vector3 rot1, Vector3 rot2, float smooth)
{
	Vector3 ret(0, 0, 0);

	ret.x = (rot2.x - rot1.x) / smooth + rot1.x;
	ret.y = (rot2.y - rot1.y) / smooth + rot1.y;
	ret.z = (rot2.z - rot1.z) / smooth + rot1.z;

	return ret;
}

void Aimbotty(Vector3 Target, Vector3 Camera_rotation, float SmoothFactor) 
{
	Vector3 ConvertRotation = Camera_rotation;

	auto ControlRotation = Read<Vector3>(PlayerController + 0x440);
	Vector3 DeltaRotation = ConvertRotation - ControlRotation;

	ConvertRotation = Target - (DeltaRotation * SmoothFactor);

	Vector3 Smoothed = SmoothAngles(Camera_rotation, ConvertRotation, SmoothFactor);

	Write<Vector3>(PlayerController + 0x440, Smoothed);
}



auto CallAimbot(DWORD_PTR entity) -> void
{
	int hitboxid;
	Vector3 Head = GetBone(Read<uint64_t>(entity + 0x430), 8);
	Vector3 neck = GetBone(Read<uint64_t>(entity + 0x430), 7);
	Vector3 body = GetBone(Read<uint64_t>(entity + 0x430), 5);

	if (settings::aimbot::selectedhitbox == 2)
	{
		AimAtVector(body, CameraLocation);
	}

	Vector3 angles_out = AimAtVector(neck, CameraLocation);

	#include "cheat.hpp"


	if (settings::aimbot::selectedhitbox == 0)
	{
		Vector3 angles_out = AimAtVector(Head, CameraLocation);
	}
	else if (settings::aimbot::selectedhitbox == 1)
	{
		Vector3 angles_out = AimAtVector(neck, CameraLocation);
	}
	else if (settings::aimbot::selectedhitbox == 2)
	{
		Vector3 angles_out = AimAtVector(body, CameraLocation);
	}
	/*
	if (settings::aimbot::silent)
	{
		Write<Vector3>(LocalPawn + 0x14C + 0x580 + 0xC, neck.x);
	}
	*/
	if (settings::aimbot::smooth)
	{
		Aimbotty(angles_out, CameraRotation, settings::aimbot::smoothness);
	}
	else Write<Vector3>(PlayerController + 0x440, angles_out);
}



#include "defs.h"

uintptr_t decryptWorld(uintptr_t base_address)
{
	const auto key = Read<uintptr_t>(image_base + offsets::World_Key);
	const auto state = Read<State>(image_base + offsets::World_State);
	const auto uworld_ptr = decrypt_uworld(key, (uintptr_t*)&state);
	return Read<uintptr_t>(uworld_ptr);
}