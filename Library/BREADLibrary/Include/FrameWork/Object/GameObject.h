#pragma once

#include <memory>
#include <string>
#include "Math/BreadMath.h"
#include "FrameWork/Object.h"
#include "FrameWork/Component/Component.h"
#include "FrameWork/FrameTime.h"


namespace Bread
{
	namespace FrameWork
	{
		class GameObject : public std::enable_shared_from_this<GameObject>,
			public IObject
		{
		private:
			using super = FND::Base;

			std::weak_ptr<GameObject> parent;
			std::vector<std::shared_ptr<GameObject>> children;
			std::vector<std::shared_ptr<Component>> components;

			std::vector<IObject*> objects;	// ワーク用

			std::string name;

			Math::Vector3 translate = Math::Vector3::Zero;
			Math::Quaternion rotate = Math::Quaternion::Zero;
			Math::Vector3 scale = Math::Vector3::OneAll;
			Math::Vector3 pivot = Math::Vector3::Zero;
			Math::Matrix localTransform = Math::Matrix::One;
			Math::Matrix worldTransform = Math::Matrix::One;
			Math::AABB localBounds = Math::AABB::Zero;
			Math::AABB worldBounds = Math::AABB::Zero;

			u32 layer = 0;

			bool active = true;
			bool boundsDirty = false;
			bool boundsUpdated = false;
			bool localTransformDirty = false;
			bool worldTransformDirty = false;
			bool worldTransformUpdated = false;

		public:
			GameObject();
			virtual ~GameObject();

		private:
			GameObject(const GameObject&);
			const GameObject& operator = (const GameObject&);

		public:
			static std::shared_ptr<GameObject> Create();

			// 名前を設定
			void SetName(const char* name) override { this->name = name; }

			// 名前を取得
			const char* GetName() override { return this->name.c_str(); }

			// アクティブ状態を設定
			void SetActive(bool active) override { this->active = active; }

			// アクティブ状態を取得
			bool IsActive() override { return this->active; }

			// 親ゲームオブジェクトを取得
			IObject* GetParent() override { return this->parent.lock().get(); }

			// 子ゲームオブジェクト数取得
			const std::vector<IObject*>& GetChildren() override;

			// 子を追加できるか判定
			bool CanAddChild(IObject* child) override;

			// 子を追加
			bool AddChild(IObject* child) override;

			// バウンディングボックスを取得
			const Math::AABB& GetBounds() override { return GetWorldBounds(); }

			// スケール値を設定
			void SetScale(const Math::Vector3& scale) override { this->scale = scale; InvalidateLocalTransform(); }

			// 回転値を設定
			void SetRotate(const Math::Quaternion& rotate) override { this->rotate = rotate; InvalidateLocalTransform(); }

			// 移動値を設定
			void SetTranslate(const Math::Vector3& translate) override { this->translate = translate; InvalidateLocalTransform(); }

			// スケール値を取得
			const Math::Vector3& GetScale() override { return scale; }

			// 回転値を取得
			const Math::Quaternion& GetRotate() override { return rotate; }

			// 移動値を取得
			const Math::Vector3& GetTranslate() override { return translate; }

			// ローカルトランスフォームを取得
			const Math::Matrix& GetLocalTransform() override { return localTransform; }

			// ワールドトランスフォームを取得
			const Math::Matrix& GetWorldTransform() override { return worldTransform; }

			// トランスフォームフラグを取得
			TransformationFlag GetTransformationFlag() override { return TransformationFlag::All; }

			// 描画ノードを収集
			void GetRenderables(IGame* game, RenderableCollection& collection);

			// 行列更新処理
			void UpdateTransform(IGame* game);

			// フレーム毎の更新処理
			void Update(IGame* game, UpdateType update_type);

			// レイヤーを取得
			u32 GetLayer() const { return layer; }

			// ローカルバウンディングボックスを取得
			const Math::AABB& GetLocalBounds() const { return localBounds; }

			// ワールドバウンディングボックスを取得
			const Math::AABB& GetWorldBounds() const { return worldBounds; }

			// ローカルトランスフォームを設定
			void SetLocalTransform(const Math::Matrix& transform);

			// ローカルバウンディングボックスを拡張
			void ExtendLocalBounds(const Math::AABB& bounds);

			// ワールドバウンディングボックスを計算
			void ComputeWorldBounds(Math::AABB& bounds);

			// 親ゲームオブジェクトを設定
			void SetParent(std::shared_ptr<GameObject> parent);

			// 子ゲームオブジェクトを追加
			void AddChild(std::shared_ptr<GameObject> child, s32 index = -1);

			// 子ゲームオブジェクトを削除
			void RemoveChild(std::shared_ptr<GameObject> child);

			// 親からゲームオブジェクトを削除
			void RemoveFromParent();

			// 子ゲームオブジェクトをクリア
			void ClearChildren();

			// コンポーネントを追加
			void AddComponent(std::shared_ptr<Component> component, IGame* game, s32 index = -1);

			// コンポーネントを削除
			void RemoveComponent(std::shared_ptr<Component> component, IGame* game);

			// コンポーネントをクリア
			void ClearComponents();

			// ローカルトランスフォームを更新
			void UpdateLocalTransform();

			// ワールドトランスフォームを更新
			void UpdateWorldTransform();

			// ワールドバウンディングボックス
			void UpdateWorldBounds();

			// ローカルトランスフォームの更新を要求
			void InvalidateLocalTransform();

			// ワールドトランスフォームの更新を要求
			void InvalidateWorldTransform();

			// ワールドバウンディングボックスの更新を要求
			void InvalidateWorldBounds();

			// セットアップ
			virtual void Setup(IGame* game);

			// インスタンス生成後の解決処理
			virtual void Resolve();

			// 破棄
			virtual void Destroy(IGame* game);

			// 指定の型のコンポーネントを取得
			template<class T>
			std::shared_ptr<T> GetComponent()
			{
				for (auto&& component : components)
				{
					auto obj = std::dynamic_pointer_cast<T>(component);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			// 指定の型のコンポーネントを全て取得
			template<class T>
			void GetComponents(std::vector<std::shared_ptr<T>>& components)
			{
				for (auto&& component : components)
				{
					auto obj = std::dynamic_pointer_cast<T>(component);
					if (obj != nullptr)
					{
						components.emplace_back(obj);
					}
				}
			}

			// 子ゲームオブジェクトから指定の型のコンポーネントを取得
			template<class T>
			std::shared_ptr<T> GetComponentInChildren()
			{
				for (auto&& child : children)
				{
					auto component = child->GetComponent<T>();
					if (component != nullptr)
					{
						return component;
					}
				}

				for (auto&& child : children)
				{
					auto component = child->GetComponentInChildren<T>();
					if (component != nullptr)
					{
						return component;
					}
				}

				return nullptr;
			}


			// 指定の型のコンポーネントを追加
			template<class T, class... Args>
			std::shared_ptr<T> AddComponent(IGame* game, Args... args)
			{
				std::shared_ptr<T> component = std::make_shared<T>(args...);
				AddComponent(component, game, -1);
				return component;
			}
		};
	}
}