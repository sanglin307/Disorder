// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_WARNINGS 
#include <Windows.h>
extern HINSTANCE GAppInstance;
#endif

#undef min 
#undef max

//std lib
#include <stdlib.h> 
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
#define BOOST_FILESYSTEM_STATIC_LINK
#include <boost/filesystem.hpp>
//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_generators.hpp>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool.hpp>
#include "ComPtr.h"
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H


//GLM
#ifdef _DEBUG
//#define GLM_MESSAGES
#endif
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

//Class pre declare
namespace Disorder
{
	class Component;
	typedef boost::shared_ptr<Component> ComponentPtr;
	class CameraUpdateStrategy;
	typedef boost::shared_ptr<CameraUpdateStrategy> CameraUpdateStrategyPtr;
	class CameraFirstPersonUpdate;
	typedef boost::shared_ptr<CameraFirstPersonUpdate> CameraFirstPersonUpdatePtr;
	class CameraSphereTargetUpdate;
	typedef boost::shared_ptr<CameraSphereTargetUpdate> CameraSphereTargetUpdatePtr;
	class Camera;
	typedef boost::shared_ptr<Camera> CameraPtr;
	class InputEventHandler;
	typedef boost::shared_ptr<InputEventHandler> InputEventHandlerPtr;
	class InputManager;
	typedef boost::shared_ptr<InputManager> InputManagerPtr;
	class InputListener;
	typedef boost::shared_ptr<InputListener> InputListenerPtr;
	class Console;
	typedef boost::shared_ptr<Console> ConsolePtr;
	class Client;
	typedef boost::shared_ptr<Client> ClientPtr;
	class WindowsClient;
	typedef boost::shared_ptr<WindowsClient> WindowsClientPtr;
	class Viewport;
	typedef boost::shared_ptr<Viewport> ViewportPtr;
	class Image;
	typedef boost::shared_ptr<Image> ImagePtr;
	class VertexElement;
	typedef boost::shared_ptr<VertexElement> VertexElementPtr;
	class Geometry;
	typedef boost::shared_ptr<Geometry> GeometryPtr;
	class GameObject;
	typedef boost::shared_ptr<GameObject> GameObjectPtr;
	class Level;
	typedef boost::shared_ptr<Level> LevelPtr;
	class ShaderProperty;
	typedef boost::shared_ptr<ShaderProperty> ShaderPropertyPtr;
	class ShaderPropertyManager;
	typedef boost::shared_ptr<ShaderPropertyManager> ShaderPropertyManagerPtr;
	class Texture;
	typedef boost::shared_ptr<Texture> TexturePtr;
	class SurfaceMaterial;
	typedef boost::shared_ptr<SurfaceMaterial> SurfaceMaterialPtr;
	class RenderEngine;
	typedef boost::shared_ptr<RenderEngine> RenderEnginePtr;
	class Renderer;
	typedef boost::shared_ptr<Renderer> RendererPtr;
	class GeometryRenderer;
	typedef boost::shared_ptr<GeometryRenderer> GeometryRendererPtr;
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
	class RenderTexture3D;
	typedef boost::shared_ptr<RenderTexture3D> RenderTexture3DPtr;
	class SurfaceView;
	typedef boost::shared_ptr<SurfaceView> SurfaceViewPtr;
	class RenderSurface;
	typedef boost::shared_ptr<RenderSurface> RenderSurfacePtr;
	class RenderGBuffer;
	typedef boost::shared_ptr<RenderGBuffer> RenderGBufferPtr;
	class RenderSurfaceCache;
	typedef boost::shared_ptr<RenderSurfaceCache> RenderSurfaceCachePtr;
	class ShadowMap;
	typedef boost::shared_ptr<ShadowMap> ShadowMapPtr;
	class RenderPath;
	typedef boost::shared_ptr<RenderPath> RenderPathPtr;
	class ForwardRenderPath;
	typedef boost::shared_ptr<ForwardRenderPath> ForwardRenderPathPtr;
	class DeferredShading;
	typedef boost::shared_ptr<DeferredShading> DeferredShadingPtr;
	class FontManager;
	typedef boost::shared_ptr<FontManager> FontManagerPtr;
	class Font;
	typedef boost::shared_ptr<Font> FontPtr;
	class Skybox;
	typedef boost::shared_ptr<Skybox> SkyboxPtr;
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
	class DirectionLight;
	typedef boost::shared_ptr<DirectionLight> DirectionLightPtr;
	class PointLight;
	typedef boost::shared_ptr<PointLight> PointLightPtr;
	class SpotLight;
	typedef boost::shared_ptr<SpotLight> SpotLightPtr;
	class Console;
	typedef boost::shared_ptr<Console> ConsolePtr;
	class Config;
	typedef boost::shared_ptr<Config> ConfigPtr;
	class Engine;
	typedef boost::shared_ptr<Engine> EnginePtr;
	class ImageManager;
	typedef boost::shared_ptr<ImageManager> ImageManagerPtr;
	class MainLoop;
	typedef boost::shared_ptr<MainLoop> MainLoopPtr;
	class SceneManager;
	typedef boost::shared_ptr<SceneManager> SceneManagerPtr;
	class Logger;
	typedef boost::shared_ptr<Logger> LoggerPtr;
	class World;
	typedef boost::shared_ptr<World> WorldPtr;


}

//project related
#include "SHA1.h"
#include "Singleton.h"
#include "Plane.h"
#include "MathLib.h"
#include "Common.h"
#include "GameTime.h"
#include "Bounds.h"
#include "Frustrum.h"
#include "FileSystem.h"
#include "MainLoop.h"
#include "InputManager.h"
#include "Component.h"
#include "Camera.h"
#include "Light.h"
#include "RenderDefine.h"
#include "Image.h"
#include "VertexFormat.h"
#include "ShaderProperty.h"
#include "RenderResource.h"
#include "RenderState.h"
#include "RenderSurface.h"
#include "Material.h"
#include "Font.h"
#include "FontManager.h"
#include "Skybox.h"
#include "RenderLayout.h"
#include "Geometry.h"
#include "Renderer.h"
#include "Canvas.h"
#include "RenderResourceManager.h"
#include "RenderEffect.h"
#include "RenderEngine.h"
#include "ShadowMap.h"
#include "RenderPath.h"
#include "Viewport.h"
#include "SceneImporter.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Level.h"
#include "World.h"
#include "Console.h"
#include "Client.h"
#include "Engine.h"
#include "Config.h"

#include "Logger.h"
 
// TODO: reference additional headers your program requires here
