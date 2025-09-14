/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2024 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef XBOX_MEMORY_OPTIMIZATIONS_H
#define XBOX_MEMORY_OPTIMIZATIONS_H

#ifdef XBOX

	#include <cstddef>
	#include <memory>
	#include <vector>
	#include <windows.h>

namespace GemRB {

// Xbox memory constants - original Xbox has 64MB total RAM
static constexpr size_t XBOX_TOTAL_MEMORY = 64 * 1024 * 1024; // 64MB
static constexpr size_t XBOX_SYSTEM_RESERVED = 16 * 1024 * 1024; // 16MB for system
static constexpr size_t XBOX_AVAILABLE_MEMORY = XBOX_TOTAL_MEMORY - XBOX_SYSTEM_RESERVED;
static constexpr size_t XBOX_TEXTURE_CACHE_SIZE = 8 * 1024 * 1024; // 8MB for textures
static constexpr size_t XBOX_AUDIO_BUFFER_POOL = 2 * 1024 * 1024; // 2MB for audio

// Xbox memory pool manager for efficient allocation
class XboxMemoryManager {
public:
	static XboxMemoryManager& GetInstance();

	// Initialize Xbox memory management
	void Initialize();

	// Memory pool operations
	void* AllocateFromPool(size_t size, const char* category = nullptr);
	void DeallocateFromPool(void* ptr);

	// Memory usage monitoring
	size_t GetUsedMemory() const;
	size_t GetAvailableMemory() const;
	float GetMemoryUsagePercentage() const;

	// Xbox-specific optimizations
	void CompactMemoryPools();
	void FlushUnusedCaches();

	// Memory debugging for Xbox
	void PrintMemoryStats() const;

private:
	XboxMemoryManager() = default;
	bool initialized = false;
	size_t totalAllocated = 0;

	// Memory allocation tracking
	struct AllocationInfo {
		size_t size;
		const char* category;
	};

	std::vector<AllocationInfo> allocations;
};

// Xbox texture cache optimization
class XboxTextureCache {
public:
	static XboxTextureCache& GetInstance();

	// Initialize texture cache with Xbox GPU optimizations
	void Initialize();

	// Xbox GPU texture compression
	bool CompressTexture(void* textureData, size_t size);
	void* GetCompressedTexture(const char* filename);

	// Xbox video memory optimizations
	void OptimizeForXboxGPU();
	void PreloadCommonTextures();

	// Cache management
	void FlushOldTextures();
	size_t GetCacheUsage() const;

private:
	XboxTextureCache() = default;
	bool initialized = false;
	size_t cacheUsed = 0;
};

// Xbox GPU optimization helpers
class XboxGPUOptimizer {
public:
	// Initialize Xbox GPU optimizations
	static void Initialize();

	// Xbox-specific rendering optimizations
	static void EnableXboxRenderingOptimizations();
	static void SetupXboxDisplayMode();

	// Xbox progressive scan support (480p)
	static bool EnableProgressiveScan();
	static bool IsProgressiveScanSupported();

	// Xbox GPU memory management
	static void OptimizeGPUMemoryUsage();
	static void EnableHardwareAcceleration();

	// Xbox-specific video modes
	static void SetOptimalVideoMode();
	static bool SupportsWidescreen();
};

// Xbox hard drive caching optimizations
class XboxStorageOptimizer {
public:
	// Initialize Xbox hard drive optimizations
	static void Initialize();

	// Xbox hard drive caching
	static void EnableSmartCaching();
	static void PreloadGameData();

	// Xbox file system optimizations
	static void OptimizeFileAccess();
	static bool CacheFrequentlyUsedFiles();

	// Xbox save game optimization
	static void OptimizeSaveGameOperations();

private:
	static bool initialized;
};

} // namespace GemRB

#endif // XBOX
#endif // XBOX_MEMORY_OPTIMIZATIONS_H
