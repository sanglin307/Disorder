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
#include <boost/assert.hpp>
#include <boost/make_shared.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool.hpp>
#include "ComPtr.h"
#include <boost/locale.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


//Class pre declare
namespace Disorder
{
	class AABB;
	typedef boost::shared_ptr<AABB> AxisAlignedBoxPtr;
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
	class GameObject;
	typedef boost::shared_ptr<GameObject> GameObjectPtr;
	class Level;
	typedef boost::shared_ptr<Level> LevelPtr;
	class RenderEngine;
	typedef boost::shared_ptr<RenderEngine> RenderEnginePtr;
	class Renderer;
	typedef boost::shared_ptr<Renderer> RendererPtr;
	class RenderView;
	typedef boost::shared_ptr<RenderView> RenderViewPtr;
	class RenderTarget;
	typedef boost::shared_ptr<RenderTarget> RenderTargetPtr;
	class RenderLayout;
	typedef boost::shared_ptr<RenderLayout> RenderLayoutPtr;
	class RenderTechnique;
	typedef boost::shared_ptr<RenderTechnique> RenderTechniquePtr;
	class ShaderObject;
	typedef boost::shared_ptr<ShaderObject> ShaderObjectPtr;
	class VertexShader;
	typedef boost::shared_ptr<VertexShader> VertexShaderPtr;
	class PixelShader;
	typedef boost::shared_ptr<PixelShader> PixelShaderPtr;
	class RenderResourceManager;
	typedef boost::shared_ptr<RenderResourceManager> RenderResourceManagerPtr;
	class BrushRenderer;
	typedef boost::shared_ptr<BrushRenderer> BrushRendererPtr;
	class CubeRenderer;
	typedef boost::shared_ptr<CubeRenderer> CubeRendererPtr;
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
#include "AABB.h"
#include "FileSystem.h"
#include "MainLoop.h"


#include "InputManager.h"
#include "Component.h"
#include "Camera.h"
#include "Light.h"
#include "RenderUtil.h"
#include "RenderResource.h"
#include "Font.h"
#include "FontManager.h"
#include "RenderState.h"
#include "RenderTechnique.h"
#include "RenderView.h"
#include "RenderTarget.h"
#include "RenderLayout.h"
#include "Renderer.h"
#include "Canvas.h"
#include "RenderResourceManager.h"
#include "RenderEngine.h"
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
