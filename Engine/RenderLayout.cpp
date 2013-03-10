#include "EngineInclude.h"

namespace Disorder
{
	std::string RenderLayout::GenerateMapKey(const VertexInputDes* pVertexInputDes,unsigned int vertexInputDesLength,TopologyType topologyType)
	{
		// we use format: vertexArraySize:Semantic,Format,InstanceDataStepRate,InstanceData:TopologyType
		std::stringstream result;

		result << "RenderLayout:" << vertexInputDesLength <<':';
		for(unsigned int index=0;index<vertexInputDesLength;index++)
		{
			result << pVertexInputDes->Semantic<<',';
			result << pVertexInputDes->Format<<',';
			result << pVertexInputDes->InstanceDataStepRate<<',';
			result << pVertexInputDes->InstanceData<<':';
			pVertexInputDes++;
		}

		result << topologyType;

	 
		return result.str();
 
	}
}