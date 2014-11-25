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
#include <boost/assert.hpp>
#define BOOST_FILESYSTEM_STATIC_LINK
#include <boost/filesystem.hpp>
//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_generators.hpp>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/pool/pool.hpp>
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
	class CameraUpdateStrategy;
	class CameraFirstPersonUpdate;
	class CameraSphereTargetUpdate;
	class Camera;
	class InputEventHandler;
	class InputManager;
	class InputListener;
	class Console;
	class Client;
	class WindowsClient;
	class Viewport;
	class Image;
	class VertexElement;
	class Geometry;
	class GameObject;
	class Level;
	class ShaderProperty;
	class ShaderPropertyManager;
	class Texture;
	class SurfaceMaterial;
	class RenderEngine;
	class Renderer;
	class GeometryRenderer;
	class RenderLayout;
	class RenderEffect;
	class ShaderObject;
	class RenderResourceManager;
	class BrushRenderer;
	class RenderBuffer;
	class RenderTexture;
	class RenderTexture1D;
	class RenderTexture2D;
	class RenderTexture3D;
	class SurfaceView;
	class RenderSurface;
	class SimpleTile;
	class MainRenderTarget;
	class RenderGBuffer;
	class RenderSurfaceCache;
	class ShadowMap;
	class FXAA;
	class RenderPath;
	class ForwardRenderPath;
	class DeferredShading;
	class FontManager;
	class Font;
	class Skybox;
	class SamplerState;
	class RasterizeState;
	class BlendState;
	class DepthStencilState;
	class Canvas;
	class FileSystem;
	class FileObject;
	class SceneImporter;
	class FbxSceneImporter;
	class Light;
	class DirectionLight;
	class PointLight;
	class SpotLight;
	class Console;
	class Config;
	class Engine;
	class ImageManager;
	class MainLoop;
	class SceneManager;
	class Logger;
	class World;


}

//project related
#include "SHA1.h"
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
#include "Renderer.h"
#include "RenderSurface.h"
#include "Material.h"
#include "Font.h"
#include "FontManager.h"
#include "Skybox.h"
#include "RenderLayout.h"
#include "Geometry.h"
#include "Canvas.h"
#include "RenderResourceManager.h"
#include "RenderEffect.h"
#include "RenderEngine.h"
#include "ShadowMap.h"
#include "PostProcess.h"
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
