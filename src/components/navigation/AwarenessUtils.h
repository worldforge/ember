/*
 Copyright (C) 2014 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 This work is based on code written by Mikko Mononen, as specified below.

//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
 */

#ifndef AWARENESSUTILS_H_
#define AWARENESSUTILS_H_

#include "fastlz.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourCommon.h"
#include "DetourTileCache.h"
#include "DetourTileCacheBuilder.h"
#include <string.h>

namespace Ember {
namespace Navigation {

struct FastLZCompressor : public dtTileCacheCompressor {
	virtual ~FastLZCompressor() = default;

	int maxCompressedSize(const int bufferSize) override {
		return (int) ((float) bufferSize * 1.05f);
	}

	dtStatus compress(const unsigned char* buffer, const int bufferSize, unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize) override {
		*compressedSize = fastlz_compress((void*) buffer, bufferSize, compressed);
		return DT_SUCCESS;
	}

	dtStatus decompress(const unsigned char* compressed, const int compressedSize, unsigned char* buffer, const int maxBufferSize, int* bufferSize) override {
		*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
		return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
	}
};

struct LinearAllocator : public dtTileCacheAlloc {
	unsigned char* buffer;
	int capacity;
	int top;
	int high;

	explicit LinearAllocator(const int cap) :
			buffer(nullptr), capacity(0), top(0), high(0) {
		resize(cap);
	}

	virtual ~LinearAllocator() {
		dtFree(buffer);
	}

	void resize(const int cap) {
		if (buffer)
			dtFree(buffer);
		buffer = (unsigned char*) dtAlloc(cap, DT_ALLOC_PERM);
		capacity = cap;
	}

	void reset() override {
		high = dtMax(high, top);
		top = 0;
	}

	void* alloc(const int size) override {
		if (!buffer)
			return nullptr;
		if (top + size > capacity)
			return nullptr;
		unsigned char* mem = &buffer[top];
		top += size;
		return mem;
	}

	void free(void* /*ptr*/) override {
		// Empty
	}
};

struct MeshProcess : public dtTileCacheMeshProcess {

	inline MeshProcess() = default;

	virtual ~MeshProcess() = default;

	void process(struct dtNavMeshCreateParams* params, unsigned char* polyAreas, unsigned short* polyFlags) override {
		// Update poly flags from areas.
		for (int i = 0; i < params->polyCount; ++i) {
			if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
				polyAreas[i] = POLYAREA_GROUND;

			if (polyAreas[i] == POLYAREA_GROUND || polyAreas[i] == POLYAREA_GRASS || polyAreas[i] == POLYAREA_ROAD) {
				polyFlags[i] = POLYFLAGS_WALK;
			} else if (polyAreas[i] == POLYAREA_WATER) {
				polyFlags[i] = POLYFLAGS_SWIM;
			} else if (polyAreas[i] == POLYAREA_DOOR) {
				polyFlags[i] = POLYFLAGS_WALK | POLYFLAGS_DOOR;
			}
		}
	}
};

struct TileCacheData {
	unsigned char* data;
	int dataSize;
};

static const int MAX_LAYERS = 32;

struct RasterizationContext {
	RasterizationContext() :
			solid(nullptr),
			triareas(nullptr),
			lset(nullptr),
			chf(nullptr),
			ntiles(0) {
		memset(tiles, 0, sizeof(TileCacheData) * MAX_LAYERS);
	}

	~RasterizationContext() {
		rcFreeHeightField(solid);
		delete[] triareas;
		rcFreeHeightfieldLayerSet(lset);
		rcFreeCompactHeightfield(chf);
		for (auto & tile : tiles) {
			dtFree(tile.data);
			tile.data = nullptr;
		}
	}

	rcHeightfield* solid;
	unsigned char* triareas;
	rcHeightfieldLayerSet* lset;
	rcCompactHeightfield* chf;
	TileCacheData tiles[MAX_LAYERS]{};
	int ntiles;
};

}
}

#endif /* AWARENESSUTILS_H_ */
