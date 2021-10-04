#pragma once
#include <map>
#include <vector>
#include <memory>

#include "Math/BreadMath.h"
#include "FND/Base.h"

namespace Bread
{
	namespace FrameWork
	{
		class TerrainManager:public FND::Base
		{
			using SpatialPosition = Math::Vector3S32;
			struct TerrainModel
			{
				SpatialPosition index;
				std::map<SpatialPosition, std::vector<>> registFace;
			};
			std::map<std::shared_ptr<>, TerrainModel> terrains;

		public:
			TerrainManager() = default;
			~TerrainManager() {};

		private:
			void RegisterPolygon();

		public:
			std::vector<> GetSpatialFaces(const Math::Vector3S32 index);

		};
	}//namespace FrameWork
}//namespace Bread