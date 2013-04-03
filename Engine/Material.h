#ifndef _DISORDER_MATERIAL_H_
#define _DISORDER_MATERIAL_H_


namespace Disorder
{
	enum MaterialType
	{
		MT_Lambert,
	};

	enum MaterialViewType
	{
		MVT_Perspective,
		MVT_Depth_Normal,
		MVT_GBuffer,
		MVT_Lights,
        MVT_FinalPass,
		MVT_NUM_VIEW_TYPES
	};    

	class MaterialParam
	{
	};

	class MaterialParamVector : public MaterialParam
	{
	};

	class MaterialParamTexture : public MaterialParam
	{
	};


	class Material
	{
	public:
		MaterialType Type;

		//Common property
		MaterialParamPtr DiffuseColor;
		MaterialParamPtr SpecularColor;


		//effect
		RenderEffectPtr Effect[MVT_NUM_VIEW_TYPES];


	};

	class MaterialGenerator
	{
	public:
		static MaterialPtr GenerateLambert();

	};

}


#endif