#include "EngineInclude.h"

namespace Disorder
{
	std::string RenderLayout::GenerateMapKey(const std::vector<VertexInputDes> const& vertexInput,TopologyType topologyType)
	{
		// we use format: vertexArraySize:Semantic,Format,InstanceDataStepRate,InstanceData:TopologyType
		std::stringstream result;

		result << "RenderLayout:" << vertexInput.size() <<':';
		for(unsigned int index=0;index<vertexInput.size();index++)
		{
			result << vertexInput[index]->Semantic<<',';
			result << vertexInput[index]->Format<<',';
			result << vertexInput[index]->InstanceDataStepRate<<',';
			result << vertexInput[index]->InstanceData<<':';
		}

		result << topologyType;

	 
		return result.str();
 
	}
}