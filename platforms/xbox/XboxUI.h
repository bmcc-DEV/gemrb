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

#ifndef XBOX_UI_H
#define XBOX_UI_H

#include "exports.h"
#include "ie_types.h"

#include "GUI/Window.h"

namespace GemRB {

class GEM_EXPORT XboxUI {
public:
	static void Initialize();
	static void Shutdown();

	// CRT TV optimizations
	static void OptimizeForCRT();
	static void SetCRTSafeZone(float percentage); // 0.85 = 85% of screen
	static void AdjustFontSizesForTV();
	static void SetMinimumFontSize(int size);

	// Loading screens and feedback
	static void ShowLoadingScreen(const String& message, float progress = -1.0f);
	static void HideLoadingScreen();
	static void UpdateLoadingProgress(float progress, const String& message = "");

	// Error feedback
	static void ShowErrorMessage(const String& title, const String& message, bool blocking = true);
	static void ShowWarningMessage(const String& message, uint32_t duration = 3000);
	static void ShowInfoMessage(const String& message, uint32_t duration = 2000);

	// Splash screen
	static void ShowSplashScreen();
	static void HideSplashScreen();

	// Frame rate optimization
	static void SetTargetFrameRate(int fps);
	static void EnableVSync(bool enable);
	static void ReduceVisualEffects(bool reduce);

	// UI scaling for TV viewing distance
	static void SetUIScale(float scale);
	static float GetRecommendedUIScale();

	// Menu adaptations
	static void AdaptMenusForController();
	static void EnableControllerCursor();
	static void SetControllerCursorSpeed(float speed);

private:
	static bool initialized;
	static bool crtOptimized;
	static float crtSafeZone;
	static int minimumFontSize;
	static float uiScale;
	static bool loadingScreenVisible;
	static Window* loadingWindow;
	static Window* errorWindow;
	static tick_t lastFrameTime;
	static int targetFPS;

	static void CreateLoadingWindow();
	static void CreateErrorWindow();
	static void UpdateFrameRateLimiter();
};

// Xbox-specific UI controls optimized for controllers
class GEM_EXPORT XboxButton {
public:
	enum XboxButtonStyle {
		XBOX_A_BUTTON,
		XBOX_B_BUTTON,
		XBOX_X_BUTTON,
		XBOX_Y_BUTTON,
		XBOX_START_BUTTON,
		XBOX_BACK_BUTTON
	};

	static void DrawXboxButton(const Region& region, XboxButtonStyle style, bool pressed = false);
	static void SetButtonPrompts(bool enabled);
};

// Xbox loading indicators
class GEM_EXPORT XboxLoadingIndicator {
public:
	static void Show(const Point& position, const String& text = "");
	static void Hide();
	static void Update();
	static void SetStyle(int style); // 0=spinner, 1=progress bar, 2=dots

private:
	static bool visible;
	static Point position;
	static String text;
	static int style;
	static float animationTime;
};

} // namespace GemRB

#endif // XBOX_UI_H
