#ifndef _DISORDER_MATERIAL_H_
#define _DISORDER_MATERIAL_H_


namespace Disorder
{
	// fbx means...
	enum MaterialType
	{
		MT_Phong,
		MT_Lambert,
		MT_HLSL,
		MT_CGFX,
		MT_MentalRay,
		MT_None
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
	public :
		virtual void* GetData() = 0;

	};

	class MaterialParamInt : public MaterialParam
	{
	public:

		MaterialParamInt()
		{
			_value = 0;
		}

		void SetValue(int value)
		{
			_value = value;
		}

		int GetValue()
		{
			return _value;
		}

        virtual void* GetData()
        {
			return (void*)&_value;
        }

	private:
		int _value;
	};

	class MaterialParamFloat : public MaterialParam
	{
	public:

		MaterialParamFloat()
		{
			_value = 0.0f;
		}

		void SetValue(float value)
		{
			_value = value;
		}

		float GetValue()
		{
			return _value;
		}

        virtual void* GetData()
        {
			return (void*)&_value;
        }

	private:
		float _value;
	};

	class MaterialParamVector4 : public MaterialParam
	{
	public:

		MaterialParamVector4()
		{
		   _vec = Vector3::ZERO;
		}

		void SetValue(Vector4 const& vec)
		{
			_vec = vec;
		}

		Vector4 GetValue()
		{
			return _vec;
		}

        virtual void* GetData()
        {
            return (void*)&_vec;
        }

	private:
		Vector4 _vec;
	};

	class MaterialParamVector3 : public MaterialParam
	{
	public:

		MaterialParamVector3()
		{
		   _vec = Vector3::ZERO;
		}

		void SetValue(Vector3 const& vec)
		{
			_vec = vec;
		}

		Vector3 GetValue()
		{
			return _vec;
		}

        virtual void* GetData()
        {
            return (void*)&_vec;
        }

	private:
		Vector3 _vec;
	};

	class MaterialParamMatrix : public MaterialParam
	{
	public :
		void SetValue(Matrix4 const& mat)
		{
			_mat = mat;
		}

		Matrix4 GetValue()
		{
			return _mat;
		}
        
        virtual void* GetData()
        {
            return (void*)&_mat;
        }

	private:
		Matrix4 _mat;
	};

	class MaterialParamCBuffer : public MaterialParam
	{
	public:
		void SetValue(RenderBufferPtr const& buffer)
		{
			_constBuffer = buffer;
		}

		RenderBufferPtr GetValue()
		{
			return _constBuffer;
		}

        virtual void* GetData()
        {
            return _constBuffer->GetLowInterface();
        }

	private:
		RenderBufferPtr _constBuffer;
	};

	class MaterialParamShaderRes : public MaterialParam  // texture and structed thing.
	{
	public:
		void SetValue(RenderViewPtr const& view)
		{
			_renderView = view;
		}

		RenderViewPtr GetValue()
		{
			return _renderView;
		}

        virtual void* GetData()
        {
            return _renderView->GetLowInterface();
        }
	private :
		RenderViewPtr _renderView;
	};

	class MaterialParamSamplerState : public MaterialParam
	{
	public:
		void SetValue(SamplerStatePtr const& sampler)
		{
			_sampler = sampler;
		}

		SamplerStatePtr GetValue()
		{
			return _sampler;
		}

        virtual void* GetData()
        {
            return _sampler->GetLowInterface();
        }
	private:
		SamplerStatePtr _sampler;
	};

	class MaterialParamUnordered : public MaterialParam  // Unordered view
	{
	public:
		void SetValue(RenderViewPtr const& view)
		{
			_renderView = view;
		}

		RenderViewPtr GetValue()
		{
			return _renderView;
		}
        
        virtual void* GetData()
        {
            return _renderView->GetLowInterface();
        }

	private :
		RenderViewPtr _renderView;
	};

	class Material
	{
	public:
		Material();

		MaterialType Type;
		std::string ShaderModel;

		//Common property
		Vector3 AmbientColor;
		Vector3 DiffuseColor;
		Vector3 SpecularColor;
		Vector3 EmissiveColor;
		double Opacity;
		double Shininess;
		double Reflectivity;
 
		//effect
		RenderEffectPtr Effect[MVT_NUM_VIEW_TYPES];

		std::string Name;
	};

	class MaterialGenerator
	{
	public:
		static MaterialPtr GenerateLambert(Vector3 const& ambientColor,Vector3 const& diffuseColor);

	};

}


#endif