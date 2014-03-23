#ifndef _DISORDER_GLRENDERLAYOUT_H_
#define _DISORDER_GLRENDERLAYOUT_H_

namespace Disorder
{

	// equal to OpenGL's Vertex Array Object(VAO)
	class GLRenderLayout : public RenderLayout
	{
	public:

		static GLint Location_Position;
		static GLint Location_Color;
		static GLint Location_Normal;
		static GLint Location_Tex0;

		~GLRenderLayout();

		static GLRenderLayoutPtr Create(RenderEffectPtr const& renderEffect,TopologyType topologyType,bool soloBuffer);
 
		virtual void * GetLowInterface();
		virtual void FinishBufferBinding(RenderEffectPtr const& renderEffect);

		static GLint GetGLTypeSize(GLint type);
	public:
 

	private:
		GLRenderLayout(){}

		GLuint _VAOHandle;
	};
}

#endif