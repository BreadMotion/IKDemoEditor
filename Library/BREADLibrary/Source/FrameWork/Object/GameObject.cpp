#include "pch.h"
#include "FrameWork/Object/GameObject.h"


namespace Bread
{
	namespace FrameWork
	{
		GameObject::GameObject()
		{
		}

		GameObject::~GameObject()
		{
			ClearComponents();
			ClearChildren();
		}

		// 生成
		std::shared_ptr<GameObject> GameObject::Create()
		{
			return std::make_shared<GameObject>();
		}

		// 子ゲームオブジェクト数取得
		const std::vector<IObject*>& GameObject::GetChildren()
		{
			objects.clear();
			for (std::shared_ptr<GameObject>& child : children)
			{
				objects.push_back(child.get());
			}

			for (std::shared_ptr<Component>& component : components)
			{
				component->GetChildren(objects);
			}

			return objects;
		}

		// 子を追加できるか判定
		bool GameObject::CanAddChild(IObject* child)
		{
			GameObject* gameObject = dynamic_cast<GameObject*>(child);
			return gameObject != nullptr;
		}

		// 子を追加
		bool GameObject::AddChild(IObject* child)
		{
			GameObject* gameObject = dynamic_cast<GameObject*>(child);
			if (gameObject != nullptr)
			{
				AddChild(gameObject->shared_from_this(), -1);
				return true;
			}

			return false;
		}

		// 描画ノード収集
		void GameObject::GetRenderables(IGame* game, RenderableCollection& collection)
		{
			// TODO : Draw
			/*if (!IsActive())
			{
				return;
			}

			if (game->GetRenderState()->GetCullingMaskFlags().Test(1 << layer))
			{
				for (auto&& component : m_components)
				{
					if (component->IsEnable())
					{
						component->GetRenderables(game, collection, false);
					}
				}
			}

			for (auto&& child : m_children)
			{
				child->GetRenderables(game, collection);
			}*/
		}

		// 行列更新処理
		void GameObject::UpdateTransform(IGame* game)
		{
			if (!IsActive())
			{
				return;
			}

			worldTransformUpdated = false;
			boundsUpdated = false;

			Math::AABB localBounds;
			localBounds.Reset();
			this->localBounds = localBounds;

			// トランスフォーム更新処理
			UpdateLocalTransform();
			UpdateWorldTransform();

			// コンポーネント更新処理
			for (auto&& component : components)
			{
				if (component->IsEnable())
				{
					component->UpdateTransform(game);
				}
			}

			if (this->localBounds.min == localBounds.min &&
				this->localBounds.max == localBounds.max)
			{
				this->localBounds = Math::AABB::Zero;
			}

			// バウンディングボックス更新処理
			UpdateWorldBounds();

			// 子ゲームオブジェクト更新処理
			for (auto&& child : children)
			{
				child->UpdateTransform(game);
			}
		}

		// 更新
		void GameObject::Update(IGame* game, UpdateType update_type)
		{
			if (!IsActive())
			{
				return;
			}

			// コンポーネント更新処理
			for (auto&& component : components)
			{
				if (component->IsEnable())
				{
					if (!component->IsStarted())
					{
						component->SetStart();
						component->Start(game);
					}
					component->Update(game);
				}
			}

			// 子ゲームオブジェクト更新処理
			for (auto&& child : children)
			{
				child->Update(game, update_type);
			}
		}

		// ローカルトランスフォーム設定
		void GameObject::SetLocalTransform(const Math::Matrix& transform)
		{
			localTransform = transform;
			InvalidateWorldTransform();
			InvalidateWorldBounds();
		}

		// ローカルバウンディングボックス拡張
		void GameObject::ExtendLocalBounds(const Math::AABB& bounds)
		{
			localBounds.Extend(bounds);
			InvalidateWorldBounds();
		}

		// ワールドバウンディングボックスを計算
		void GameObject::ComputeWorldBounds(Math::AABB& bounds)
		{
			bounds.Extend(worldBounds);

			for (auto&& child : children)
			{
				child->ComputeWorldBounds(bounds);
			}
		}

		// 親ゲームオブジェクト設定
		void GameObject::SetParent(std::shared_ptr<GameObject> parent)
		{
			InvalidateWorldBounds();
			this->parent = parent;
			InvalidateWorldBounds();
			InvalidateWorldTransform();
		}

		// 子ゲームオブジェクト追加
		void GameObject::AddChild(std::shared_ptr<GameObject> child, s32 index)
		{
			child->RemoveFromParent();
			child->SetParent(shared_from_this());

			if (index < 0)
			{
				children.emplace_back(child);
			}
			else
			{
				children.insert(children.begin() + index, child);
			}
		}

		// 子ゲームオブジェクト削除
		void GameObject::RemoveChild(std::shared_ptr<GameObject> child)
		{
			child->SetParent(nullptr);

			auto it = std::find(children.begin(), children.end(), child);
			if (it != children.end())
			{
				children.erase(it);
			}
		}

		// 親からゲームオブジェクトを削除
		void GameObject::RemoveFromParent()
		{
			std::shared_ptr<GameObject> parent = this->parent.lock();
			if (parent != nullptr)
			{
				parent->RemoveChild(shared_from_this());
			}
		}

		// 子ゲームオブジェクトをクリア
		void GameObject::ClearChildren()
		{
			children.clear();
		}

		// コンポーネント追加
		void GameObject::AddComponent(std::shared_ptr<Component> component, IGame* game, s32 index)
		{
			component->SetOwner(shared_from_this());
			component->Awake(game);

			if (index < 0)
			{
				components.emplace_back(component);
			}
			else
			{
				components.insert(components.begin() + index, component);
			}
		}

		// コンポーネント削除
		void GameObject::RemoveComponent(std::shared_ptr<Component> component, IGame* game)
		{
			component->SetOwner(nullptr);
			component->OnDestroy(game);

			auto&& it = std::find(components.begin(), components.end(), component);
			if (it != components.end())
			{
				components.erase(it);
			}
		}

		// コンポーネントをクリア
		void GameObject::ClearComponents()
		{
			components.clear();
		}

		// ローカルトランスフォーム更新
		void GameObject::UpdateLocalTransform()
		{
			if (localTransformDirty)
			{
				localTransformDirty = false;
#if 1
				Math::Matrix pivotM, scaleM, rotateM, translateM;
				pivotM = Math::MatrixTranslation(-pivot.x, -pivot.y, -pivot.z);
				scaleM = Math::MatrixScaling(scale.x, scale.y, scale.z);
				rotateM = Math::MatrixRotationQuaternion(&rotate);
				translateM = Math::MatrixTranslation(pivot.x + translate.x, pivot.y + translate.y, pivot.z + translate.z);

				Math::Matrix pivotScale, pivotScaleRotate;
				pivotScale = Math::MatrixMultiply(pivotM, scaleM);
				pivotScaleRotate = Math::MatrixMultiply(pivotScale, rotateM);
				localTransform = Math::MatrixMultiply(pivotScaleRotate, translateM);
#else
				math::MatrixRotationQuaternion(&m_local_transform, &m_rotate);
				math::Vec4Scale(&m_local_transform.x, &m_local_transform.x, m_scale.x);
				math::Vec4Scale(&m_local_transform.y, &m_local_transform.y, m_scale.y);
				math::Vec4Scale(&m_local_transform.z, &m_local_transform.z, m_scale.z);
				fnd::MemCpy(&m_local_transform.w, &m_translate, sizeof(m_translate));
#endif
				InvalidateWorldTransform();
				InvalidateWorldBounds();
			}
		}

		// ワールドトランスフォーム更新
		void GameObject::UpdateWorldTransform()
		{
			if (worldTransformDirty)
			{
				std::shared_ptr<GameObject> parent = this->parent.lock();
				if (parent != nullptr)
				{
					worldTransform = Math::MatrixMultiply(localTransform, parent->worldTransform);
				}
				else
				{
					worldTransform = localTransform;
				}

				worldTransformDirty = false;
				worldTransformUpdated = true;
			}
		}

		// ワールドバウンディングボックス更新
		void GameObject::UpdateWorldBounds()
		{
			if (boundsDirty)
			{
				worldBounds = localBounds;
				worldBounds.Transform(worldTransform);

				boundsDirty = false;
				boundsUpdated = true;
			}
		}

		// ローカルトランスフォーム更新通知
		void GameObject::InvalidateLocalTransform()
		{
			localTransformDirty = true;
		}

		// ワールドトランスフォーム更新通知
		void GameObject::InvalidateWorldTransform()
		{
			worldTransformDirty = true;
			InvalidateWorldBounds();

			for (auto&& child : children)
			{
				child->InvalidateWorldTransform();
			}
		}

		// ワールドバウンディングボックス更新通知
		void GameObject::InvalidateWorldBounds()
		{
			boundsDirty = true;

			std::shared_ptr<GameObject> parent = this->parent.lock();
			if (parent != nullptr)
			{
				parent->InvalidateWorldBounds();
			}
		}

		// セットアップ
		void GameObject::Setup(IGame* game)
		{
			for (auto&& component : components)
			{
				component->Awake(game);
			}

			boundsDirty = true;
			localTransformDirty = true;
			boundsDirty = true;
			UpdateLocalTransform();
			UpdateWorldTransform();
			UpdateWorldBounds();

			for (auto&& child : children)
			{
				child->Setup(game);
			}

		}

		// インスタンス生成後の解決処理
		void GameObject::Resolve()
		{
			for (auto&& child : children)
			{
				child->Resolve();
			}
		}

		// 破棄
		void GameObject::Destroy(IGame* game)
		{
			for (auto&& component : components)
			{
				component->OnDestroy(game);
			}

			for (auto&& child : children)
			{
				child->Destroy(game);
			}
		}
	}
}