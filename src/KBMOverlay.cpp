#include "KBMOverlay.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAKE_CVAR(var, name, description, defaultVal, min, max) \
cvarManager->registerCvar(name, defaultVal, description, true, true, min, true, max).bindTo(var)

void KBMOverlay::onLoad() {
	srand(time(0));
	g_CVarManager = cvarManager.get();
	LOG("KBMOverlay " KBMO_VERSION " loaded.");

	// Make cvars
	MAKE_CVAR(config.size, "kbmo_size", "Size of the overlay", "400", 200, 1000);
	MAKE_CVAR(config.styleIndex, "kbmo_style", "Style of the overlay", "1", 1, IM_ARRAYSIZE(styles));

	// Create hooks
	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&KBMOverlay::hkOnTick, this, std::placeholders::_1));

	// Open menu when started
	gameWrapper->SetTimeout([](GameWrapper* wrapper) {
		g_CVarManager->executeCommand("togglemenu KBMOverlay");
		}, 0);
}

void KBMOverlay::onUnload() {
	// Nothing for now
}

void KBMOverlay::SetImGuiContext(uintptr_t _ctx) {
	auto ctx = (ImGuiContext*)_ctx;
	ImGui::SetCurrentContext(ctx);

	static bool first = true;
	if (first) {
		first = false;

		// Set bigFont as the most-recently-loaded ImGui font
		bigFont = ctx->IO.Fonts->Fonts.back();
	}
}

// Update keys each tick
void KBMOverlay::hkOnTick(string eventName) {
	GetKeyState(0); // Need to make a GetKeyState call before calling GetKeyboardState
	GetKeyboardState(buttonsCache);
	for (byte& k : buttonsCache) {
		k = (k & 128);
	}
}

void FillRect(ImArea area, ImColor color, float rounding = 0) {
	ImGui::GetWindowDrawList()->AddRectFilled(area.min, area.max, color, rounding);
}

void FillText(ImVec2 pos, string str, ImColor color, float size) {
	ImVec2 textSize = ImGui::CalcTextSize(str.c_str());
	pos -= textSize / 2;
	ImGui::GetWindowDrawList()->AddText(pos, color, str.c_str());
}

void KBMOverlay::Render() {
	auto ctx = ImGui::GetCurrentContext();
	if (!ctx)
		return;

	if (!bigFont || !bigFont->IsLoaded()) {
		LOG("Font not loaded, skipping render frame.");
		return;
	}

	ImGui::SetNextWindowSize({ (float)*config.size, roundf(*config.size * 0.6) });
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

	bool isBakkesMenuOpen = gameWrapper->IsCursorVisible();
	if (!isBakkesMenuOpen)
		windowFlags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.75f));

	if (ImGui::Begin("KBMOverlay", &isOpen, windowFlags)) {

		auto& style = styles[*config.styleIndex - 1];

		constexpr float
			PADDING = 4,
			ROUNDING = 8;

		ImArea windowArea = {
			ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin(),
			ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax()
		};
		windowArea.Pad(PADDING);
		ImArea keyboardArea = windowArea.RelSub({ 0, 0 }, { 0.6f, 1.f }).ShrinkToCenterSquare();
		ImArea mouseArea = windowArea.RelSub({ 0.6f, 0 }, { 1.f, 1.f });

		static ImColor keyTextColors[] = { C_WHITE, C_BLACK };

		ImGui::PushFont(bigFont);
		{ // Keys
			constexpr char MAIN_KEYS[] = "QWEASD";
			for (int i = 0; i < 6; i++) {
				int x = i % 3;
				int y = i / 3;
				ImArea keyArea = keyboardArea.Column(x, 3).Row(y, 3).Pad(PADDING);

				bool pressed = buttonsCache[MAIN_KEYS[i]];
				char keyStr[2] = { MAIN_KEYS[i], 0 };
				FillRect(keyArea, style.keyColors[pressed], ROUNDING);
				FillText(keyArea.GetCenter(), keyStr, keyTextColors[pressed], 20);
			}

			ImArea shiftKeyArea = keyboardArea.Row(2, 3).Pad(PADDING);
			bool pressed = buttonsCache[VK_SHIFT];
			FillRect(shiftKeyArea, style.keyColors[pressed], ROUNDING);
			FillText(shiftKeyArea.GetCenter(), "SHIFT", keyTextColors[pressed], 20);
		}


		{ // Mouse
			for (int i = 0; i < 2; i++) {
				ImArea mouseButtonArea = mouseArea.Column(i, 2).Pad(PADDING);
				bool pressed = buttonsCache[VK_LBUTTON + i];
				FillRect(mouseButtonArea, style.keyColors[pressed], ROUNDING);

				ImArea mouseButtonTextArea = mouseButtonArea.Row(4, 5);
				FillText(mouseButtonTextArea.GetCenter(), i ? "R" : "L", keyTextColors[pressed], 20);
			}
		}
		ImGui::PopFont();
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}