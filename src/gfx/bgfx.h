#pragma once

#include "../math.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

/// Shim stolen from bgfx_utils
/// Returns true if both internal transient index and vertex buffer have
/// enough space.
///
/// @param[in] _numVertices Number of vertices.
/// @param[in] _layout Vertex layout.
/// @param[in] _numIndices Number of indices.
///
inline bool checkAvailTransientBuffers(u32 _numVertices, const bgfx::VertexLayout &_layout, u32 _numIndices) {
    return _numVertices == bgfx::getAvailTransientVertexBuffer(_numVertices, _layout) &&
           (0 == _numIndices || _numIndices == bgfx::getAvailTransientIndexBuffer(_numIndices));
}