#pragma once
#include "../../Includes/strings.hpp"


namespace hotkeys
{
	int aimkey;
	int airstuckey;
	int instares;
}

static int keystatus = 0;
static int realkey = 0;

bool GetKey(int key)
{
	realkey = key;
	return true;
}
void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				hotkeys::aimkey = i;
				keystatus = 0;
				return;
			}
		}
	}
}

static const char* keyNames[] =
{
	"Press any key..",
	"Left Mouse",
	"Right Mouse",
	"Cancel",
	"Middle Mouse",
	"Mouse 5",
	"Mouse 4",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
};

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = "Select Key";
	else
		aimkeys = preview_value;

	if (status == 1)
	{

		aimkeys = "Press the Key";
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

std::string space = " ";
static int tab = 0;
auto drawmenu() -> void
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		settings::show_menu = !settings::show_menu;
	}


	ImColor White = { 255.0f, 255.0f, 255.0f, 1.0f };
	//ImGui::GetOverlayDrawList()->AddText(ImVec2(890, 940), White, "PASTERS COMMUNITY LEAK BY CRiQ#6362", 0);

	if (settings::show_menu)
	{
		ImGuiIO io = ImGui::GetIO();
		std::string Name = XorStr("Valorant").c_str();
		ImGui::Begin(Name.c_str(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize(Name.c_str(), ImVec2(300, 390));
		ImGui::Spacing();
		ImGui::Text("Overlay: %.2f Frametime", 20000 / io.Framerate);
		ImGui::Spacing();
		ImGui::Separator();

		ImGui::Spacing();
		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();
		if (ImGui::Button("Aimbot"))
		{
			tab = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Visuals"))
		{
			tab = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Visuals Team"))
		{
			tab = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Misc"))
		{
			tab = 3;
		}
		ImGui::Separator();



		if (tab == 0)
		{
			ImGui::Spacing();
			ImGui::Checkbox(XorStr("Memory Aimbot").c_str(), &settings::aimbot::aimbot);
			if (settings::aimbot::aimbot) settings::aimbot::silent = false;
			ImGui::Checkbox(XorStr("Silent Aimbot").c_str(), &settings::aimbot::silent);
			if (settings::aimbot::silent) settings::aimbot::aimbot = false;
			ImGui::Combo(XorStr("Hitbox").c_str(), &settings::aimbot::selectedhitbox, settings::aimbot::hitbox, IM_ARRAYSIZE(settings::aimbot::hitbox));
			ImGui::Text(XorStr("Aimbot Key: ").c_str());
			ImGui::SameLine();
			HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus);
			ImGui::Spacing();
			ImGui::Checkbox(XorStr("Nazi Crosshair").c_str(), &settings::aimbot::nazicrosshair);
			ImGui::Checkbox(XorStr("Visiblity Check").c_str(), &settings::aimbot::vischeck);
				ImGui::Checkbox(XorStr("Smoothing").c_str(), &settings::aimbot::smooth);
				ImGui::Checkbox(XorStr("Show FOV Circle").c_str(), &settings::aimbot::drawfov);
				ImGui::Checkbox(XorStr("Filled FOV").c_str(), &settings::aimbot::filledfov);
				ImGui::Spacing();
				ImGui::Text(XorStr("FOV Size").c_str());
				ImGui::SliderFloat("    ", &settings::aimbot::fov, 10, 900);
				ImGui::Text(XorStr("Smoothing ").c_str());
				ImGui::SliderFloat("  ", &settings::aimbot::smoothness, 1, 100);
		}
		else if (tab == 1)
		{
			ImGui::Spacing();


			ImGui::Checkbox(XorStr("Box ESP").c_str(), &settings::visuals::box);
			if (settings::visuals::box) settings::visuals::box3d = false;


			ImGui::Checkbox(XorStr("Corner Box ESP").c_str(), &settings::visuals::box3d);
			if (settings::visuals::box3d) settings::visuals::box = false;

			ImGui::Checkbox(XorStr("Filled Box ESP").c_str(), &settings::visuals::filledbox);
			ImGui::Checkbox(XorStr("Snaplines").c_str(), &settings::visuals::snapline);
			if (settings::visuals::snapline == true)
			{
				ImGui::Combo(XorStr("Snapline Pos").c_str(), &settings::visuals::snaplinepos, settings::visuals::snappos, IM_ARRAYSIZE(settings::visuals::snappos));
			}
			ImGui::Checkbox(XorStr("Health ESP").c_str(), &settings::visuals::health);
			if (settings::visuals::health == true)
			{
				ImGui::Combo(XorStr("Health Type").c_str(), &settings::visuals::healthtype, settings::visuals::healtht, IM_ARRAYSIZE(settings::visuals::healtht));
			}
			ImGui::Checkbox(XorStr("HeadCircle ESP").c_str(), &settings::visuals::headcircle);
			if (settings::visuals::headcircle == true)
			{
				ImGui::Combo(XorStr("HeadCircle Type").c_str(), &settings::visuals::headboxtype, settings::visuals::headboxt, IM_ARRAYSIZE(settings::visuals::headboxt));
			}
			ImGui::Checkbox(XorStr("Weapon State ESP").c_str(), &settings::visuals::weaponstate);
			ImGui::Checkbox(XorStr("Visible ESP").c_str(), &settings::visuals::visibleesp);
			ImGui::Checkbox(XorStr("Distance ESP").c_str(), &settings::visuals::distance);
			ImGui::Checkbox(XorStr("Glow ESP").c_str(), &settings::visuals::glow);
			ImGui::Checkbox(XorStr("Skeleton ESP").c_str(), &settings::visuals::skeleton);
			ImGui::Checkbox(XorStr("Agent ESP").c_str(), &settings::visuals::agentesp); //still in the works
			ImGui::Checkbox(XorStr("Weapon ESP").c_str(), &settings::visuals::weaponesp); //still in the works
			ImGui::Checkbox(XorStr("Text ESP").c_str(), &settings::visuals::textesp);
			ImGui::Checkbox(XorStr("Ammo ESP").c_str(), &settings::visuals::ammoesp);
			//ImGui::Checkbox(XorStr("Shield ESP").c_str(), &settings::visuals::shield);
			//ImGui::Checkbox(XorStr("Weapon Status ESP").c_str(), &settings::visuals::spiketimer);
			//ImGui::Checkbox(XorStr("Spike ESP").c_str(), &settings::visuals::spiketimer);
		}
		else if (tab == 2)
		{
			ImGui::Spacing();
			ImGui::Checkbox(XorStr("Box ESP").c_str(), &settings::visualsteam::teambox);
			ImGui::Checkbox(XorStr("Snaplines").c_str(), &settings::visualsteam::snaplineteam);
			if (settings::visualsteam::snaplineteam == true)
			{
				ImGui::Combo(XorStr("Snapline Pos").c_str(), &settings::visualsteam::snaplineposteam, settings::visualsteam::snapposteam, IM_ARRAYSIZE(settings::visualsteam::snapposteam));
			}
			ImGui::Checkbox(XorStr("Skeleton ESP").c_str(), &settings::visualsteam::skeleton);


		}
		else if (tab == 3)
		{
			ImGui::Checkbox(XorStr("Rendering Player Count").c_str(), &settings::visuals::rendercount);
			ImGui::Checkbox(XorStr("Radar").c_str(), &settings::visuals::radar);
			ImGui::Checkbox(XorStr("FOV Changer").c_str(), &settings::fovchanger);
			ImGui::SliderFloat(XorStr("Value").c_str(), &settings::fovchangervalue, 60, 260);
			ImGui::Spacing();
			if (ImGui::Button("Team Swap"))
			{
				settings::visuals::teamswap = true;
			}
			if (settings::visuals::teamswap == true)
			{
				ImGui::SameLine();
				ImGui::Text("ON");
			}
			else if (settings::visuals::teamswap == false)
			{
				ImGui::SameLine();
				ImGui::Text("OFF");
			}

			ImGui::Spacing();
			if (ImGui::Button("Discord"))
			{
				//system("start ");
			}
		}

		// visuals



		//aimbot 
	
		ImGui::End();

	}
}