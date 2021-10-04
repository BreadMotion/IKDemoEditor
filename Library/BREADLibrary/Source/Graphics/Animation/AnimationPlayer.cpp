#include "pch.h"
#include "FND/Assert.h"
#include "FND/Logger.h"
#include "Math/Arithmetic.h"
#include "AnimationPlayer.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// アニメーションプレイヤー
		//****************************************************************************
		// 生成
		std::unique_ptr<IAnimationPlayer> IAnimationPlayer::Create()
		{
			return std::make_unique<AnimationPlayer>();
		}

		// 初期化
		bool AnimationPlayer::Initialize(std::shared_ptr<IAnimationResource>& resource)
		{
			this->resource = resource;
			accessKeys.resize(resource->GetAnimationData().nodeNames.size());

			return true;
		}

		// 終了化
		void AnimationPlayer::Finalize()
		{

		}

		// 更新
		void AnimationPlayer::Update(f32 elapsedTime)
		{
			if (IsPlaying())
			{
				blendRate = 1.0f;

				if (blendTime < blendLength)						// ※補完率の計算
				{													// 本来は前のポーズから次のポーズの補完したポーズに
					blendTime += elapsedTime * speed;				// 現在のポーズを合成することで綺麗なポーズにしたいが、
					if (blendTime >= blendLength)					// ポーズの計算量が前と次と現在で3倍かかり、また、
					{												// 合成するためのポーズを保存する一時バッファが必要なため、
						blendTime = blendLength;					// 現在のポーズから次のポーズの補完で、補完率を尻上がりに
					}												// 大きくすることで擬似的に再現している。
					blendRate = blendTime / blendLength;
					blendRate = blendRate * blendRate;
				}


				f32 fEndTime;
				if (reverse)
				{
					// 逆再生
					currentTime -= elapsedTime * speed;
					fEndTime = loop ? loopBeginTime : beginTime;
					if (currentTime <= fEndTime)
					{
						if (loop)
						{
							currentTime = (currentTime - fEndTime) + loopEndTime;
						}
						else
						{
							currentTime = beginTime;
							playing = false;
						}
					}
				}
				else
				{
					// 通常再生
					currentTime += elapsedTime * speed;
					fEndTime = loop ? loopEndTime : endTime;
					if (currentTime >= fEndTime)
					{
						if (loop)
						{
							currentTime = (currentTime - fEndTime) + loopBeginTime;
						}
						else
						{
							currentTime = fEndTime;
							playing = false;
						}
					}
				}
			}
		}

		// スケール値計算
		void AnimationPlayer::CalculateScale(s32 nodeID, Math::Vector3& scale)
		{
			BREAD_ASSERT(currentClip != nullptr);

			if (Math::Equal(blendRate, 1.0f))
			{
				// 通常の計算
				currentClip->GetScale(nodeID, currentTime, scale, accessKeys.at(nodeID).scaleID);
			}
			else if (Math::IsZero(blendRate))
			{
				// 何もしない
			}
			else
			{
				// ブレンド計算
				Math::Vector3 value;
				currentClip->GetScale(nodeID, currentTime, value, accessKeys.at(nodeID).scaleID);

				scale = Math::Vector3Lerp(scale, value, blendRate);
			}
		}

		// 回転値計算
		void AnimationPlayer::CalculateRotate(s32 nodeID, Math::Quaternion& rotate)
		{
			BREAD_ASSERT(currentClip != nullptr);

			if (Math::Equal(blendRate, 1.0f))
			{
				// 通常の計算
				currentClip->GetRotate(nodeID, currentTime, rotate, accessKeys.at(nodeID).rotateID);
			}
			else if (Math::IsZero(blendRate))
			{
				// 何もしない
			}
			else
			{
				// ブレンド計算
				Math::Quaternion value;
				currentClip->GetRotate(nodeID, currentTime, value, accessKeys.at(nodeID).rotateID);

				Math::QuaternionSlerp(rotate, value, blendRate);
			}
		}

		// 移動値計算
		void AnimationPlayer::CalculateTranslate(s32 nodeID, Math::Vector3& translate)
		{
			BREAD_ASSERT(currentClip != nullptr);

			if (Math::Equal(blendRate, 1.0f))
			{
				// 通常の計算
				currentClip->GetTranslate(nodeID, currentTime, translate, accessKeys.at(nodeID).translateID);
			}
			else if (Math::IsZero(blendRate))
			{
				// 何もしない
			}
			else
			{
				// ブレンド計算
				Math::Vector3 value;
				currentClip->GetTranslate(nodeID, currentTime, value, accessKeys.at(nodeID).translateID);

				translate = Math::Vector3Lerp(translate, value, blendRate);
			}
		}

		// 再生中か
		bool AnimationPlayer::IsPlaying()
		{
			return playing && currentClip != nullptr;
		}

		// 再生
		void AnimationPlayer::Play(s32 index)
		{
			currentClip = nullptr;
			playing     = false;
			currentTime = 0.0f;

			const Graphics::AnimationData& data = resource->GetAnimationData();

			if (index < 0 || index >= static_cast<s32>(data.clips.size()))
			{
				return;
			}

			const Graphics::AnimationData::Clip& clip = data.clips.at(index);

			currentClip = &clip;
			playing     = true;
			beginTime   = loopBeginTime = 0.0f;
			endTime     = loopEndTime = clip.secondsLength;
			currentTime = reverse ? endTime : beginTime;

		}

		// 再生
		void AnimationPlayer::Play(const char* name)
		{
			const Graphics::AnimationData& data = resource->GetAnimationData();
			for (size_t i = 0; i < data.clips.size(); ++i)
			{
				if (data.clips.at(i).name == name)
				{
					Play(static_cast<s32>(i));
				}
			}
		}

		// 巻き戻して再生
		void AnimationPlayer::Rewind()
		{
			if (currentClip != nullptr)
			{
				playing = true;
				currentTime = reverse ? endTime : beginTime;
			}
		}

		// 一時停止/再開
		void AnimationPlayer::Pause(bool pause)
		{
			playing = !pause;
		}

		// 停止
		void AnimationPlayer::Stop()
		{
			playing = false;
		}

		// ループ再生設定
		void AnimationPlayer::SetLoop(bool loop)
		{
			this->loop = loop;
		}

		// ループ再生か
		bool AnimationPlayer::IsLoop()
		{
			return loop;
		}

		// 逆再生設定
		void AnimationPlayer::SetReverse(bool reverse)
		{
			this->reverse = reverse;
		}

		// 逆再生か
		bool AnimationPlayer::IsReverse()
		{
			return reverse;
		}

		// 再生時間長(秒)取得
		f32 AnimationPlayer::GetLength()
		{
			if (currentClip != nullptr)
			{
				return currentClip->secondsLength;
			}
			return 0.0f;
		}

		// 再生速度倍率設定
		void AnimationPlayer::SetSpeed(f32 speed)
		{
			this->speed = speed;
		}

		// 再生速度倍率取得
		f32 AnimationPlayer::GetSpeed()
		{
			return speed;
		}

		// ブレンド時間設定
		void AnimationPlayer::SetBlendTime(f32 seconds)
		{
			blendTime = 0.0f;
			blendLength = seconds;
		}

		// ブレンド時間取得
		f32 AnimationPlayer::GetBlendTime()
		{
			return blendLength;
		}

		// ブレンド中か
		bool AnimationPlayer::IsBlend()
		{
			return blendRate != 1.0f ? true : false;
		}

		// 再生位置設定
		void AnimationPlayer::SetCurrentTime(f32 seconds)
		{
			currentTime = seconds;
		}

		// 再生位置取得
		f32 AnimationPlayer::GetAnimCurrentTime()
		{
			return currentTime;
		}

		// 残り再生時間取得
		f32 AnimationPlayer::GetLastTime()
		{
			if (reverse)
			{
				return currentTime - beginTime;
			}
			else
			{
				return endTime - currentTime;
			}
		}

		// 再生開始位置設定
		void AnimationPlayer::SetBeginTime(f32 seconds)
		{
			beginTime = seconds;
		}

		// 再生開始位置取得
		f32 AnimationPlayer::GetBeginTime()
		{
			return beginTime;
		}

		// 再生終了位置設定
		void AnimationPlayer::SetEndTime(f32 seconds)
		{
			endTime = seconds;
		}

		// 再生終了位置取得
		f32 AnimationPlayer::GetEndTime()
		{
			return endTime;
		}

		// ループ再生開始位置設定
		void AnimationPlayer::SetLoopBeginTime(f32 seconds)
		{
			loopBeginTime = seconds;
		}

		// ループ再生開始位置取得
		f32 AnimationPlayer::GetLoopBeginTime()
		{
			return loopBeginTime;
		}

		// ループ再生終了位置設定
		void AnimationPlayer::SetLoopEndTime(f32 seconds)
		{
			loopEndTime = seconds;
		}

		// ループ再生終了位置取得
		f32 AnimationPlayer::GetLoopEndTime()
		{
			return loopEndTime;
		}

	}	// namespace grp
}	// namespace ys