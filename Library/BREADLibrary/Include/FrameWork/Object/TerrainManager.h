#pragma once
#include <map>
#include <vector>
#include <memory>

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
			using SpatialPosition = Math::Vector3S32;
			struct TerrainModel
			{
				std::map<SpatialPosition, std::vector<ModelObject::Face::VertexIndex>*> registFace;
			};
			std::map<ModelObject::Face*, TerrainModel> terrains;
		public:
			TerrainManager() = default;
			~TerrainManager() {};

		private:
			//�����̃��f������ǂ��̋�ԂɃ|���S��������̂����ׂēo�^����
			void RegisterPolygon(std::shared_ptr<ModelObject> model);

		public:
			//��ԍ��W�̃C���f�b�N�X�ԍ���n����
			//3*3*3�̋�Ԃ̃|���S���̒��_����n��
			[[nodiscard]]
			const std::vector<ModelObject::Face::VertexIndex> GetSpatialFaces(const Math::Vector3S32& index);

		};
	}//namespace FrameWork
}//namespace Bread