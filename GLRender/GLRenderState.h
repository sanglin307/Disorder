#ifndef _DISORDER_GLRENDERSTATE_H_
#define _DISORDER_GLRENDERSTATE_H_

namespace Disorder
{
	class GLSamplerState : public SamplerState
	{
	public:
		~GLSamplerState();
		static GLSamplerStatePtr Create(SamplerDesc* pSamplerDesc);
		virtual void* GetLowInterface();


	protected:
		GLSamplerState();
		GLuint _stateHandle;
	};

}

#endif