#pragma once


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// ビューポート
		//****************************************************************************
		struct Viewport
		{
			float x;
			float y;
			float width;
			float height;
			float minDepth;
			float maxDepth;
		};
	} // namespace Graphics
} // namespace Bread