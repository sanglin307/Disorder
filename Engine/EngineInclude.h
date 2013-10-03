// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#endif

#undef min 
#undef max

//std lib
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>

//boost lib
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/assert.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool.hpp>
#include "ComPtr.h"
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


//Class pre declare
namespace Disorder
{
	class Component;
	typedef boost::shared_ptr<Component> ComponentPtr;
	class Camera;
	typedef boost::shared_ptr<Camera> CameraPtr;
	class InputEventHandler;
	typedef boost::shared_ptr<InputEventHandler> InputEventHandlerPtr;
	class InputManager;
	typedef boost::shared_ptr<InputManager> InputManagerPtr;
	class InputListener;
	typedef boost::shared_ptr<InputListener> InputListenerPtr;
	class Client;
	typedef boost::shared_ptr<Client> ClientPtr;
	class WindowsClient;
	typedef boost::shared_ptr<WindowsClient> WindowsClientPtr;
	class Viewport;
	typedef boost::shared_ptr<Viewport> ViewportPtr;
	class VertexElement;
	typedef boost::shared_ptr<VertexElement> VertexElementPtr;
	class Geometry;
	typedef boost::shared_ptr<Geometry> GeometryPtr;
	class GameObject;
	typedef boost::shared_ptr<GameObject> GameObjectPtr;
	class Level;
	typedef boost::shared_ptr<Level> LevelPtr;
	class MaterialParam;
	typedef boost::shared_ptr<MaterialParam> MaterialParamPtr;
	class MaterialParamInt;
	typedef boost::shared_ptr<MaterialParamInt> MaterialParamIntPtr;
	class MaterialParamFloat;
	typedef boost::shared_ptr<MaterialParamFloat> MaterialParamFloatPtr;
	class MaterialParamVector3;
	typedef boost::shared_ptr<MaterialParamVector3> MaterialParamVector3Ptr;
	class MaterialParamVector4;
	typedef boost::shared_ptr<MaterialParamVector4> MaterialParamVector4Ptr;
	class MaterialParamMatrix;
	typedef boost::shared_ptr<MaterialParamMatrix> MaterialParamMatrixPtr;
	class MaterialParamCBuffer;
	typedef boost::shared_ptr<MaterialParamCBuffer> MaterialParamCBufferPtr;
	class MaterialParamShaderRes;
	typedef boost::shared_ptr<MaterialParamShaderRes> MaterialParamShaderResPtr;
	class MaterialParamSamplerState;
	typedef boost::shared_ptr<MaterialParamSamplerState> MaterialParamSamplerStatePtr;
	class MaterialParamUnordered;
	typedef boost::shared_ptr<MaterialParamUnordered> MaterialParamUnorderedPtr;
	class MaterialParameterManager;
	typedef boost::shared_ptr<MaterialParameterManager> MaterialParameterManagerPtr;
	class Material;
	typedef boost::shared_ptr<Material> MaterialPtr;
	class Texture;
	typedef boost::shared_ptr<Texture> TexturePtr;
	class RenderEngine;
	typedef boost::shared_ptr<RenderEngine> RenderEnginePtr;
	class Renderer;
	typedef boost::shared_ptr<Renderer> RendererPtr;
	class GeometryRenderer;
	typedef boost::shared_ptr<GeometryRenderer> GeometryRendererPtr;
	class RenderView;
	typedef boost::shared_ptr<RenderView> RenderViewPtr;
	class RenderTarget;
	typedef boost::shared_ptr<RenderTarget> RenderTargetPtr;
	class RenderLayout;
	typedef boost::shared_ptr<RenderLayout> RenderLayoutPtr;
	class RenderEffect;
	typedef boost::shared_ptr<RenderEffect> RenderEffectPtr;
	class ShaderObject;
	typedef boost::shared_ptr<ShaderObject> ShaderObjectPtr;
	class RenderResourceManager;
	typedef boost::shared_ptr<RenderResourceManager> RenderResourceManagerPtr;
	class BrushRenderer;
	typedef boost::shared_ptr<BrushRenderer> BrushRendererPtr;
	class RenderBuffer;
	typedef boost::shared_ptr<RenderBuffer> RenderBufferPtr;
	class RenderTexture;
	typedef boost::shared_ptr<RenderTexture> RenderTexturePtr;
	class RenderTexture1D;
	typedef boost::shared_ptr<RenderTexture1D> RenderTexture1DPtr;
	class RenderTexture2D;
	typedef boost::shared_ptr<RenderTexture2D> RenderTexture2DPtr;
	class FontManager;
	typedef boost::shared_ptr<FontManager> FontManagerPtr;
	class Font;
	typedef boost::shared_ptr<Font> FontPtr;
	class SamplerState;
	typedef boost::shared_ptr<SamplerState> SamplerStatePtr;
	class RasterizeState;
	typedef boost::shared_ptr<RasterizeState> RasterizeStatePtr;
	class BlendState;
	typedef boost::shared_ptr<BlendState> BlendStatePtr;
	class DepthStencilState;
	typedef boost::shared_ptr<DepthStencilState> DepthStencilStatePtr;
	class Canvas;
	typedef boost::shared_ptr<Canvas> CanvasPtr;
	class FileSystem;
	typedef boost::shared_ptr<FileSystem> FileSystemPtr;
	class FileObject;
	typedef boost::shared_ptr<FileObject> FileObjectPtr;
	class SceneImporter;
	typedef boost::shared_ptr<SceneImporter> SceneImporterPtr;
	class FbxSceneImporter;
	typedef boost::shared_ptr<FbxSceneImporter> FbxSceneImporterPtr;
	class Light;
	typedef boost::shared_ptr<Light> LightPtr;


}

//project related
#include "Singleton.h"
#include "MathLib.h"
#include "Common.h"
#include "GameTime.h"
#include "Quaternion.h"
#include "Vectors.h"
#include "Plane.h"
#include "Matrixs.h"
#include "Bounds.h"
#include "FileSystem.h"
#include "MainLoop.h"
#include "InputManager.h"
#include "Component.h"
#include "Camera.h"
#include "Light.h"

#include "RenderInclude.h"
#include "Texture.h"
#include "RenderUtil.h"
#include "RenderResource.h"
#include "RenderState.h"
#include "RenderView.h"
#include "Material.h"
#include "Font.h"
#include "FontManager.h"
#include "RenderTarget.h"
#include "RenderLayout.h"
#include "Geometry.h"
#include "SceneObjectGenerator.h"
#include "Renderer.h"
#include "Canvas.h"
#include "RenderResourceManager.h"
#include "RenderEffect.h"
#include "RenderEngine.h"
#include "RenderPath.h"
#include "Viewport.h"
#include "SceneImporter.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Level.h"
#include "World.h"
#include "Client.h"
#include "Engine.h"
#include "Config.h"


 
// TODO: reference additional headers your program requires here
