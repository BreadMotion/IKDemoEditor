#pragma once
#include <vector>
#include <string>

#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		//�S�ẴW���C���g�̊�ՂƂȂ�\����
		struct IJoint
		{
			std::string          name;        //�W���C���g�̖��O
			IJoint*              parent;      //�e�W���C���g�̃|�C���^�[
			std::vector<IJoint*> child;       //�q�W���C���g�̃|�C���^�[�z��

			Math::Vector3      scale;         //�W���C���g�̃X�P�[���l
			Math::Quaternion   rotate;        //�W���C���g�̉�]�l
			Math::Vector3      translate;     //�W���C���g�̕��s�ړ��l

			Math::Matrix       localTransform;//�W���C���g�̃��[�J���s��
			Math::Matrix       worldTransform;//�W���C���g�̃��[���h�s��

			Math::Vector3      minRot;        //�W���C���g�̍ŏ���]��(Euler)
			Math::Vector3      maxRot;        //�W���C���g�̍ő��]��(Euler)
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