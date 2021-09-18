#pragma once

#include "Graphics/Animation.h"
#include "Math/BreadMath.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// アニメーションプレイヤークラス
		//****************************************************************************
		class AnimationPlayer : public IAnimationPlayer
		{
		private:
			struct AccessKey
			{
				s32 scaleID     = 0;
				s32 rotateID    = 0;
				s32 translateID = 0;
			};

		private:
			std::shared_ptr<IAnimationResource>	resource;
			std::vector<AccessKey> accessKeys;
			const AnimationData::Clip* currentClip = nullptr;
			f32 blendLength     = 0.0f;
			f32 blendTime        = 0.0f;
			f32 blendRate         = 0.0f;
			f32 speed               = 1.0f;
			f32 currentTime      = 0.0f;
			f32 beginTime        = 0.0f;
			f32 endTime           = 0.0f;
			f32 loopBeginTime = 0.0f;
			f32 loopEndTime    = 0.0f;
			bool playing           = false;
			bool loop                = false;
			bool reverse           = false;

		public:
			AnimationPlayer() {}
			~AnimationPlayer() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(std::shared_ptr<IAnimationResource>& resource) override;

			// 終了化
			void Finalize() override;

			// 更新
			void Update(f32 elapsed_time) override;

			// スケール値計算
			void CalculateScale(s32 nodeID, Math::Vector3& scale) override;

			// 回転値計算
			void CalculateRotate(s32 nodeID, Math::Quaternion& rotate) override;

			// 移動値計算
			void CalculateTranslate(s32 nodeID, Math::Vector3& translate) override;

			// 再生中か
			bool IsPlaying() override;

			// 再生
			void Play(s32 index) override;
			void Play(const char* name) override;

			// 巻き戻して再生
			void Rewind() override;

			// 一時停止/再開
			void Pause(bool pause) override;

			// 停止
			void Stop() override;

			// ループ再生設定
			void SetLoop(bool loop) override;

			// ループ再生か
			bool IsLoop() override;

			// 逆再生設定
			void SetReverse(bool reverse) override;

			// 逆再生か
			bool IsReverse() override;

			// 再生時間長(秒)取得
			f32 GetLength() override;

			// 再生速度倍率設定
			void SetSpeed(f32 speed) override;

			// 再生速度倍率取得
			f32 GetSpeed() override;

			// ブレンド時間設定
			void SetBlendTime(f32 seconds) override;

			// ブレンド時間取得
			f32 GetBlendTime() override;

			// ブレンド中か
			bool IsBlend() override;

			// 再生位置設定
			void SetCurrentTime(f32 seconds) override;

			// 再生位置取得
			f32 GetAnimCurrentTime() override;

			// 残り再生時間取得
			f32 GetLastTime() override;

			// 再生開始位置設定
			void SetBeginTime(f32 seconds) override;

			// 再生開始位置取得
			f32 GetBeginTime() override;

			// 再生終了位置設定
			void SetEndTime(f32 seconds) override;

			// 再生終了位置取得
			f32 GetEndTime() override;

			// ループ再生開始位置設定
			void SetLoopBeginTime(f32 seconds) override;

			// ループ再生開始位置取得
			f32 GetLoopBeginTime() override;

			// ループ再生終了位置設定
			void SetLoopEndTime(f32 seconds) override;

			// ループ再生終了位置取得
			f32 GetLoopEndTime() override;
		};
	} // namespace Graphics
} // namespace Bread