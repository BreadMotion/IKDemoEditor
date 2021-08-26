#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "OS/Resource.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// アニメーションデータ
		//****************************************************************************
		struct AnimationData
		{
			struct ScaleKey
			{
				f32                  seconds;
				Math::Vector3 value;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct RotateKey
			{
				f32                        seconds;
				Math::Quaternion value;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct TranslateKey
			{
				f32                  seconds;
				Math::Vector3 value;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Node
			{
				//std::string name;
				std::vector<ScaleKey>      scaleKeys;
				std::vector<RotateKey>    rotateKeys;
				std::vector<TranslateKey> translateKeys;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Clip
			{
				std::string               name;
				f32                          secondsLength;
				u32                         frameLength;
				std::vector<Node> nodes;

				void GetScale(s32 nodeID, f32 seconds, Math::Vector3& value, s32& keyID) const;
				void GetRotate(s32 nodeID, f32 seconds, Math::Quaternion& value, s32& keyID) const;
				void GetTranslate(s32 nodeID, f32 seconds, Math::Vector3& value, s32& keyID) const;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			std::vector<Clip>          clips;
			std::vector<std::string> nodeNames;

			template<class Archive>
			void serialize(Archive& archive, u32 version);

			// シリアライズ
			static void Serialize(const AnimationData& data, const char* filename);

			// デシリアライズ
			static bool Deserialize(AnimationData& data, const char* filename);
		};

		//****************************************************************************
		// アニメーションリソース操作インターフェース
		//****************************************************************************
		class IAnimationResource : public OS::Resource
		{
		public:
			// アニメーションデータ取得
			virtual const AnimationData& GetAnimationData() = 0;
		};

		//****************************************************************************
		// アニメーションリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IAnimationResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<IAnimationResourceFactory> Create();
		};

		//****************************************************************************
		// アニメーションプレイヤー操作インターフェース
		//****************************************************************************
		class IAnimationPlayer : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IAnimationPlayer> Create();

			// 初期化
			virtual bool Initialize(std::shared_ptr<IAnimationResource>& resource) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 更新
			virtual void Update(f32 elapsedTime) = 0;

			// スケール値計算
			virtual void CalculateScale(s32 nodeID, Math::Vector3& scale) = 0;

			// 回転値計算
			virtual void CalculateRotate(s32 nodeID, Math::Quaternion& rotate) = 0;

			// 移動値計算
			virtual void CalculateTranslate(s32 nodeID, Math::Vector3& translate) = 0;

			// 再生中か
			virtual bool IsPlaying() = 0;

			// 再生
			virtual void Play(s32 index) = 0;
			virtual void Play(const char* name) = 0;

			// 巻き戻して再生
			virtual void Rewind() = 0;

			// 一時停止/再開
			virtual void Pause(bool pause) = 0;

			// 停止
			virtual void Stop() = 0;

			// ループ再生設定
			virtual void SetLoop(bool loop) = 0;

			// ループ再生か
			virtual bool IsLoop() = 0;

			// 逆再生設定
			virtual void SetReverse(bool reverse) = 0;

			// 逆再生か
			virtual bool IsReverse() = 0;

			// 再生時間長(秒)取得
			virtual f32 GetLength() = 0;

			// 再生速度倍率設定
			virtual void SetSpeed(f32 speed) = 0;

			// 再生速度倍率取得
			virtual f32 GetSpeed() = 0;

			// ブレンド時間設定
			virtual void SetBlendTime(f32 seconds) = 0;

			// ブレンド時間取得
			virtual f32 GetBlendTime() = 0;

			// ブレンド中か
			virtual bool IsBlend() = 0;

			// 再生位置(秒)設定
			virtual void SetCurrentTime(f32 seconds) = 0;

			// 再生位置取得
			virtual f32 GetAnimCurrentTime() = 0;

			// 残り再生時間取得
			virtual f32 GetLastTime() = 0;

			// 再生開始位置設定
			virtual void SetBeginTime(f32 seconds) = 0;

			// 再生開始位置取得
			virtual f32 GetBeginTime() = 0;

			// 再生終了位置設定
			virtual void SetEndTime(f32 seconds) = 0;

			// 再生終了位置取得
			virtual f32 GetEndTime() = 0;

			// ループ再生開始位置設定
			virtual void SetLoopBeginTime(f32 seconds) = 0;

			// ループ再生開始位置取得
			virtual f32 GetLoopBeginTime() = 0;

			// ループ再生終了位置設定
			virtual void SetLoopEndTime(f32 seconds) = 0;

			// ループ再生終了位置取得
			virtual f32 GetLoopEndTime() = 0;
		};
	} // namespace Graphics
} // namespace Bread