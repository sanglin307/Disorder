#include "EngineInclude.h"

namespace Disorder
{
	Skybox::Skybox()
	{ 
		ShaderPropertyManager* globalProperty = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_skyTextureProperty = globalProperty->CreateProperty(ShaderPropertyManager::sSkyboxTexture, eSP_ShaderResource, 1);
		_skySamplerProperty = globalProperty->CreateProperty(ShaderPropertyManager::sSkyboxSampler, eSP_SampleState, 1);

		std::string skyTextureName = "early_morning_";
		std::vector<Image*> textureArray;
		textureArray.push_back(GImageManager->Load(GConfig->ResourceTexPath + skyTextureName + "RT.jpg"));
		textureArray.push_back(GImageManager->Load(GConfig->ResourceTexPath + skyTextureName + "LF.jpg"));
		textureArray.push_back(GImageManager->Load(GConfig->ResourceTexPath + skyTextureName + "UP.jpg"));
		textureArray.push_back(GImageManager->Load(GConfig->ResourceTexPath + skyTextureName + "DN.jpg"));
		textureArray.push_back(GImageManager->Load(GConfig->ResourceTexPath + skyTextureName + "FR.jpg"));
		textureArray.push_back(GImageManager->Load(GConfig->ResourceTexPath + skyTextureName + "BK.jpg"));
		
		SamplerDesc sdesc;
		sdesc.Filter = SF_Anisotropic;
		sdesc.MaxAnisotropy = 16;
		_skyCubeSampler = GRenderResourceMgr->CreateSamplerState(&sdesc);
		_skyTexture = GRenderResourceMgr->CreateTexture2D("skyboxcubetex",_skyCubeSampler, PF_R8G8B8A8_TYPELESS, false, textureArray, SF_AsCubeMap);
		_skyCubeView = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _skyTexture, PF_R8G8B8A8_UNORM, SF_AsCubeMap);
		_skyTextureProperty->SetData(_skyCubeView);
		_skySamplerProperty->SetData(_skyCubeSampler);

		_renderEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "Skybox", SM_4_0, "VS");
		ShaderObject* pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "Skybox", SM_4_0, "PS");
		_renderEffect->BindShader(vertexShader);
		_renderEffect->BindShader(pixelShader);
		_renderEffect->LinkShaders();

		DepthStencilDesc desc;
		desc.DepthWrite = false;
		desc.DepthEnable = false;
		DepthStencilState* depthPtr = GRenderResourceMgr->CreateDepthStencilState(&desc, 0);
		_renderEffect->BindDepthStencilState(depthPtr);

		glm::vec3 positions[] = 
		{
			glm::vec3(-1, -1, 1), glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(1,-1,1),   // front
			glm::vec3(-1, 1, -1), glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, 1), glm::vec3(-1,-1,-1), // left
			glm::vec3(1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1), glm::vec3(1, -1, 1), glm::vec3(1, 1, -1), glm::vec3(1,-1,-1), //right
			glm::vec3(1, -1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, 1, -1), glm::vec3(-1,-1,-1), // back
			glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(1, 1, 1), glm::vec3(-1, 1, -1), glm::vec3(1, 1, 1), glm::vec3(-1,1,1), // top
			glm::vec3(1, -1, 1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, -1), glm::vec3(-1,-1,1) // bottom
		};

		_renderLayout = GRenderResourceMgr->CreateRenderLayout(_renderEffect, TT_TriangleList,true);
		RenderBuffer* vertexBuffer = GRenderResourceMgr->CreateBuffer("SkyBoxVertex",RBT_Vertex, BU_StaticDraw, sizeof(glm::vec3), sizeof(glm::vec3)*36, positions);
		_renderLayout->BindVertexBuffer(vertexBuffer);

		_renderLayout->FinishBufferBinding(_renderEffect);
	}

	void Skybox::Render()
	{
		GRenderEngine->SetRenderLayout(_renderLayout);
		GRenderEngine->SetEffect(_renderEffect);
		GRenderEngine->Draw(36, 0);
	}
}