#include "pch.h"
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include "FND/Logger.h"
#include "OS/Path.h"
#include "Math/Arithmetic.h"
#include "AnimationResource.h"


CEREAL_CLASS_VERSION(Bread::Graphics::AnimationData, 1)
CEREAL_CLASS_VERSION(Bread::Graphics::AnimationData::ScaleKey, 1)
CEREAL_CLASS_VERSION(Bread::Graphics::AnimationData::RotateKey, 1)
CEREAL_CLASS_VERSION(Bread::Graphics::AnimationData::TranslateKey, 1)
CEREAL_CLASS_VERSION(Bread::Graphics::AnimationData::Node, 1)
CEREAL_CLASS_VERSION(Bread::Graphics::AnimationData::Clip, 1)


namespace Bread
{
	namespace Graphics
	{
		//============================================================================
		// アニメーションデータ
		//----------------------------------------------------------------------------
		template<class Archive>
		void AnimationData::ScaleKey::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(seconds),
				CEREAL_NVP(value)
			);
		}

		template<class Archive>
		void AnimationData::RotateKey::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(seconds),
				CEREAL_NVP(value)
			);
		}

		template<class Archive>
		void AnimationData::TranslateKey::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(seconds),
				CEREAL_NVP(value)
			);
		}

		template<class Archive>
		void AnimationData::Node::serialize(Archive& archive, u32 version)
		{
			archive(
				//CEREAL_NVP(name),
				CEREAL_NVP(scaleKeys),
				CEREAL_NVP(rotateKeys),
				CEREAL_NVP(translateKeys)
			);
		}

		template<class Archive>
		void AnimationData::Clip::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(name),
				CEREAL_NVP(secondsLength),
				CEREAL_NVP(frameLength),
				CEREAL_NVP(nodes)
			);
		}

		template<class Archive>
		void AnimationData::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(clips),
				CEREAL_NVP(nodeNames)
			);
		}

		// シリアライズ
		void AnimationData::Serialize(const AnimationData& animation, const char* filename)
		{
			// バイナリ
			{
				std::ofstream stream(filename, std::ios::binary);
				if (stream.is_open())
				{
					cereal::BinaryOutputArchive archive(stream);

					archive(CEREAL_NVP(animation));
				}
			}
			// JSON
			{
				std::ofstream stream(OS::Path::ChangeFileExtension(filename, "json"));
				if (stream.is_open())
				{
					cereal::JSONOutputArchive archive(stream);

					archive(CEREAL_NVP(animation));
				}
			}
		}

		// デシリアライズ
		bool AnimationData::Deserialize(AnimationData& animation, const char* filename)
		{
			std::ifstream stream(filename, std::ios::binary);
			if (stream.is_open())
			{
				cereal::BinaryInputArchive archive(stream);

				archive(CEREAL_NVP(animation));
			}
			else
			{
				// バイナリファイルがなかった場合はJSONファイルを開く
				stream.open(OS::Path::ChangeFileExtension(filename, "json"));
				if (stream.is_open())
				{
					cereal::JSONInputArchive archive(stream);

					archive(CEREAL_NVP(animation));
				}
				else
				{
					BREAD_LOG_GRP_ERROR("not found : %s\n", filename);
					return false;
				}
			}

			return true;
		}

		//============================================================================
		// アニメーションクリップ
		//----------------------------------------------------------------------------
		// キーフレーム情報取得
		template<typename T>
		void GetKeyInfo(f32 seconds, s32& keyID, f32& lerpRate, const std::vector<T>& keys)
		{
			if (keys.at(keyID).seconds <= seconds)
			{
				s32 key_count = static_cast<s32>(keys.size());
				for (; keyID < key_count; ++keyID)
				{
					if (Math::Equal(keys.at(keyID).seconds, seconds))
					{
						lerpRate = 0.0f;
						return;
					}
					if (keys.at(keyID).seconds > seconds)
					{
						--keyID;
						lerpRate = (seconds - keys.at(keyID).seconds) / (keys.at(keyID + 1).seconds - keys.at(keyID).seconds);
						return;
					}
				}
				--keyID;
			}
			else
			{
				for (; ; --keyID)
				{
					if (Math::Equal(keys.at(keyID).seconds, seconds))
					{
						lerpRate = 0.0f;
						return;
					}
					if (keys.at(keyID).seconds < seconds)
					{
						lerpRate = (seconds - keys.at(keyID).seconds) / (keys.at(keyID + 1).seconds - keys.at(keyID).seconds);
						return;
					}
					if (keyID == 0)
					{
						break;
					}
				}
			}
			lerpRate = 0.0f;
		}

		// スケール値取得
		void AnimationData::Clip::GetScale(s32 nodeID, f32 seconds, Math::Vector3& value, s32& keyID) const
		{
			const Node& node = nodes.at(nodeID);

			if (node.scaleKeys.size() <= keyID) return;

			f32 lerpRate;
			GetKeyInfo(seconds, keyID, lerpRate, node.scaleKeys);

			if (Math::Equal(lerpRate, 0.0f))
			{
				value = node.scaleKeys.at(keyID).value;
			}
			else
			{
				value = Math::Vector3Lerp(node.scaleKeys.at(keyID).value, node.scaleKeys.at(keyID + 1).value, lerpRate);
			}
		}

		// 回転値取得
		void AnimationData::Clip::GetRotate(s32 nodeID, f32 seconds, Math::Quaternion& value, s32& keyID) const
		{
			const Node& node = nodes.at(nodeID);

			if (node.rotateKeys.size() <= keyID) return;

			f32 lerpRate;
			GetKeyInfo(seconds, keyID, lerpRate, node.rotateKeys);

			if (Math::Equal(lerpRate, 0.0f))
			{
				value = node.rotateKeys.at(keyID).value;
			}
			else
			{
				value = Math::QuaternionSlerp(node.rotateKeys.at(keyID).value, node.rotateKeys.at(keyID + 1).value, lerpRate);
			}
		}

		// 移動値取得
		void AnimationData::Clip::GetTranslate(s32 nodeID, f32 seconds, Math::Vector3& value, s32& keyID) const
		{
			const Node& node = nodes.at(nodeID);

			if (node.translateKeys.size() <= keyID) return;

			f32 lerpRate;
			GetKeyInfo(seconds, keyID, lerpRate, node.translateKeys);

			if (Math::Equal(lerpRate, 0.0f))
			{
				value = node.translateKeys.at(keyID).value;
			}
			else
			{
				value = Math::Vector3Lerp(node.translateKeys.at(keyID).value, node.translateKeys.at(keyID + 1).value, lerpRate);
			}
		}


		//============================================================================
		// アニメーションリソースクラス
		//----------------------------------------------------------------------------
		// 初期化
		bool AnimationResource::Initialize(const char* filename)
		{
			// デシリアライズ
			if (!AnimationData::Deserialize(data, filename))
			{
				return false;
			}

			return true;
		}

		//============================================================================
		// アニメーションリソースファクトリクラス
		//----------------------------------------------------------------------------
		// 生成
		std::unique_ptr<IAnimationResourceFactory> IAnimationResourceFactory::Create()
		{
			return std::make_unique<AnimationResourceFactory>();
		}

		// リソース作成
		std::unique_ptr<OS::Resource> AnimationResourceFactory::CreateResource(u32 type)
		{
			return std::make_unique<AnimationResource>();
		}

		// リソース読み込み
		bool AnimationResourceFactory::LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)
		{
			AnimationResource* animationResource = static_cast<AnimationResource*>(resource);
			return animationResource->Initialize(filename);
		}

	}	// namespace grp
}	// namespace ys