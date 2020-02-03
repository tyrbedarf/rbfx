//
// Copyright (c) 2017-2020 the rbfx project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

/// \file

#pragma once

#include "../Graphics/Drawable.h"

#include <EASTL/vector.h>

namespace Urho3D
{

class Camera;
class Context;
class Octree;
class WorkQueue;

// TODO: Get rid of it
class View;

/// Represents Model in editable form.
class URHO3D_API ViewRenderer
{
public:
    /// Update drawables from Octree.
    void Update(const FrameInfo& frameInfo, Octree* octree);

    /// Render scene pass.
    void RenderScenePass(View* view);

private:
    /// Per-drawable traits.
    enum DrawableTrait : unsigned char
    {
        /// No traits.
        NoTraits = 0,
        /// Drawable was collected by base query and its batches were updated.
        HasBaseBatches = 1 << 0,
    };

    /// Per-worker data for base drawables update.
    struct PerWorkerBaseUpdate
    {
        /// Geometry objects.
        ea::vector<Drawable*> geometries_;
        /// Lights.
        ea::vector<Light*> lights_;
        /// Scene minimum Z value.
        float minZ_{};
        /// Scene maximum Z value.
        float maxZ_{};

        /// Clear.
        void Clear()
        {
            geometries_.clear();
            lights_.clear();
            minZ_ = M_LARGE_VALUE;
            maxZ_ = -M_LARGE_VALUE;
        }
    };

    /// Context.
    Context* context_{};
    /// Octree.
    Octree* octree_{};
    /// Work queue.
    WorkQueue* workQueue_{};
    /// Cull camera.
    Camera* cullCamera_{};
    /// Frame info.
    FrameInfo frameInfo_;

    /// Number of threads including main thread.
    unsigned numThreads_{};
    /// Temporary drawables.
    ea::vector<ea::vector<Drawable*>> tempDrawables_;
    /// Temporary buffers for base update.
    ea::vector<PerWorkerBaseUpdate> tempBaseUpdate_;

    /// Total number of drawables.
    unsigned numDrawables_{};
    /// Various Drawable traits.
    ea::vector<unsigned char> drawableTraits_;
    /// Drawable min and max Z values.
    ea::vector<ea::pair<float, float>> drawableMinMaxZ_;
};

}
