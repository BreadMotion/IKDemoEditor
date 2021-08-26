#include "pch.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "FND/STD.h"
#include "Math/BreadMath.h"
#include "mesh.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// メッシュ設定記述構造体
		//****************************************************************************
		// 頂点接線を計算
		void MeshDesc::ComputeTangents()
		{

			if (indexCount == 0 || vertexCount == 0
				|| (indicesU16 == nullptr && indicesU32 == nullptr)
				|| positions == nullptr
				|| normals == nullptr
				|| tangents == nullptr
				|| texcoords0 == nullptr)
			{
				return;
			}

			Math::Vector3* tan1 = new Math::Vector3[vertexCount];
			FND::MemSet(tan1, 0, sizeof(Math::Vector3) * vertexCount);

			for (u32 a = 0; a + 2 < indexCount; a += 3)
			{
				u32 i1, i2, i3;
				if (indicesU16 != nullptr)
				{
					i1 = indicesU16[a + 0];
					i2 = indicesU16[a + 1];
					i3 = indicesU16[a + 2];
				}
				else
				{
					i1 = indicesU32[a + 0];
					i2 = indicesU32[a + 1];
					i3 = indicesU32[a + 2];
				}

				const Math::Vector3& v1 = positions[i1];
				const Math::Vector3& v2 = positions[i2];
				const Math::Vector3& v3 = positions[i3];

				const Math::Vector2& w1 = texcoords0[i1];
				const Math::Vector2& w2 = texcoords0[i2];
				const Math::Vector2& w3 = texcoords0[i3];

				// position differences
				f32 x1 = v2.x - v1.x;
				f32 x2 = v3.x - v1.x;
				f32 y1 = v2.y - v1.y;
				f32 y2 = v3.y - v1.y;
				f32 z1 = v2.z - v1.z;
				f32 z2 = v3.z - v1.z;

				// texcoord differences
				f32 s1 = w2.x - w1.x;
				f32 s2 = w3.x - w1.x;
				f32 t1 = w2.y - w1.y;
				f32 t2 = w3.y - w1.y;

				// ratio
				f32 r = 1.0f / (s1 * t2 - s2 * t1);

				// vector in the s direction ( tangent )
				Math::Vector3 sdir = {
					(t2 * x1 - t1 * x2) * r,
					(t2 * y1 - t1 * y2) * r,
					(t2 * z1 - t1 * z2) * r
				};

				//accumulate tangents for each triangle
				tan1[i1] += sdir;
				tan1[i2] += sdir;
				tan1[i3] += sdir;
			}

			// loop through and ortho normalize the tangents
			auto dst = const_cast<Math::Vector3*>(tangents);
			for (u32 a = 0; a < vertexCount; ++a)
			{
				Math::Vector3 n = normals[a];
				Math::Vector3 t = tan1[a];
				{
					// OrthoNormalize
					n = Math::Vector3Normalize(n);
					Math::Vector3 tmp = n * Math::Vector3Dot(n, t);
					t = Math::Vector3Normalize(t - tmp);
				}
				dst[a] = t;
			}

			delete[] tan1;
		}

		//****************************************************************************
		// メッシュ操作オブジェクト基底
		//****************************************************************************
		// バウンディングボックスを計算
		void Mesh::ComputeBounds()
		{
			// update bounds
			if (desc.vertexCount > 0)
			{
				const Math::Vector3* p = desc.positions;

				bounds.Reset();

				for (u32 i = 0; i < desc.vertexCount; ++i, ++p)
				{
					bounds.min.x = FND::Min(bounds.min.x, p->x);
					bounds.min.y = FND::Min(bounds.min.y, p->y);
					bounds.min.z = FND::Min(bounds.min.z, p->z);

					bounds.max.x = FND::Max(bounds.max.x, p->x);
					bounds.max.y = FND::Max(bounds.max.y, p->y);
					bounds.max.z = FND::Max(bounds.max.z, p->z);
				}
			}
		}

		// バウンディングボックス計算
		void Mesh::ComputeBounds(Math::AABB& bounds, const Math::Matrix boneTransforms[])
		{
			/*bounds.Reset();

			if (boneTransforms != nullptr)
			{

				for (u32 i = 0; i < desc.vertexCount; ++i)
				{
					const Math::Vector4& position = desc.positions[i];
					const Math::Vector4& weight0 = desc.blendWeights0[i];
					const Math::Vector4& weight1 = desc.blendWeights1[i];
					const Math::Vector4_U8& indices0 = desc.blendIndices0[i];
					const Math::Vector4_U8& indices1 = desc.blendIndices1[i];

					Math::Vector3 vertex = Math::Vector3::Zero;
					for (u32 j = 0; j < 4; ++j)
					{
						Math::Vector3 out0, out1;
						out0 = Math::Vector4TransformCoord(position, boneTransforms[indices0.v[j]]);
						out1 = Math::Vector4TransformCoord(position, boneTransforms[indices1.v[j]]);

						vertex += out0 * weight0.v[j];
						vertex += out1 * weight1.v[j];
					}
					bounds.min.x = FND::Min(bounds.min.x, vertex.x);
					bounds.min.y = FND::Min(bounds.min.y, vertex.y);
					bounds.min.z = FND::Min(bounds.min.z, vertex.z);

					bounds.max.x = FND::Max(bounds.max.x, vertex.x);
					bounds.max.y = FND::Max(bounds.max.y, vertex.y);
					bounds.max.z = FND::Max(bounds.max.z, vertex.z);
				}
			}
			else
			{
				ComputeBounds();
				bounds = this->bounds;
			}*/
		}

		// スキニング頂点計算
		void Mesh::ComputeSkinnedVertices(Math::Vector3 vertices[], const Math::Matrix bone_transforms[])
		{
			/*for (u32 i = 0; i < desc.vertexCount; ++i)
			{
				const Math::Vector3& position = desc.positions[i];
				const Math::Vector4& weight0 = desc.blendWeights0[i];
				const Math::Vector4& weight1 = desc.blendWeights1[i];
				const Math::Vector4_U8& indices0 = desc.blendIndices0[i];
				const Math::Vector4_U8& indices1 = desc.blendIndices1[i];
				Math::Vector3& vertex = vertices[i];

				vertex = Math::Vector3::Zero;
				for (u32 j = 0; j < 4; ++j)
				{
					Math::Vector3 out0, out1;
					out0 = Math::Vector3TransformCoord(position, bone_transforms[indices0.v[j]]);
					out1 = Math::Vector3TransformCoord(position, bone_transforms[indices1.v[j]]);

					vertex += out0 * weight0.v[j];
					vertex += out1 * weight1.v[j];
				}
			}*/
		}
	} // namespace Graphics
} // namespace Bread