#pragma once

#include <map>
#include <memory>
#include <optional>
#include "Math/BREADMath.h"


template <class T>
class State
{
private:
	T id;

protected:
	bool canChangeState = true; // 基本は、いつでもステート移行できるようにしておく

public:
	State() = delete;
	explicit State(T id) : id(id) {}
	virtual ~State() = default;

public:
	// 状態に入ったときに呼ばれる関数
	virtual void SetUp() = 0;

	// 次の状態に移る前に呼ばれる関数
	virtual void CleanUp() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="elapsedTime"> 経過時間 </param>
	/// <returns> 次の移行するステートID </returns>
	virtual T Update(Bread::f32 elapsedTime) = 0;

public:
	// ステートID取得
	const T& GetID() { return id; }

	// ステートの移行ができるか？
	bool CanChangeState() { return canChangeState; }
};

template <class T>
class StateMachine
{
protected:
	std::map<T, std::shared_ptr<State<T>>> stateList;
	std::shared_ptr<State<T>> currentState;

public:
	StateMachine() = default;
	virtual ~StateMachine() = default;

public:
	// ステートマシンのセットアップ
	virtual void SetUp() = 0;

	// ステートマシンのクリーンアップ
	virtual void CleanUp()
	{
		currentState.reset();
		stateList.clear();
	}

	// 更新
	virtual T Update(Bread::f32 elapsedTime) = 0;

public:
	/// <summary>
	/// ステート追加
	/// </summary>
	/// <param name="state"> 追加するステートのstd::make_shared </param>
	void AddState(const std::shared_ptr<State<T>>& state)
	{
		if (state == nullptr) return;

		auto it = stateList.find(state->GetID());
		if (it != stateList.end())
		{
			return;
		}

		stateList[state->GetID()] = state;
	}

	/// <summary>
	/// ステート追加
	/// </summary>
	/// <param name="state"> 追加するステートのstd::make_shared </param>
	void RemoveState(const std::shared_ptr<State<T>>& state)
	{
		if (state == nullptr) return;

		auto it = stateList.find(state->GetID());
		if (it != stateList.end())
		{
			stateList.erase(it);
		}
	}

	/// <summary>
	/// ステート取得
	/// </summary>
	/// <param name="type"> ステートID </param>
	/// <returns> ステート </returns>
	std::shared_ptr<State<T>> GetState(T stateID)
	{
		auto it = stateList.find(stateID);
		if (it != stateList.end())
		{
			return nullptr;
		}

		return stateList[stateID];
	}

	/// <summary>
	/// 現在のステートID取得
	/// </summary>
	/// <returns> 現在のステートID </returns>
	T GetCurrentStateName() const
	{
		if (currentState == nullptr)
		{
			return static_cast<T>(-1);
		}

		return currentState->GetID();
	}

	/// <summary>
	/// 指定のステートへ移行
	/// </summary>
	/// <param name="nextStateID"> 移行先のステートID </param>
	/// <param name="forcedChange"> 強制的にステート移行する </param>
	/// <returns> ステート移行成功 : 0, ステート移行失敗 : -1 </returns>
	Bread::s32 GoToState(T nextStateID, bool forcedChange = false)
	{
		auto it = stateList.find(nextStateID);
		if (it == stateList.end())
		{
			return -1;
		}

		if (currentState != nullptr)
		{
			if (!currentState->CanChangeState() && !forcedChange)
			{
				return -1;
			}
			else
			{
				currentState->CleanUp();
			}
		}

		currentState = stateList[nextStateID];
		currentState->SetUp();

		return 0;
	}
};