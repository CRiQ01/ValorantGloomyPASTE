#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>

#include "../../Includes/Imgui/imgui.h"
#include "../../Includes/Imgui/imgui_impl_win32.h"
#include "../../Includes/Imgui/imgui_impl_dx9.h"
#include "../game/globals.hpp"
#include "../Game/structs.hpp"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "Dwmapi.lib")

IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

HWND MyWnd = NULL;
HWND GameWnd = NULL;
MSG Message = { NULL };

RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;

DWORD ScreenCenterX;
DWORD ScreenCenterY;

static ULONG Width = GetSystemMetrics(SM_CXSCREEN);
static ULONG Height = GetSystemMetrics(SM_CYSCREEN);

WPARAM main_loop();
void render();

class FVector {
public:
	FVector() : x(0.f), y(0.f), z(0.f) {}
	FVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~FVector() {}

	float x;
	float y;
	float z;

	inline float Dot(FVector v) {
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(FVector v) {
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	FVector operator+(FVector v) {
		return FVector(x + v.x, y + v.y, z + v.z);
	}

	FVector operator-(FVector v) {
		return FVector(x - v.x, y - v.y, z - v.z);
	}
	FVector operator/(float flDiv) {
		return FVector(x / flDiv, y / flDiv, z / flDiv);
	}

	FVector operator*(float Scale) {

		return FVector(x * Scale, y * Scale, z * Scale);
	}
	inline float Length() {
		return sqrtf((x * x) + (y * y) + (z * z));
	}
	FVector operator-=(FVector v) {

		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
};
struct FVector2D {
public:
	float x;
	float y;

	inline FVector2D() : x(0), y(0) {}
	inline FVector2D(float x, float y) : x(x), y(y) {}

	inline float Distance(FVector2D v) {
		return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
	}

	inline FVector2D operator+(const FVector2D& v) const {
		return FVector2D(x + v.x, y + v.y);
	}

	inline FVector2D operator-(const FVector2D& v) const {
		return FVector2D(x - v.x, y - v.y);
	}
};

float RadianToDegree(float radian)
{
	return radian * (180 / M_PI);
}

float DegreeToRadian(float degree)
{
	return degree * (M_PI / 180);

}

FVector RadianToDegree(FVector radians)
{
	FVector degrees;
	degrees.x = radians.x * (180 / M_PI);
	degrees.y = radians.y * (180 / M_PI);
	degrees.z = radians.z * (180 / M_PI);
	return degrees;
}

FVector DegreeToRadian(FVector degrees)
{
	FVector radians;
	radians.x = degrees.x * (M_PI / 180);
	radians.y = degrees.y * (M_PI / 180);
	radians.z = degrees.z * (M_PI / 180);
	return radians;
}

typedef struct {
	uintptr_t actor_ptr;
	uintptr_t damage_handler_ptr;
	uintptr_t player_state_ptr;
	uintptr_t root_component_ptr;
	uintptr_t mesh_ptr;
	uintptr_t bone_array_ptr;
	int bone_count;
} EnemyPtr;

FVector getBonePosition(EnemyPtr enemy, int index)
{
	size_t size = sizeof(FTransform);
	FTransform firstBone = Read<FTransform>(enemy.bone_array_ptr + (size * index));
	FTransform componentToWorld = Read<FTransform>(enemy.mesh_ptr + offsets::component_to_world);
	D3DMATRIX matrix = MatrixMultiplication(firstBone.ToMatrixWithScale(), componentToWorld.ToMatrixWithScale());
	return FVector(matrix._41, matrix._42, matrix._43);
}

D3DMATRIX toMatrix(FVector Rotation, FVector origin = FVector(0, 0, 0))
{
	float Pitch = (Rotation.x * float(M_PI) / 180.f);
	float Yaw = (Rotation.y * float(M_PI) / 180.f);
	float Roll = (Rotation.z * float(M_PI) / 180.f);

	float SP = sinf(Pitch);
	float CP = cosf(Pitch);
	float SY = sinf(Yaw);
	float CY = cosf(Yaw);
	float SR = sinf(Roll);
	float CR = cosf(Roll);

	D3DMATRIX Matrix;
	Matrix._11 = CP * CY;
	Matrix._12 = CP * SY;
	Matrix._13 = SP;
	Matrix._14 = 0.f;

	Matrix._21 = SR * SP * CY - CR * SY;
	Matrix._22 = SR * SP * SY + CR * CY;
	Matrix._23 = -SR * CP;
	Matrix._24 = 0.f;

	Matrix._31 = -(CR * SP * CY + SR * SY);
	Matrix._32 = CY * SR - CR * SP * SY;
	Matrix._33 = CR * CP;
	Matrix._34 = 0.f;

	Matrix._41 = origin.x;
	Matrix._42 = origin.y;
	Matrix._43 = origin.z;
	Matrix._44 = 1.f;

	return Matrix;
}

FVector2D worldToScreen(FVector world_location, FVector position, FVector rotation, float fov)
{
	FVector2D screen_location = FVector2D(0, 0);

	D3DMATRIX tempMatrix = toMatrix(rotation);

	FVector vAxisX, vAxisY, vAxisZ;

	vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	FVector vDelta = world_location - position;
	FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < .1f)
		vTransformed.z = .1f;

	float FovAngle = fov;
	float ScreenCenterX = 1920 / 2.0f;
	float ScreenCenterY = 1080 / 2.0f;

	screen_location.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	screen_location.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return screen_location;
}

auto init_wndparams(HWND hWnd) -> HRESULT
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device))) {
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGui::GetIO().Fonts->AddFontDefault();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = NULL;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 13);
	ImGuiStyle* style = &ImGui::GetStyle();
	
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);
	return S_OK;
}

auto get_process_wnd(uint32_t pid) -> HWND
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
		auto pParams = (std::pair<HWND, uint32_t>*)(lParam);
		uint32_t processId = 0;

		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processId)) && processId == pParams->second) {
			SetLastError((uint32_t)-1);
			pParams->first = hwnd;
			return FALSE;
		}

		return TRUE;

		}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
		return params.first;

	return NULL;
}

auto cleanup_d3d() -> void
{
	if (p_Device != NULL) {
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL) {
		p_Object->Release();
	}
}

auto set_window_target() -> void
{
	while (true) {
		GameWnd = get_process_wnd(process_id);
		if (GameWnd) {
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(GameWnd, &GameRect);
			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
	}
}

auto setup_window() -> void 
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)set_window_target, 0, 0, 0);
	
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("AVeryExternalOverlayx"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyWnd = CreateWindowExA(NULL, ("AVeryExternalOverlayx"), ("AVeryExternalOverlayx"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyWnd, &margin);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);
}

void DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), 0, 0);
}

/*
LPD3DXLINE xwwwwwwqq = NULL;
void DrawLine2(float StartX, float StartY, float EndX, float EndY, D3DCOLOR dColor, float thicc)
{
	xwwwwwwqq[0].SetWidth(thicc);
	xwwwwwwqq[0].SetGLLines(0);
	xwwwwwwqq[0].SetAntialias(1);
	D3DXVECTOR2 v2Line[2];
	v2Line[0].x = StartX;
	v2Line[0].y = StartY;
	v2Line[1].x = EndX;
	v2Line[1].y = EndY;

	xwwwwwwqq[0].Begin();
	xwwwwwwqq[0].Draw(v2Line, 2, dColor);
	xwwwwwwqq[0].End();
}


void DrawSkeleton(uintptr_t Mesh, bool IsMale, DWORD col)
{
	int skeleton[][4] = {
		{ 7, 8, 0, 0},
		{ 7, IsMale ? 47 : 46, IsMale ? 48 : 47, IsMale ? 49 : 48 },
		{ 7, 21, 22, 23 },
		{ 7, 5, 4, 3 },
		{ 3, IsMale ? 82 : 80, IsMale ? 83 : 81, IsMale ? 85 : 83 },
		{ 3, IsMale ? 75 : 73, IsMale ? 76 : 74, IsMale ? 78 : 76 }
	};

	for (auto part : skeleton)
	{
		Vector3 previous{};
		for (int i = 0; i < 4; i++)
		{
			if (!part[i]) break;

			Vector3 current = GetBone(Mesh, part[i]);
			if (previous.x == 0.f)
			{
				previous = current;
				continue;
			}
			Vector3 p1{}, c1{};
			p1 = ProjectWorldToScreen(previous);
			c1 = ProjectWorldToScreen(current);
			DrawLine2(p1.x, p1.y, c1.x, c1.y, col, 1.5f);
			previous = current;
		}
	}
}
*/


void DrawFilledRect2(int x, int y, int w, int h, ImColor color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f);
}

static void DrawRect(int x, int y, int w, int h, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
}

void DrawBorder(float x, float y, float w, float h, float px, RGBA* BorderColor)
{
	DrawRect(x, (y + h - px), w, px, BorderColor, 1 / 2);
	DrawRect(x, y, px, h, BorderColor, 1 / 2);
	DrawRect(x, y, w, px, BorderColor, 1 / 2);
	DrawRect((x + w - px), y, px, h, BorderColor, 1 / 2);
}

void DrawRightProgressBar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	DrawFilledRect(x + (w / 2) - 25, y, thick, (h)*m_health / 100, &healthcol);
}

void DrawRightProgressBar1(int x, int y, int w, int h, int thick, int m_shield)
{
	int G = (0 * m_shield / 100);
	int R = 77 - G;
	RGBA shieldcolor = { R, G, 255, 255 };

	DrawFilledRect(x + (w / 2) - 25, y, thick, (h)*m_shield / 100, &shieldcolor);
}

void DrawNormalBox(int x, int y, int w, int h, int borderPx, ImColor color)
{
	ImColor White = { 255.0f, 255.0f, 255.0f, 1.0f };
	DrawFilledRect2(x + borderPx, y, w, borderPx, White);
	DrawFilledRect2(x + w - w + borderPx, y, w, borderPx, White);
	DrawFilledRect2(x, y, borderPx, h, White);
	DrawFilledRect2(x, y + h - h + borderPx * 2, borderPx, h, White);
	DrawFilledRect2(x + borderPx, y + h + borderPx, w, borderPx, White);
	DrawFilledRect2(x + w - w + borderPx, y + h + borderPx, w, borderPx, White);
	DrawFilledRect2(x + w + borderPx, y, borderPx, h, White);
	DrawFilledRect2(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, White);
}

void renderBoneLine(FVector first_bone_position, FVector second_bone_position, FVector position, FVector rotation, float fov)
{
	FVector2D first_bone_screen_position = worldToScreen(first_bone_position, position, rotation, fov);
	ImVec2 fist_screen_position = ImVec2(first_bone_screen_position.x, first_bone_screen_position.y);
	FVector2D second_bone_screen_position = worldToScreen(second_bone_position, position, rotation, fov);
	ImVec2 second_screen_position = ImVec2(second_bone_screen_position.x, second_bone_screen_position.y);
	ImGui::GetOverlayDrawList()->AddLine(fist_screen_position, second_screen_position, ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f)));
}

void ShowWatermark(bool* OpenWindows, const char* Text, ImVec4 Color)
{
	const float Distance = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	if (io.MousePos.x < 900.0f && io.MousePos.y < 800.0f)
	{
		if (corner != -1)
		{
			WindowFlags |= ImGuiWindowFlags_NoMove;
			ImVec2 WindowPosPivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
			ImVec2 WindowPos = ImVec2((corner & 1) ? io.DisplaySize.x - Distance : Distance, (corner & 2) ? io.DisplaySize.y - Distance : Distance);
			ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always, WindowPosPivot);

		}
		ImGui::SetNextWindowBgAlpha(0.05f);
		if (ImGui::Begin("Amount of Players Rendering", OpenWindows, WindowFlags))
		{
			ImGui::TextColored(Color, Text);
			ImGui::End();
		}
	}
	else
	{
		if (corner != -1)
		{
			WindowFlags |= ImGuiWindowFlags_NoMove;
			ImVec2 WindowPosPivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
			ImVec2 WindowPos = ImVec2((corner & 1) ? io.DisplaySize.x - Distance : Distance, (corner & 2) ? io.DisplaySize.y - Distance : Distance);
			ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always, WindowPosPivot);

		}
		ImGui::SetNextWindowBgAlpha(0.35f);
		if (ImGui::Begin("Watermark", OpenWindows, WindowFlags))
		{
			ImGui::TextColored(Color, Text);
			ImGui::End();
		}
	}
}


void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness) 
{
	ImColor Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	ImColor White = { 226.0f, 92.0f, 92.0f, 1.0f };

	float lineW = (W / 3);
	float lineH = (H / 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImColor(Black), thickness + 2);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImColor(Black), thickness + 2);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImColor(White), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImColor(White), thickness);
}
auto DrawLine(const ImVec2& x, const ImVec2 y, ImU32 color, const FLOAT width) -> void
{
	ImGui::GetOverlayDrawList()->AddLine(x, y, color, width);
}

void DrawLine1(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

void DrawCircleFilled1(int x, int y, int radius, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

auto Draw2DBox(float x, float y, float w, float h, ImColor color)-> void
{
	DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.3f); // top 
	DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + 1.4f), color, 1.3f); // left
	DrawLine(ImVec2(x + w, y - 1.3f), ImVec2(x + w, y + h + 1.4f), color, 1.3f);  // right
	DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.3f);   // bottom 
}

auto RectFilled(float x, float y, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)-> void
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

static FVector pRadar;

static int enemy_size = 3;
void DrawCircleFilled2(int x, int y, int radius, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}
FVector2D WorldRadar(FVector srcPos, FVector distPos, float yaw, float radarX, float radarY, float size)
{
	auto cosYaw = cos(DegreeToRadian(yaw));
	auto sinYaw = sin(DegreeToRadian(yaw));

	auto deltaX = srcPos.x - distPos.x;
	auto deltaY = srcPos.y - distPos.y;

	auto locationX = (float)(deltaY * cosYaw - deltaX * sinYaw) / 45.f;
	auto locationY = (float)(deltaX * cosYaw + deltaY * sinYaw) / 45.f;

	if (locationX > (size - 2.f))
		locationX = (size - 2.f);
	else if (locationX < -(size - 2.f))
		locationX = -(size - 2.f);

	if (locationY > (size - 6.f))
		locationY = (size - 6.f);
	else if (locationY < -(size - 6.f))
		locationY = -(size - 6.f);

	return FVector2D((int)(-locationX + radarX), (int)(locationY + radarY));
}
/*
void Draw3dBox1(int x, int y, int w, int h, ImColor color)
{
	auto hc11 = ProjectWorldToScreen(Vector3(vhead.x - 50, headBone.y, headBone.z + 25));
	auto hc22 = ProjectWorldToScreen(Vector3(headBone.x, headBone.y - 50, headBone.z + 25));
	auto hc33 = ProjectWorldToScreen(Vector3(headBone.x + 50, headBone.y, headBone.z + 25));
	auto hc44 = ProjectWorldToScreen(Vector3(headBone.x, headBone.y + 50, headBone.z + 25));
	auto hc11c = ProjectWorldToScreen(Vector3(rootBone.x - 50, rootBone.y, rootBone.z - 10));
	auto hc22c = ProjectWorldToScreen(Vector3(rootBone.x, rootBone.y - 50, rootBone.z - 10));
	auto hc33c = ProjectWorldToScreen(Vector3(rootBone.x + 50, rootBone.y, rootBone.z - 10));
	auto hc44c = ProjectWorldToScreen(Vector3(rootBone.x, rootBone.y + 50, rootBone.z - 10));
	DrawLine(hc33.x, hc33.y, hc22.x, hc22.y, 3dColor, 1);
	DrawLine(hc33.x, hc33.y, hc44.x, hc44.y, 3dColor, 1);
	DrawLine(hc11.x, hc11.y, hc22.x, hc22.y, 3dColor, 1);
	DrawLine(hc11.x, hc11.y, hc44.x, hc44.y, 3dColor, 1);
	DrawLine(hc33c.x, hc33c.y, hc22c.x, hc22c.y, 3dColor, 1);
	DrawLine(hc33c.x, hc33c.y, hc44c.x, hc44c.y, 3dColor, 1);
	DrawLine(hc11c.x, hc11c.y, hc22c.x, hc22c.y, 3dColor, 1);
	DrawLine(hc11c.x, hc11c.y, hc44c.x, hc44c.y, 3dColor, 1);
	DrawLine(hc33.x, hc33.y, hc33c.x, hc33c.y, 3dColor, 1);
	DrawLine(hc11.x, hc11.y, hc11c.x, hc11c.y, 3dColor, 1);
	DrawLine(hc22.x, hc22.y, hc22c.x, hc22c.y, 3dColor, 1);
	DrawLine(hc44.x, hc44.y, hc44c.x, hc44c.y, 3dColor, 1);
}
*/

class CameraStruct
{
public:
	Vector3 Position; //0x1240 
	Vector3 Rotation; //0x124C 
	float Fov; //0x1258 
};

CameraStruct inline GetCameraCache()
{
	intptr_t camera_cache = Read<intptr_t>(globals::localplayercontroller + offsets::PlayerCameraManager);
	auto Camera = Read<CameraStruct>(camera_cache + offsets::camera_cache);
	return Camera;
}

void DrawRadar(FVector EntityPos)
{
	auto radar_posX = pRadar.x + 135;
	auto radar_posY = pRadar.y + 135;
	if (globals::localplayerpawn == 0) return;
	uint64_t LocalRootComp = Read<uint64_t>(globals::localplayerpawn + offsets::RootComponent);
	FVector LocalPos = Read<FVector>(LocalRootComp + offsets::root_position);
	auto Radar2D = WorldRadar(LocalPos, EntityPos, GetCameraCache().Rotation.y, radar_posX, radar_posY, 135.f);
	DrawCircleFilled2(Radar2D.x, Radar2D.y, 4, &Col.red);
}

auto DrawHealthBar(float x, float y, float w, float h, int phealth, bool Outlined)-> void
{
	auto vList = ImGui::GetOverlayDrawList();

	int healthValue = max(0, min(phealth, 100));

	ImColor barColor = ImColor
	(
		min(510 * (100 - healthValue) / 100, 255), min(510 * healthValue / 100, 255),
		25,
		255
	);
	if (Outlined)
		vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

	RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);
}

void DrawCircleFilled(int x, int y, int radius, RGBA* color, float segments)
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), segments);
}


void DrawCircle(int x, int y, int radius, RGBA* color, float segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), segments);
}

std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}

std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

LPD3DXFONT font;

void DrawString(int x, int y, DWORD color, const char* fmt, DWORD TRPE = DT_NOCLIP)
{
	RECT FontPos = { x, y, x + 120, y + 20 };
	font->DrawTextA(NULL, fmt, -1, &FontPos, TRPE, color);
}

void DrawLString(float fontSize, int x, int y, ImU32 color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 2;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), color, text.c_str());
}
