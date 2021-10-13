#pragma once
#include "FND/Base.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		//��Ԃ̃u���b�N���Ǘ�����N���X
		//���g�͓n���ꂽ���W��񂩂�u���b�N�̕��A���s�A�����Ȃǂ̏�񂩂牽�u���b�N�ڂɂ���̂��v�Z���邾���̃N���X
		class SpatialDivisionManager
		{
		private:
			const f32 Height{ 200.0f };
			const f32 width { 200.0f };

		public:
			SpatialDivisionManager() = default;
			~SpatialDivisionManager() {}

		public:
			Math::Vector3S32 SpatialCurrent(const Math::Vector3& position);
			Math::Vector3S32 SpatialCurrent(const f32 x, const f32 y, const f32 z);
		};
	}//namespace FrameWork
}//namespace Bread