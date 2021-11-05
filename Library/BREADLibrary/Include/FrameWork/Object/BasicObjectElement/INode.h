#pragma once
#include <vector>
#include <string>
#include <memory>

#include "FND/DirtyFlag.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		class Actor;
		class ModelObject;
		class Transform;

		struct ITransform
		{
		public:
			ITransform*              parent{nullptr};     //�e�W���C���g�̃|�C���^�[
			std::vector<ITransform*> child;               //�q�W���C���g�̃|�C���^�[�z��

			Math::Vector3    scale;     //�W���C���g�̃X�P�[���l
			Math::Quaternion rotate;    //�W���C���g�̉�]�l
			Math::Vector3    translate; //�W���C���g�̕��s�ړ��l

			Math::Matrix     localTransform;
			Math::Matrix     worldTransform;
		};

		//�S�ẴW���C���g�̊�ՂƂȂ�\����
		struct IJoint : public ITransform
		{
			std::string        name;        //�W���C���g�̖��O

			Math::Vector3      minRot;        //�W���C���g�̍ŏ���]��(Euler)
			Math::Vector3      maxRot;        //�W���C���g�̍ő��]��(Euler)
		};

		//����Ń��f���̃W���C���g���m��g�ݍ��킹�ăI�u�W�F�N�g���\�z����N���X
		struct IJointAssembly
		{
			std::vector<ITransform*> joins;

			template <class Component>
			std::shared_ptr<Actor> AddJoint(std::shared_ptr<Actor> owner)
			{
				//�q�A�N�^�[�̐���
				auto childActor{ owner->AddChildActor<Actor>() };
				{//�q�A�N�^�[�̐ݒ�y�уR���|�[�l���g�̒ǉ�
					childActor->AddComponent<Component>();

					//�e�A�N�^�[�̃��f���Ǝq�A�N�^�[�̃��f���̃��[�g�W���C���g�̐e�q�֌W���\�z
					auto& childJoint{ childActor->GetComponent<ModelObject>()->GetNodes()->at(0) };

					//���߂Ă̒ǉ�����Ȃ���ΐe�����݂���̂Őe�q�֌W���\�z
					if (joins.size() > 0)
					{
						childJoint.parent = joins.back();
						joins.back()->child.emplace_back(childJoint);
					}
					//�Ō���ɒǉ�
					joins.emplace_back(childJoint);
				}

				//��̃I�u�W�F�N�g�Ƃ��ď����ł���悤�ɓo�^����
				return childActor;
			}
		};

		//�S�Ẵ|���S���̊�b�ƂȂ�\����
		struct IFace
		{
			struct VertexIndex
			{
				std::vector<Math::Vector3> vertex; //�|���S���̒��_���W
			};
			std::vector<VertexIndex>       face;   //�|���S��
		};

		struct IMeshNode
		{
			std::vector<Math::Matrix> boneTransform;
			u32 boneTransformCount = 0;
		};

		//�S�ẴV�F�[�_�[�̖��O���i�[����\����
		struct IShaderName
		{
		protected:
			std::string shaderName;   //�`�悷��V�F�[�_�̖��O
		};

		//RenderManager�Ŏg�p����V�F�[�_�[�𔻕ʂ��邽��
		struct MyShaderName : public IShaderName
		{
		public:
			void SetShaderNema(const std::string& name)
			{
				shaderName = name;
			}
			const std::string& GetShaderName()
			{
				return shaderName;
			}
		};
	}
}