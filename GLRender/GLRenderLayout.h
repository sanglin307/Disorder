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
		GLRenderLayout(RenderEffect* renderEffect,TopologyType topologyType,bool soloBuffer);
 
		virtual void * GetHandle();
		virtual void FinishBufferBinding(RenderEffect* renderEffect);

		static GLint GetGLTypeSize(GLint type);
	private:

		GLuint _VAOHandle;
	};
}

#endif