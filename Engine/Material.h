#ifndef _DISORDER_MATERIAL_H_
#define _DISORDER_MATERIAL_H_


namespace Disorder
{
	

	class MaterialParam
	{
	public :
		virtual void* GetData() = 0;
		virtual int GetElementNumber() = 0;

		int MaxElementNumber;
	};

	class MaterialParamInt : public MaterialParam
	{
	public:

		MaterialParamInt()
		{
			MaxElementNumber = 0;
		}

		void SetValue(int value)
		{
			if(_value.size() == 0 )
				_value.push_back(value);
			else
				_value[0] = value;
		}

		void SetValueArray(std::vector<int> const& vec)
		{
			BOOST_ASSERT(vec.size()<=MaxElementNumber);
			_value = vec;
		}

		virtual int GetElementNumber()
		{
			return _value.size();
		}
 
        virtual void* GetData()
        {
			return (void*)(_value.data());
        }

	private:
		std::vector<int> _value;
	};

	class MaterialParamFloat : public MaterialParam
	{
	public:

		MaterialParamFloat()
		{
			MaxElementNumber = 0;
		}
 
		void SetValue(float value)
		{
			if(_value.size() == 0 )
				_value.push_back(value);
			else
				_value[0] = value;
		}

		void SetValueArray(std::vector<float> const& vec)
		{
			BOOST_ASSERT(vec.size()<=MaxElementNumber);
			_value = vec;
		}

		virtual int GetElementNumber()
		{
			return _value.size();
		}
 
        virtual void* GetData()
        {
			return (void*)(_value.data());
        }

	private:
		std::vector<float> _value;
	};

	class MaterialParamVector4 : public MaterialParam
	{
	public:

		MaterialParamVector4()
		{
			MaxElementNumber = 0;
		}

		void SetValue(Vector4 const& vec)
		{
			if(_vec.size() == 0 )
				_vec.push_back(vec);
			else
				_vec[0] = vec;
		}

		void SetValueArray(std::vector<Vector4> const& vec)
		{
			BOOST_ASSERT(vec.size()<=MaxElementNumber);
			_vec = vec;
		}

		virtual int GetElementNumber()
		{
			return _vec.size();
		}
 
        virtual void* GetData()
        {
			return (void*)(_vec.data());
        }

	private:
		std::vector<Vector4> _vec;
	};

 
	class MaterialParamVector3 : public MaterialParam
	{
	public:

		MaterialParamVector3()
		{
			MaxElementNumber = 0;
		}

	    void SetValue(Vector3 const& vec)
		{
			if(_vec.size() == 0 )
				_vec.push_back(vec);
			else
				_vec[0] = vec;
		}

		void SetValueArray(std::vector<Vector3> const& vec)
		{
			BOOST_ASSERT(vec.size()<=MaxElementNumber);
			_vec = vec;
		}

		virtual int GetElementNumber()
		{
			return _vec.size();
		}
 
        virtual void* GetData()
        {
			return (void*)(_vec.data());
        }

	private:
		std::vector<Vector3> _vec;
	};

	class MaterialParamMatrix : public MaterialParam
	{
	public :

		MaterialParamMatrix()
		{
			MaxElementNumber = 0;
		}

		void SetValue(Matrix4 const& mat)
		{
			if(_mat.size() == 0 )
				_mat.push_back(mat);
			else
				_mat[0] = mat;
		}
 
		void SetValueArray(std::vector<Matrix4> const& vec)
		{
			BOOST_ASSERT(vec.size()<=MaxElementNumber);
			_mat = vec;
		}

		virtual int GetElementNumber()
		{
			return _mat.size();
		}
        
        virtual void* GetData()
        {
			return (void*)(_mat.data());
        }

	private:
		std::vector<Matrix4> _mat;
	};

	class MaterialParamCBuffer : public MaterialParam
	{
	public:

		MaterialParamCBuffer()
		{
			MaxElementNumber = 1;
		}


		void SetValue(RenderBufferPtr const& buffer)
		{
			_constBuffer = buffer;
		}

		RenderBufferPtr GetValue()
		{
			return _constBuffer;
		}

		virtual int GetElementNumber()
		{
			return 1;
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

		MaterialParamShaderRes()
		{
			MaxElementNumber = 1;
		}


		void SetValue(RenderViewPtr const& view)
		{
			_renderView = view;
		}

		RenderViewPtr GetValue()
		{
			return _renderView;
		}

		virtual int GetElementNumber()
		{
			return 1;
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

		MaterialParamSamplerState()
		{
			MaxElementNumber = 1;
		}

		void SetValue(SamplerStatePtr const& sampler)
		{
			_sampler = sampler;
		}

		SamplerStatePtr GetValue()
		{
			return _sampler;
		}

		virtual int GetElementNumber()
		{
			return 1;
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

		MaterialParamUnordered()
		{
			MaxElementNumber = 1;
		}

		void SetValue(RenderViewPtr const& view)
		{
			_renderView = view;
		}

		RenderViewPtr GetValue()
		{
			return _renderView;
		}

		virtual int GetElementNumber()
		{
			return 1;
		}
        
        virtual void* GetData()
        {
            return _renderView->GetLowInterface();
        }

	private :
		RenderViewPtr _renderView;
	};

	class MaterialParameterManager
	{
		typedef boost::unordered_map<std::string,MaterialParamPtr> MaterialParamMap;

	public:
		// Material Param
		MaterialParamVector3Ptr GetVector3Parameter(std::string const& name);
		MaterialParamVector4Ptr GetVector4Parameter(std::string const& name);
		MaterialParamIntPtr GetIntParameter(std::string const& name);
		MaterialParamFloatPtr GetFloatParameter(std::string const& name);
		MaterialParamMatrixPtr GetMatrixParameter(std::string const& name);

	private:
		MaterialParamMap _materialParamMap;

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
		float Opacity;
		float Shininess;
		float Reflectivity;
 
		void UpdateMaterialParameters(GameObjectPtr const& gameObject,CameraPtr const& camera);

		//material parameters
		//transform
		MaterialParamMatrixPtr WorldViewProjMatrix;
		MaterialParamMatrixPtr WorldMatrix;
		MaterialParamMatrixPtr ViewMatrix;
		MaterialParamMatrixPtr ProjMatrix;
		MaterialParamMatrixPtr WorldNormal;

		MaterialParamVector3Ptr CameraPosition;
		MaterialParamVector4Ptr AmbientColorParam;
		MaterialParamVector4Ptr DiffuseColorParam;
		MaterialParamVector4Ptr SpecularColorParam;
		MaterialParamVector4Ptr EmissiveColorParam;
		MaterialParamFloatPtr OpacityParam;
		MaterialParamFloatPtr ShininessParam;
		MaterialParamFloatPtr RelectivityParam;

		// light array
		MaterialParamIntPtr LightNumber;
		MaterialParamVector4Ptr LightIntensityPack;
		MaterialParamVector3Ptr LightDirArray;
		MaterialParamVector3Ptr LightColorArray;
 
		// for each light
		MaterialParamIntPtr    LightType;
	    MaterialParamFloatPtr  LightIntensity;
	    MaterialParamVector3Ptr LightPos;
	    MaterialParamVector3Ptr LightColor;

		//effect
		RenderEffectPtr Effect[RPT_MaxRenderPath][MaxMaterialPassNumber];
 
		std::string Name;
 
	};

	class MaterialGenerator
	{
	public:
		static MaterialPtr GenerateLambert(Vector3 const& ambientColor,Vector3 const& diffuseColor);
		static MaterialPtr GeneratePhong(Vector3 const& ambientColor,Vector3 const& diffuseColor,Vector3 const& specularColor,float shininess);


	};

}


#endif