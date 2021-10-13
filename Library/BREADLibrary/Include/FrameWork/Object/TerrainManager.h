#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>

#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FND/Base.h"

namespace Bread
{
	namespace FrameWork
	{
		//�n�ʔ���̃I�u�W�F�N�g�̃��b�V�����ǂ��̋�Ԃɂ���̂����Ǘ�����N���X
		class TerrainManager : public FND::Base
		{
			struct TerrainModel
			{
				std::map<std::string, std::vector<ModelObject::Face::VertexIndex>> registFace;
			};
			std::map<std::shared_ptr<Actor>, TerrainModel> terrains;
		public:
			TerrainManager() = default;
			~TerrainManager() {};

		public:
			//�����̃��f���R���|�[�l���g����ǂ��̋�ԂɃ|���S��������̂����ׂēo�^����
			void RegisterPolygon(std::shared_ptr<Actor> model);

		public:
			//��ԍ��W�̃C���f�b�N�X�ԍ���n����
			//3*3*3�̋�Ԃ̃|���S���̃��[���h��Ԃł̒��_����n��
			[[nodiscard]]
			const std::vector<ModelObject::Face::VertexIndex> GetSpatialFaces(const Math::Vector3S32& index);

		public:
			void GUi();

		private:
			std::string toStringFromSpatialIndex(const Math::Vector3S32& index);
		};
	}//namespace FrameWork
}//namespace Bread