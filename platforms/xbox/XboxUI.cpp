/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2024 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "XboxUI.h"

#include "GlobalTimer.h"
#include "Interface.h"

#include "GUI/GUIFactory.h"
#include "GUI/Label.h"
#include "GUI/Progressbar.h"
#include "Logging/Logging.h"
#include "Video/Video.h"

namespace GemRB {

bool XboxUI::initialized = false;
bool XboxUI::crtOptimized = false;
float XboxUI::crtSafeZone = 0.85f;
int XboxUI::minimumFontSize = 14;
float XboxUI::uiScale = 1.2f;
bool XboxUI::loadingScreenVisible = false;
Window* XboxUI::loadingWindow = nullptr;
Window* XboxUI::errorWindow = nullptr;
tick_t XboxUI::lastFrameTime = 0;
int XboxUI::targetFPS = 64;

void XboxUI::Initialize()
{
	if (initialized) return;

	Log(MESSAGE, "XboxUI", "Initializing Xbox UI optimizations");

	OptimizeForCRT();
	SetTargetFrameRate(64); // Xbox target

	CreateLoadingWindow();
	CreateErrorWindow();

	initialized = true;
}

void XboxUI::Shutdown()
{
	if (!initialized) return;

	Log(MESSAGE, "XboxUI", "Shutting down Xbox UI");

	HideLoadingScreen();
	delete loadingWindow;
	delete errorWindow;

	initialized = false;
}

void XboxUI::OptimizeForCRT()
{
	Log(MESSAGE, "XboxUI", "Optimizing UI for CRT TV display");

	// Set safe zone for CRT overscan
	SetCRTSafeZone(0.85f);

	// Increase font sizes for TV viewing distance
	AdjustFontSizesForTV();

	// Scale UI elements for better visibility
	SetUIScale(1.3f);

	crtOptimized = true;
}

void XboxUI::SetCRTSafeZone(float percentage)
{
	crtSafeZone = percentage;
	Log(MESSAGE, "XboxUI", "Set CRT safe zone to %.1f%%", percentage * 100.0f);
}

void XboxUI::AdjustFontSizesForTV()
{
	// Increase minimum font size for TV readability
	minimumFontSize = 16;
	Log(MESSAGE, "XboxUI", "Adjusted font sizes for TV viewing");
}

void XboxUI::SetMinimumFontSize(int size)
{
	minimumFontSize = size;
}

void XboxUI::ShowLoadingScreen(const String& message, float progress)
{
	if (!loadingWindow) {
		CreateLoadingWindow();
	}

	// Update loading message
	Label* messageLabel = static_cast<Label*>(loadingWindow->GetControl("MESSAGE"));
	if (messageLabel) {
		messageLabel->SetText(message);
	}

	// Update progress bar if progress specified
	if (progress >= 0.0f) {
		Progressbar* progressBar = static_cast<Progressbar*>(loadingWindow->GetControl("PROGRESS"));
		if (progressBar) {
			progressBar->SetProgress(progress);
		}
	}

	loadingWindow->SetVisible(true);
	loadingScreenVisible = true;

	Log(DEBUG, "XboxUI", "Showing loading screen: %s", message.c_str());
}

void XboxUI::HideLoadingScreen()
{
	if (loadingWindow) {
		loadingWindow->SetVisible(false);
	}
	loadingScreenVisible = false;

	Log(DEBUG, "XboxUI", "Hiding loading screen");
}

void XboxUI::UpdateLoadingProgress(float progress, const String& message)
{
	if (!loadingScreenVisible) return;

	if (!message.empty()) {
		Label* messageLabel = static_cast<Label*>(loadingWindow->GetControl("MESSAGE"));
		if (messageLabel) {
			messageLabel->SetText(message);
		}
	}

	Progressbar* progressBar = static_cast<Progressbar*>(loadingWindow->GetControl("PROGRESS"));
	if (progressBar) {
		progressBar->SetProgress(progress);
	}
}

void XboxUI::ShowErrorMessage(const String& title, const String& message, bool blocking)
{
	if (!errorWindow) {
		CreateErrorWindow();
	}

	Label* titleLabel = static_cast<Label*>(errorWindow->GetControl("TITLE"));
	if (titleLabel) {
		titleLabel->SetText(title);
	}

	Label* messageLabel = static_cast<Label*>(errorWindow->GetControl("MESSAGE"));
	if (messageLabel) {
		messageLabel->SetText(message);
	}

	errorWindow->SetVisible(true);

	Log(ERROR, "XboxUI", "Error: %s - %s", title.c_str(), message.c_str());

	if (blocking) {
		// Wait for user input to dismiss
		// This would typically wait for controller input
	}
}

void XboxUI::ShowWarningMessage(const String& message, uint32_t duration)
{
	Log(WARNING, "XboxUI", "Warning: %s", message.c_str());
	// Implementation would show a temporary warning overlay
}

void XboxUI::ShowInfoMessage(const String& message, uint32_t duration)
{
	Log(MESSAGE, "XboxUI", "Info: %s", message.c_str());
	// Implementation would show a temporary info overlay
}

void XboxUI::ShowSplashScreen()
{
	Log(MESSAGE, "XboxUI", "Showing Xbox splash screen");
	// Implementation would show Xbox-optimized splash screen
}

void XboxUI::HideSplashScreen()
{
	Log(MESSAGE, "XboxUI", "Hiding splash screen");
}

void XboxUI::SetTargetFrameRate(int fps)
{
	targetFPS = fps;
	Log(MESSAGE, "XboxUI", "Set target frame rate to %d FPS", fps);
}

void XboxUI::EnableVSync(bool enable)
{
	Log(MESSAGE, "XboxUI", "%s VSync", enable ? "Enabled" : "Disabled");
	// Implementation would configure VSync
}

void XboxUI::ReduceVisualEffects(bool reduce)
{
	Log(MESSAGE, "XboxUI", "%s visual effects reduction", reduce ? "Enabled" : "Disabled");
	// Implementation would reduce particles, shadows, etc.
}

void XboxUI::SetUIScale(float scale)
{
	uiScale = scale;
	Log(MESSAGE, "XboxUI", "Set UI scale to %.2f", scale);
}

float XboxUI::GetRecommendedUIScale()
{
	return 1.3f; // Recommended for TV viewing
}

void XboxUI::AdaptMenusForController()
{
	Log(MESSAGE, "XboxUI", "Adapting menus for controller input");
	// Implementation would modify menu navigation
}

void XboxUI::EnableControllerCursor()
{
	Log(MESSAGE, "XboxUI", "Enabled controller cursor");
	// Implementation would show controller-driven cursor
}

void XboxUI::SetControllerCursorSpeed(float speed)
{
	Log(MESSAGE, "XboxUI", "Set controller cursor speed to %.2f", speed);
}

void XboxUI::CreateLoadingWindow()
{
	VideoDriver* video = core->GetVideoDriver();
	if (!video) return;

	Size screenSize = video->GetScreenSize();

	// Create loading window covering the screen
	Region loadingRegion(0, 0, screenSize.w, screenSize.h);
	loadingWindow = new Window(loadingRegion, 0x01000000); // Semi-transparent black

	// Add loading message label
	Region messageRegion(screenSize.w / 4, screenSize.h / 2 - 50, screenSize.w / 2, 30);
	Label* messageLabel = new Label(messageRegion, nullptr, "Loading...", nullptr);
	messageLabel->SetID("MESSAGE");
	loadingWindow->AddSubView(messageLabel);

	// Add progress bar
	Region progressRegion(screenSize.w / 4, screenSize.h / 2 + 20, screenSize.w / 2, 20);
	Progressbar* progressBar = new Progressbar(progressRegion, nullptr);
	progressBar->SetID("PROGRESS");
	loadingWindow->AddSubView(progressBar);

	loadingWindow->SetVisible(false);
}

void XboxUI::CreateErrorWindow()
{
	VideoDriver* video = core->GetVideoDriver();
	if (!video) return;

	Size screenSize = video->GetScreenSize();

	// Create error dialog window
	Region errorRegion(screenSize.w / 4, screenSize.h / 4, screenSize.w / 2, screenSize.h / 2);
	errorWindow = new Window(errorRegion, 0x80000000); // Dark background

	// Add title label
	Region titleRegion(10, 10, errorRegion.w - 20, 30);
	Label* titleLabel = new Label(titleRegion, nullptr, "Error", nullptr);
	titleLabel->SetID("TITLE");
	errorWindow->AddSubView(titleLabel);

	// Add message label
	Region messageRegion(10, 50, errorRegion.w - 20, errorRegion.h - 100);
	Label* messageLabel = new Label(messageRegion, nullptr, "", nullptr);
	messageLabel->SetID("MESSAGE");
	errorWindow->AddSubView(messageLabel);

	errorWindow->SetVisible(false);
}

void XboxUI::UpdateFrameRateLimiter()
{
	tick_t currentTime = GetMilliseconds();
	tick_t targetFrameTime = 1000 / targetFPS;

	if (currentTime - lastFrameTime < targetFrameTime) {
		// Frame rate limiting would happen here
	}

	lastFrameTime = currentTime;
}

// XboxButton implementation
void XboxButton::DrawXboxButton(const Region& region, XboxButtonStyle style, bool pressed)
{
	// Implementation would draw Xbox controller button graphics
	Log(DEBUG, "XboxButton", "Drawing Xbox button style %d %s",
	    style, pressed ? "pressed" : "normal");
}

void XboxButton::SetButtonPrompts(bool enabled)
{
	Log(MESSAGE, "XboxButton", "%s Xbox button prompts", enabled ? "Enabled" : "Disabled");
}

// XboxLoadingIndicator implementation
bool XboxLoadingIndicator::visible = false;
Point XboxLoadingIndicator::position;
String XboxLoadingIndicator::text;
int XboxLoadingIndicator::style = 0;
float XboxLoadingIndicator::animationTime = 0.0f;

void XboxLoadingIndicator::Show(const Point& pos, const String& txt)
{
	position = pos;
	text = txt;
	visible = true;
	animationTime = 0.0f;

	Log(DEBUG, "XboxLoadingIndicator", "Showing loading indicator at (%d, %d): %s",
	    pos.x, pos.y, txt.c_str());
}

void XboxLoadingIndicator::Hide()
{
	visible = false;
	Log(DEBUG, "XboxLoadingIndicator", "Hiding loading indicator");
}

void XboxLoadingIndicator::Update()
{
	if (!visible) return;

	animationTime += 16.0f; // Assuming ~60 FPS

	// Update animation
	// Implementation would animate the loading indicator
}

void XboxLoadingIndicator::SetStyle(int newStyle)
{
	style = newStyle;
	Log(DEBUG, "XboxLoadingIndicator", "Set loading indicator style to %d", style);
}

} // namespace GemRB
