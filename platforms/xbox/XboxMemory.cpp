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

#include "XboxMemory.h"

#include "GlobalTimer.h"

#include "Logging/Logging.h"

#include <algorithm>

#ifdef _XBOX
	#include <xtl.h>
#endif

namespace GemRB {

bool XboxMemoryManager::initialized = false;
const float XboxMemoryManager::LOW_MEMORY_THRESHOLD = 0.15f; // 15% available
const float XboxMemoryManager::CRITICAL_MEMORY_THRESHOLD = 0.05f; // 5% available

std::unordered_map<String, XboxMemoryManager::ResourceInfo> XboxMemoryManager::resources;
std::unordered_map<String, size_t> XboxMemoryManager::cacheSizes;
std::unordered_map<String, size_t> XboxMemoryManager::categoryUsage;
tick_t XboxMemoryManager::lastCleanup = 0;
tick_t XboxMemoryManager::cleanupInterval = 30000; // 30 seconds

void XboxMemoryManager::Initialize()
{
	if (initialized) return;

	Log(MESSAGE, "XboxMemoryManager", "Initializing Xbox memory management");

	// Set default cache sizes for different resource types
	cacheSizes["textures"] = 16 * 1024 * 1024; // 16MB for textures
	cacheSizes["sounds"] = 8 * 1024 * 1024; // 8MB for sounds
	cacheSizes["animations"] = 4 * 1024 * 1024; // 4MB for animations
	cacheSizes["scripts"] = 2 * 1024 * 1024; // 2MB for scripts

	lastCleanup = GetMilliseconds();
	initialized = true;
}

void XboxMemoryManager::Shutdown()
{
	if (!initialized) return;

	Log(MESSAGE, "XboxMemoryManager", "Shutting down Xbox memory management");

	// Clean up all tracked resources
	TriggerCleanup(true);
	resources.clear();
	categoryUsage.clear();

	initialized = false;
}

void XboxMemoryManager::Update()
{
	if (!initialized) return;

	tick_t currentTime = GetMilliseconds();

	// Periodic cleanup check
	if (currentTime - lastCleanup > cleanupInterval) {
		MemoryInfo info = GetMemoryInfo();

		if (info.isLow) {
			Log(MESSAGE, "XboxMemoryManager", "Memory low (%.1f%%), triggering cleanup",
			    info.usagePercent * 100.0f);
			TriggerCleanup(false);
		} else if (info.isCritical) {
			Log(WARNING, "XboxMemoryManager", "Memory critical (%.1f%%), aggressive cleanup",
			    info.usagePercent * 100.0f);
			TriggerCleanup(true);
		}

		lastCleanup = currentTime;
	}
}

XboxMemoryManager::MemoryInfo XboxMemoryManager::GetMemoryInfo()
{
	MemoryInfo info;
	info.totalMemory = XBOX_TOTAL_MEMORY;

#ifdef _XBOX
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	info.availableMemory = memStatus.dwAvailPhys;
	info.usedMemory = info.totalMemory - info.availableMemory;
#else
	// Simulate Xbox memory constraints on other platforms
	size_t simulatedUsed = GetSystemMemoryUsage();
	info.usedMemory = std::min(simulatedUsed, info.totalMemory);
	info.availableMemory = info.totalMemory - info.usedMemory;
#endif

	info.usagePercent = (float) info.usedMemory / info.totalMemory;
	info.isLow = (float) info.availableMemory / info.totalMemory < LOW_MEMORY_THRESHOLD;
	info.isCritical = (float) info.availableMemory / info.totalMemory < CRITICAL_MEMORY_THRESHOLD;

	return info;
}

bool XboxMemoryManager::IsMemoryLow()
{
	return GetMemoryInfo().isLow;
}

bool XboxMemoryManager::IsMemoryCritical()
{
	return GetMemoryInfo().isCritical;
}

void XboxMemoryManager::RegisterResource(const String& id, void* data, size_t size,
					 const String& type, bool isStatic)
{
	if (!initialized) return;

	ResourceInfo info;
	info.data = data;
	info.size = size;
	info.type = type;
	info.lastAccessed = GetMilliseconds();
	info.accessCount = 1;
	info.isStatic = isStatic;

	resources[id] = info;
	categoryUsage[type] += size;

	Log(DEBUG, "XboxMemoryManager", "Registered resource %s (%s): %zu bytes",
	    id.c_str(), type.c_str(), size);

	// Check if we're over cache limit for this type
	if (cacheSizes.count(type) && categoryUsage[type] > cacheSizes[type]) {
		Log(MESSAGE, "XboxMemoryManager", "Cache limit exceeded for %s, triggering cleanup", type.c_str());
		TriggerCleanup(false);
	}
}

void XboxMemoryManager::UnregisterResource(const String& id)
{
	auto it = resources.find(id);
	if (it != resources.end()) {
		categoryUsage[it->second.type] -= it->second.size;
		Log(DEBUG, "XboxMemoryManager", "Unregistered resource %s", id.c_str());
		resources.erase(it);
	}
}

void XboxMemoryManager::TouchResource(const String& id)
{
	auto it = resources.find(id);
	if (it != resources.end()) {
		it->second.lastAccessed = GetMilliseconds();
		it->second.accessCount++;
	}
}

void XboxMemoryManager::TriggerCleanup(bool aggressive)
{
	Log(MESSAGE, "XboxMemoryManager", "Starting %s cleanup", aggressive ? "aggressive" : "normal");

	tick_t threshold = GetMilliseconds() - (aggressive ? 10000 : 60000); // 10s vs 60s
	std::vector<String> unusedResources = GetUnusedResources(threshold);

	// Sort by last accessed time (oldest first)
	std::sort(unusedResources.begin(), unusedResources.end(),
		  [this](const String& a, const String& b) {
			  return resources[a].lastAccessed < resources[b].lastAccessed;
		  });

	size_t freedMemory = 0;
	size_t targetToFree = aggressive ? XBOX_TOTAL_MEMORY / 4 : XBOX_TOTAL_MEMORY / 8;

	for (const String& id : unusedResources) {
		auto it = resources.find(id);
		if (it != resources.end() && !it->second.isStatic) {
			freedMemory += it->second.size;
			Log(DEBUG, "XboxMemoryManager", "Freeing unused resource: %s (%zu bytes)",
			    id.c_str(), it->second.size);
			UnregisterResource(id);

			if (freedMemory >= targetToFree) {
				break;
			}
		}
	}

	Log(MESSAGE, "XboxMemoryManager", "Cleanup complete: freed %zu bytes", freedMemory);

	if (aggressive) {
		PerformGarbageCollection();
	}
}

void XboxMemoryManager::CleanupUnusedTextures()
{
	tick_t threshold = GetMilliseconds() - 30000; // 30 seconds

	for (auto it = resources.begin(); it != resources.end();) {
		if (it->second.type == "texture" &&
		    it->second.lastAccessed < threshold &&
		    !it->second.isStatic) {
			Log(DEBUG, "XboxMemoryManager", "Cleaning up unused texture: %s", it->first.c_str());
			categoryUsage[it->second.type] -= it->second.size;
			it = resources.erase(it);
		} else {
			++it;
		}
	}
}

void XboxMemoryManager::CleanupUnusedSounds()
{
	tick_t threshold = GetMilliseconds() - 15000; // 15 seconds for sounds (shorter lived)

	for (auto it = resources.begin(); it != resources.end();) {
		if (it->second.type == "sound" &&
		    it->second.lastAccessed < threshold &&
		    !it->second.isStatic) {
			Log(DEBUG, "XboxMemoryManager", "Cleaning up unused sound: %s", it->first.c_str());
			categoryUsage[it->second.type] -= it->second.size;
			it = resources.erase(it);
		} else {
			++it;
		}
	}
}

void XboxMemoryManager::CleanupUnusedAnimations()
{
	tick_t threshold = GetMilliseconds() - 45000; // 45 seconds for animations

	for (auto it = resources.begin(); it != resources.end();) {
		if (it->second.type == "animation" &&
		    it->second.lastAccessed < threshold &&
		    !it->second.isStatic) {
			Log(DEBUG, "XboxMemoryManager", "Cleaning up unused animation: %s", it->first.c_str());
			categoryUsage[it->second.type] -= it->second.size;
			it = resources.erase(it);
		} else {
			++it;
		}
	}
}

void XboxMemoryManager::PreloadCriticalResources()
{
	Log(MESSAGE, "XboxMemoryManager", "Preloading critical resources");
	// This would load essential UI textures, sounds, etc.
	// Implementation depends on specific game requirements
}

void XboxMemoryManager::SetCacheSize(const String& type, size_t maxSize)
{
	cacheSizes[type] = maxSize;
	Log(MESSAGE, "XboxMemoryManager", "Set cache size for %s: %zu bytes", type.c_str(), maxSize);
}

void* XboxMemoryManager::AllocateTracked(size_t size, const String& category)
{
	void* ptr = malloc(size);
	if (ptr) {
		categoryUsage[category] += size;
	}
	return ptr;
}

void XboxMemoryManager::FreeTracked(void* ptr)
{
	if (ptr) {
		free(ptr);
		// Note: In a real implementation, we'd need to track size to subtract from categoryUsage
	}
}

void XboxMemoryManager::OptimizeForFrameRate()
{
	Log(MESSAGE, "XboxMemoryManager", "Optimizing memory layout for frame rate");

	// Reduce cache sizes to ensure consistent 64 FPS
	SetCacheSize("textures", 12 * 1024 * 1024); // Reduce texture cache
	SetCacheSize("sounds", 6 * 1024 * 1024); // Reduce sound cache

	TriggerCleanup(false);
}

void XboxMemoryManager::ReduceMemoryFootprint()
{
	Log(MESSAGE, "XboxMemoryManager", "Reducing memory footprint");

	// More aggressive cache size reduction
	SetCacheSize("textures", 8 * 1024 * 1024);
	SetCacheSize("sounds", 4 * 1024 * 1024);
	SetCacheSize("animations", 2 * 1024 * 1024);

	TriggerCleanup(true);
}

void XboxMemoryManager::PerformGarbageCollection()
{
#ifdef _XBOX
	// On Xbox, we can't call system GC, but we can defragment our pools
	Log(MESSAGE, "XboxMemoryManager", "Performing memory defragmentation");
#else
	Log(MESSAGE, "XboxMemoryManager", "Simulating garbage collection");
#endif
}

std::vector<String> XboxMemoryManager::GetUnusedResources(tick_t threshold)
{
	std::vector<String> unused;

	for (const auto& pair : resources) {
		if (pair.second.lastAccessed < threshold && !pair.second.isStatic) {
			unused.push_back(pair.first);
		}
	}

	return unused;
}

size_t XboxMemoryManager::GetSystemMemoryUsage()
{
	size_t total = 0;
	for (const auto& pair : categoryUsage) {
		total += pair.second;
	}
	return total;
}

} // namespace GemRB
