#include "EngineInclude.h"


namespace Disorder
{
	Transform::Transform()
	{
		_locPos = Vector3::ZERO;
		_locRot = Vector3::ZERO;
		_locScale = Vector3::ZERO;
		_wldPos = _locPos;
		_wldRot = _locRot;
		_wldScale = _locScale;

		_worldMatrix.MakeTransform(_wldPos,_wldScale,_wldRot);
	}


	Transform::Transform(Vector3 const& pos,Vector3 const& rot,Vector3 const& scale)
	{
		_locPos = Vector3::ZERO;
		_locRot = Vector3::ZERO;
		_locScale = Vector3::ZERO;
		_wldPos = _locPos;
		_wldRot = _locRot;
		_wldScale = _locScale;

		_worldMatrix.MakeTransform(_wldPos,_wldScale,_wldRot);
	}

	void Transform::SetLocalRotation(float pitch,float yaw,float roll)
	{
		_locRot.x = pitch;
		_locRot.y = yaw;
		_locRot.z = roll;
		RefreshWorldTransform();
	}
 
	void Transform::SetLocalPosition(Vector3 const& position)
	{
		_locPos = position;
		RefreshWorldTransform();
	}

	void Transform::SetLocalScale(Vector3 const& scale)
	{
		_locScale = scale;
		RefreshWorldTransform();
	}
 
	void Transform::RefreshWorldTransform()
	{
		_worldMatrix.MakeTransform(_locPos,_locScale,_locRot);
		TransformPtr parent = _parent.lock();
		if(parent != NULL )
		{
			const Matrix4 parentMatrx = parent->GetWorldMatrix();
			_wldPos = parentMatrx * _locPos;
			_wldRot = parentMatrx * _locRot;
			_wldScale = parentMatrx * _locScale;
			_worldMatrix = parentMatrx * _worldMatrix;
			
		}
		else
		{
			_wldPos = _locPos;
			_wldRot = _locRot;
			_wldScale = _locScale;
		}

		for(int i=0;i<_vChildren.size();i++)
		{
			_vChildren[i]->RefreshWorldTransform();
		}
	}

	void Transform::RefreshLocalTransform()
	{
		_worldMatrix.MakeTransform(_wldPos,_wldScale,_wldRot);
		TransformPtr parent = _parent.lock();
		if(parent != NULL )
		{
			const Matrix4 InverseMatrx = parent->GetWorldMatrix().Inverse();
			_locPos = InverseMatrx * _wldPos;
			_locRot = InverseMatrx * _wldRot;
			_locScale = InverseMatrx * _wldScale;
		}
		else
		{
			_locPos =  _wldPos;
			_locRot = _wldRot;
			_locScale =  _wldScale;
		}

		for(int i=0;i<_vChildren.size();i++)
		{
			_vChildren[i]->RefreshWorldTransform();
		}
	}

	void Transform::SetWorldRotation(float pitch,float yaw,float roll)
	{
		 
		_wldRot.x = pitch;
		_wldRot.y = yaw;
		_wldRot.z = roll;

		RefreshLocalTransform();

	}
 
	void Transform::SetWorldPosition(Vector3 const& position)
	{
		_wldPos = position;
		RefreshLocalTransform();
	}

	void Transform::SetWorldScale(Vector3 const& scale)
	{
		_wldScale = scale;
		RefreshLocalTransform();
	}

	void Transform::AddChild(TransformPtr const& child)
	{
		AddChild(child,child->GetLocalPosition(),child->GetLocalRotation(),child->GetLocalScale());
	}

	void Transform::AddChild(TransformPtr const& child,Vector3 const& pos,Vector3 const& rot,Vector3 const& scale)
	{
		child->SetParent(shared_from_this());
		child->SetLocalPosition(pos);
		child->SetLocalRotation(rot.x,rot.y,rot.z);
		child->SetLocalScale(scale);
		_vChildren.push_back(child);
	}

}