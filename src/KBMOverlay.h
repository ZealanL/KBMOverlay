#include "Framework.h"

#include "imgui/imarea.h"

class KBMOverlay : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginWindow {
public:
	struct Style {
		ImColor keyColors[2];

		Style(ImColor keyColorIdle, ImColor keyColorPressed) {
			keyColors[0] = keyColorIdle;
			keyColors[1] = keyColorPressed;
		}
	};

	const Style styles[5] = {
		// Yellow
		{ C_DARKGRAY, ImColor(255, 240, 90, 255) },

		// Red
		{ C_DARKGRAY, ImColor(255, 110, 110, 255) },

		// Blue
		{ C_DARKGRAY, ImColor(110, 158, 255, 255) },

		// Green
		{ C_DARKGRAY, ImColor(126, 255, 87, 255) },

		// White
		{ C_DARKGRAY, C_WHITE }
	};

	struct Config {
		std::shared_ptr<int> size = std::make_shared<int>(400);
		std::shared_ptr<int> styleIndex = std::make_shared<int>(1);
	};
	Config config;

	byte buttonsCache[0x100] = {};

	ImFont* bigFont = NULL;
	bool isOpen = true;

	void hkOnTick(string eventName);

	//void hkOnRender(CanvasWrapper canvas);

	virtual void onLoad();
	virtual void onUnload();

	virtual string GetMenuName() { return "KBMOverlay"; };
	virtual string GetMenuTitle() { return "KBMOverlay"; };
	virtual void SetImGuiContext(uintptr_t _ctx) override;

	virtual bool ShouldBlockInput() { return false; }
	virtual bool IsActiveOverlay() { return false; }

	virtual void OnOpen() {
		isOpen = true;
	}

	virtual void OnClose() {
		isOpen = false;
	}

	virtual void Render();
};

BAKKESMOD_PLUGIN(KBMOverlay, "KBMOverlay", KBMO_VERSION, PLUGINTYPE_FREEPLAY)