//
// C++ Interface: OgreIncludes
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_OGREINCLUDES_H
#define EMBEROGRE_OGREINCLUDES_H

//base include files needed for Ogre

//#include <Ogre.h>
#include <OgreConfig.h>

namespace Ogre
{
// define the real number values to be used
// default to use 'float' unless precompiler option set
#if OGRE_DOUBLE_PRECISION == 1
/** Software floating point type.
   @note Not valid as a pointer to GPU buffers / parameters
 */
typedef double Real;
#else
/** Software floating point type.
   @note Not valid as a pointer to GPU buffers / parameters
 */
typedef float Real;
#endif // if OGRE_DOUBLE_PRECISION == 1

// Pre-declare classes
// Allows use of pointers in header files without including individual .h
// so decreases dependencies between files
class Angle;
class Animation;
class AnimationState;
class AnimationStateSet;
class AnimationTrack;
class Archive;
class ArchiveFactory;
class ArchiveManager;
class AutoParamDataSource;
class AxisAlignedBox;
class AxisAlignedBoxSceneQuery;
class Billboard;
class BillboardChain;
class BillboardSet;
class Bone;
class Camera;
class Codec;
class ColourValue;
class ConfigDialog;
template<typename T>
class Controller;
template<typename T>
class ControllerFunction;
class ControllerManager;
template<typename T>
class ControllerValue;
class DefaultWorkQueue;
class Degree;
class DepthBuffer;
class DynLib;
class DynLibManager;
class EdgeData;
class EdgeListBuilder;
class Entity;
class ErrorDialog;
class ExternalTextureSourceManager;
class Factory;
struct FrameEvent;
class FrameListener;
class Frustum;
class GpuProgram;
class GpuProgramManager;
class GpuProgramUsage;
class HardwareIndexBuffer;
class HardwareOcclusionQuery;
class HardwareVertexBuffer;
class HardwarePixelBuffer;
class HardwarePixelBufferSharedPtr;
class HighLevelGpuProgram;
class HighLevelGpuProgramManager;
class HighLevelGpuProgramFactory;
class IndexData;
class InstanceBatch;
class InstanceBatchHW;
class InstanceBatchHW_VTF;
class InstanceBatchShader;
class InstanceBatchVTF;
class InstanceManager;
class InstancedEntity;
class IntersectionSceneQuery;
class IntersectionSceneQueryListener;
class Image;
class KeyFrame;
class Light;
class Log;
class LogManager;
class LodStrategy;
class ManualResourceLoader;
class ManualObject;
class Material;
class MaterialManager;
class Math;
class Matrix3;
class Matrix4;
class MemoryManager;
class Mesh;
class MeshSerializer;
class MeshSerializerImpl;
class MeshManager;
class MovableObject;
class MovablePlane;
class Node;
class NodeAnimationTrack;
class NodeKeyFrame;
class NumericAnimationTrack;
class NumericKeyFrame;
class Particle;
class ParticleAffector;
class ParticleAffectorFactory;
class ParticleEmitter;
class ParticleEmitterFactory;
class ParticleSystem;
class ParticleSystemManager;
class ParticleSystemRenderer;
class ParticleSystemRendererFactory;
class ParticleVisualData;
class Pass;
class PatchMesh;
class PixelBox;
class Plane;
class PlaneBoundedVolume;
class Plugin;
class PMWorker;
class PMInjector;
class Pose;
class ProgressiveMeshGenerator;
class Profile;
class Profiler;
class Quaternion;
class Radian;
class Ray;
class RaySceneQuery;
class RaySceneQueryListener;
class Renderable;
class RenderPriorityGroup;
class RenderQueue;
class RenderQueueGroup;
class RenderQueueInvocation;
class RenderQueueInvocationSequence;
class RenderQueueListener;
class RenderObjectListener;
class RenderSystem;
class RenderSystemCapabilities;
class RenderSystemCapabilitiesManager;
class RenderSystemCapabilitiesSerializer;
class RenderTarget;
class RenderTargetListener;
class RenderTexture;
class MultiRenderTarget;
class RenderWindow;
class RenderOperation;
class Resource;
class ResourceBackgroundQueue;
class ResourceGroupManager;
class ResourceManager;
class RibbonTrail;
class Root;
class SceneManager;
class SceneManagerEnumerator;
class SceneNode;
class SceneQuery;
class SceneQueryListener;
class ScriptCompiler;
class ScriptCompilerManager;
class ScriptLoader;
class Serializer;
class ShadowCaster;
class ShadowRenderable;
class ShadowTextureManager;
class SimpleRenderable;
class SimpleSpline;
class Skeleton;
class SkeletonInstance;
class SkeletonManager;
class Sphere;
class SphereSceneQuery;
class StaticGeometry;
class StreamSerialiser;
class StringConverter;
class StringInterface;
class SubEntity;
class SubMesh;
class TagPoint;
class Technique;
class TempBlendedBufferInfo;
class ExternalTextureSource;
class TextureUnitState;
class Texture;
class TextureManager;
class TransformKeyFrame;
class Timer;
template< typename T >
struct TRect;
class UserObjectBindings;
class Vector2;
class Vector3;
class Vector4;
class Viewport;
class VertexAnimationTrack;
class VertexBufferBinding;
class VertexData;
class VertexDeclaration;
class VertexMorphKeyFrame;
class WireBoundingBox;
class WorkQueue;
class Compositor;
class CompositorManager;
class CompositorChain;
class CompositorInstance;
class CompositorLogic;
class CompositionTechnique;
class CompositionPass;
class CompositionTargetPass;
class CustomCompositionPass;

template<typename T>
class SharedPtr;
typedef SharedPtr<Compositor> CompositorPtr;
typedef SharedPtr<GpuProgram> GpuProgramPtr;
typedef SharedPtr<HighLevelGpuProgram> HighLevelGpuProgramPtr;
typedef SharedPtr<Material> MaterialPtr;
typedef SharedPtr<Mesh> MeshPtr;
typedef SharedPtr<PatchMesh> PatchMeshPtr;
typedef SharedPtr<Resource> ResourcePtr;
typedef SharedPtr<Skeleton> SkeletonPtr;
typedef SharedPtr<Texture> TexturePtr;

/////// MODIFIED
// Overlay system typedefs
class Overlay;
class OverlayContainer;
class OverlayElement;
class OverlayElementFactory;
class OverlayManager;
class OverlaySystem;
// other
class PSSMShadowCameraSetup;
}

#endif //EMBEROGRE_OGREINCLUDES_H
