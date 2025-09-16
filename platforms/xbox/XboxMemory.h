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

#ifndef XBOX_MEMORY_H
#define XBOX_MEMORY_H

#include "exports.h"
#include "ie_types.h"

#include <unordered_map>
#include <vector>

namespace GemRB {

class GEM_EXPORT XboxMemoryManager {
public:
	struct MemoryInfo {
		size_t totalMemory;
		size_t availableMemory;
		size_t usedMemory;
		float usagePercent;
		bool isLow;
		bool isCritical;
	};

	struct ResourceInfo {
		void* data;
		size_t size;
		String type;
		tick_t lastAccessed;
		int accessCount;
		bool isStatic;
	};

	static void Initialize();
	static void Shutdown();
	static void Update();

	// Memory monitoring
	static MemoryInfo GetMemoryInfo();
	static bool IsMemoryLow();
	static bool IsMemoryCritical();

	// Resource management
	static void RegisterResource(const String& id, void* data, size_t size, const String& type, bool isStatic = false);
	static void UnregisterResource(const String& id);
	static void TouchResource(const String& id);

	// Memory cleanup
	static void TriggerCleanup(bool aggressive = false);
	static void CleanupUnusedTextures();
	static void CleanupUnusedSounds();
	static void CleanupUnusedAnimations();

	// Preloading and caching
	static void PreloadCriticalResources();
	static void SetCacheSize(const String& type, size_t maxSize);

	// Memory allocation with tracking
	static void* AllocateTracked(size_t size, const String& category);
	static void FreeTracked(void* ptr);

	// Performance optimization
	static void OptimizeForFrameRate();
	static void ReduceMemoryFootprint();

private:
	static bool initialized;
	static const size_t XBOX_TOTAL_MEMORY = 64 * 1024 * 1024; // 64MB
	static const float LOW_MEMORY_THRESHOLD;
	static const float CRITICAL_MEMORY_THRESHOLD;

	static std::unordered_map<String, ResourceInfo> resources;
	static std::unordered_map<String, size_t> cacheSizes;
	static std::unordered_map<String, size_t> categoryUsage;
	static tick_t lastCleanup;
	static tick_t cleanupInterval;

	static void PerformGarbageCollection();
	static std::vector<String> GetUnusedResources(tick_t threshold);
	static size_t GetSystemMemoryUsage();
};

// Auto-cleanup RAII wrapper for Xbox memory
template<typename T>
class XboxAutoPtr {
public:
	XboxAutoPtr(T* ptr = nullptr, const String& id = "")
		: ptr_(ptr), id_(id)
	{
		if (ptr_ && !id_.empty()) {
			XboxMemoryManager::TouchResource(id_);
		}
	}

	~XboxAutoPtr()
	{
		if (ptr_ && !id_.empty()) {
			XboxMemoryManager::UnregisterResource(id_);
		}
		delete ptr_;
	}

	T* get() const { return ptr_; }
	T* operator->() const { return ptr_; }
	T& operator*() const { return *ptr_; }

	T* release()
	{
		T* temp = ptr_;
		ptr_ = nullptr;
		return temp;
	}

	void reset(T* ptr = nullptr, const String& id = "")
	{
		if (ptr_ && !id_.empty()) {
			XboxMemoryManager::UnregisterResource(id_);
		}
		delete ptr_;
		ptr_ = ptr;
		id_ = id;
		if (ptr_ && !id_.empty()) {
			XboxMemoryManager::TouchResource(id_);
		}
	}

private:
	T* ptr_;
	String id_;

	// Non-copyable
	XboxAutoPtr(const XboxAutoPtr&) = delete;
	XboxAutoPtr& operator=(const XboxAutoPtr&) = delete;
};

} // namespace GemRB

#endif // XBOX_MEMORY_H
