#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

#include "scene.h"
#include "SceneSystem.h"

#include "Math/BreadMath.h"
#include "FND/Util.h"
#include "FND/Lambda.h"
#include "FND/Instance.h"

#include "../Source/Graphics//Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"
#include "OS/StartUp.h"
#include "../Source/OS/Display/Win/DisplayWin.h"

#include "FrameWork/Input/InputDevice.h"
#include "FrameWork/Object/Object.h"

#include "../Player/PlayerComponent.h"
#include "../Stage/StageComponent.h"
#include "../Stage/StageCollisionComponent.h"
#include "../IKTestObject/LinkingSphere.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Component/ComponentManager.h"
#include "FrameWork/Object/TerrainManager.h"

#include "Graphics/RenderManager.h"

//RenderManagerを作ってSceneGameのDraw関数の中身をすっきりさせる予定

using namespace Bread;
using namespace Bread::Math;

using FND::Instance;        //使用クラス : ActorManager    , RenderManager
using FND::MapInstance;     //使用クラス : TerrainManager
using FND::SharedInstance;  //使用クラス : ResourceManager , GraphicsDeviceDX11
using FND::UniqueInstance;  //使用クラス : DisplayWin

int         FrameWork::Transform::thisEntityNum            { 0 };
const char* FrameWork::Transform::SequencerItemTypeNames[] { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

void  __fastcall Frustum     (float left         , float right      , float bottom, float       top , float  znear, float       zfar, float* m16);
void  __fastcall Perspective (float fovyInDegrees, float aspectRatio, float znear , float       zfar, float* m16                                );
void  __fastcall OrthoGraphic(const float l      , float r          , float b     , const float t   , float  zn   , const float zf  , float* m16);
template<class... T> int __fastcall count_arguments(T... args) { return sizeof...(args); }

void SceneGame::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
}

void SceneGame::Initialize()
{
	//アクターの生成＆初期化
	{
		//ResourceManagerの初期設定
		SharedInstance<OS::ResourceManager>::makeInstancePtr()->Initialize(nullptr);

		//ゲーム内の初期生成アクター
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("stage")         ->AddComponent<FrameWork::StageComponent>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("stageCollision")->AddComponent<FrameWork::StageCollisionComponent>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("camera")        ->AddComponent<Graphics::Camera>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("player")        ->AddComponent<FrameWork::PlayerComponent>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("LinkingSphere") ->AddComponent<FrameWork::LinkingSphere>();
	}

	//カメラの初期化
	{
		std::shared_ptr<Graphics::Camera> camera{
			Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };
		camera->SetEye      ({ 600.0f ,-500.0f ,0.0f }                                                    );
		camera->SetRotateX  (0.5f                                                                          );
		camera->SetRotateY  (ToRadian(180.0f)                                                              );
		camera->SetTargetPos({ 600.0f ,0.0f ,0.0f },    Vector3::Zero                                     );
		camera->SetTarget   ({ 600.0f ,0.0f ,0.0f },    Vector3::Zero                                     );
		camera->SetLookAt   ({ 600.0f ,-500.0f ,0.0f }, { 600.0f ,0.0f ,0.0f }, Vector3{ 0.0f,1.0f,0.0f });

		camera->Update();
	}

	//RenderManagerの初期化
	Instance<Graphics::RenderManager>::instance.Initialize();
	// GPUパーティクル
	{
		//	testComputeShader->Initialize(graphicsDevice);
		//	bitonicSort->Initialize(graphicsDevice);
		//	gpuParticle->Initialize(graphicsDevice, "PlayerHitEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin.png", true); // particle
	}
}

void SceneGame::Update()
{
	using namespace Bread::FrameWork;

	//事前更新
	Instance<ActorManager>::instance.PreUpdate();

	//Guizmo処理
	SetupGUI();
	GUI();

	//ActorのSelectが起きていた場合の処理
	if (selectAct)
	{
		std::shared_ptr<Transform>
			actorTransform{ selectAct->GetComponent<Transform>() };
		Matrix matrix     { actorTransform->GetWorldTransform()  };

		//Guizmoの入力
		ImGuizmoUpdate(matrix.f);

		if (auto parentActor{ selectAct->GetParentActor<Actor>() })
		{
			auto parentT{ parentActor->GetComponent<Transform>() };
			Matrix invParentM{ Math::MatrixInverse(parentT->GetWorldTransform()) };
			matrix = invParentM * matrix;
		}

		//Guizmoによる変更結果を反映
		actorTransform->SetScale    (GetScale   (matrix));
		actorTransform->SetRotate   (GetRotation(matrix));
		actorTransform->SetTranslate(GetLocation(matrix));
	}

	//更新
	Instance<ActorManager>::instance.Update();

	//事後更新
	Instance<ActorManager>::instance.NextUpdate();

	//ステージタイプのアクターを管理nfomationを表示する
	MapInstance<TerrainManager>::instance["TerrainModelManager"]  .GUI();
	MapInstance<TerrainManager>::instance["CollisionModelManager"].GUI();
}

void SceneGame::Draw()
{
	//描画
	Instance<Graphics::RenderManager>::instance.Render();
}

void SceneGame::SetupGUI()
{
	using namespace ImGui;

	ImGuiIO& io{ ImGui::GetIO() };

	std::shared_ptr<Graphics::Camera> camera
	{ Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

	f32    fov    { camera->GetFovY()       };
	Matrix pro    { camera->GetProjection() };
	f32  viewWidth{ 10.f                    }; // for orthographic

	{//setUP viewtype
		Graphics::ViewType viewType{ camera->GetViewType() };

		//射影
		if (viewType == Graphics::ViewType::Perspective)
		{
			Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, pro.f);
			ImGuizmo::SetOrthographic(false);
		}
		//平行投影
		else if ((viewType == Graphics::ViewType::Orthographic))
		{
			f32 viewHeight{ viewWidth * io.DisplaySize.y / io.DisplaySize.x };
			OrthoGraphic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, pro.f);
			ImGuizmo::SetOrthographic(true);
		}
	}
}

void SceneGame::ImGuizmoUpdate(float* ary)
{
	ImGuiIO&    io           { ImGui::GetIO() };

	std::shared_ptr<Graphics::Camera> camera
	{ Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

	Matrix      m{ camera->GetView()      };
	Matrix camPro{ camera->GetProjection() };

	//Guizmoの構築
	ImGuizmo::BeginFrame();
	{
		//シーケンサーWindowの設定
		ImGui::SetNextWindowPos(ImVec2(350, std::fabsf(256 - UniqueInstance<OS::DisplayWin>::instance->GetHeight())));
		ImGui::SetNextWindowSize(ImVec2(UniqueInstance<OS::DisplayWin>::instance->GetWidth() - 350, 256));

		ImGui::Begin(u8"シーケンサー");
		{
			ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
			//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
			ImGui::Separator();

			ImGuizmo::SetID(0);

			//アウトラインWindowで選択したアクターがある場合
			if (selectAct)
			{
				std::shared_ptr<FrameWork::Transform> transform{ selectAct->GetComponent<FrameWork::Transform>() };

				//Guizmoによる入力の受付
				transform->EditTransform(camera.get(), m.f, camPro.f, ary, true);
				transform->SequenceEditorGUI();
			}
		}ImGui::End();

	}

	//Guizmoの表示
	ImGuizmo::ViewManipulate(m.f, camera->GetDistanceFromLookAt(), ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);
}

#if 1
void SceneGame::GUI()
{
	using namespace ImGui;

	ImGuiIO& io{ ImGui::GetIO() };

	std::shared_ptr<Graphics::Camera> camera
	{ Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

	static bool outlineWindow           { true };  //アウトラインウィンドウの展開フラグ
	static bool componentWindow         { true };  //コンポーネントウィンドウの展開フラグ
	static bool controllRasterizeWindow { true };  //ラスタライザーの表示設定ウィンドウの展開フラグ

	//メニューバーの展開
	if (BeginMainMenuBar())
	{
		//"File"オブジェクト展開
		if (BeginMenu("File"))
		{
			if (MenuItem("dump"))
			{
			}
			ImGui::EndMenu();
		}

		//"Window"オブジェクト展開
		if (BeginMenu("Window"))
		{
			if (MenuItem("open SceneGame window"))
			{
				mainWindow = !mainWindow;
			}
			if (MenuItem("open Outline Window"))
			{
				outlineWindow = !outlineWindow;
			}
			if (MenuItem("open component Window"))
			{
				componentWindow = !componentWindow;
			}
			if (MenuItem("open rasterize Window"))
			{
				controllRasterizeWindow = !controllRasterizeWindow;
			}
			ImGui::EndMenu();
		}

		//Display情報を表示
		OS::DisplayWin* dis{ dynamic_cast<OS::DisplayWin*>(UniqueInstance<OS::DisplayWin>::instance.get()) };
		ImGui::Text("fps : %f", dis->fpsVal);
		ImGui::Text("frameRate : %f", dis->rate);

		ImGui::EndMainMenuBar();
	}

	//ラスタライザーウィンドウの構築
	int rastIndex{ 0 };
	if (controllRasterizeWindow)
	{
		//ラスタライザーWindowの設定
		ImGui::SetNextWindowPos(ImVec2(500 , 25));
		ImGui::SetNextWindowSize(ImVec2(100, UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 800));

		//"ラスタライザー"の構築
		Begin(u8"ラスタライザー");
		for (; rastIndex <= (int)Graphics::RasterizerState::TypeNum;)
		{
			if (ImGui::Selectable(std::to_string(rastIndex).c_str(), selected == rastIndex))//TODO : 列挙型を文字列に変換できるようにする
			{
				selected = rastIndex;
				SharedInstance<Graphics::GraphicsDeviceDX11>::instance->rasterizerState = (Graphics::RasterizerState)rastIndex;
			}
			rastIndex++;
		}
		ImGui::End();
	}

	//カメラの設定
	auto setCameraView([](FrameWork::Transform* m, Graphics::Camera* camera) {
		Vector3 target{ GetLocation(m->GetWorldTransform()) };
		const f32 xSin { sinf(camera->GetRotateX())};
		const f32 xCos { cosf(camera->GetRotateX())};
		const f32 ySin { sinf(camera->GetRotateY())};
		const f32 yCos { cosf(camera->GetRotateY())};

		const Vector3 front  { -xCos * ySin, -xSin, -xCos * yCos };
		const Vector3 _right { yCos, 0.0f, -ySin };
		const Vector3 _up    { Math::Vector3Cross(_right, front) };

		const f32     distance { camera->GetDistanceFromLookAt() };
		const Vector3 _target  { target };
		const Vector3 _distance{ distance, distance, distance };
		const Vector3 _pos     { _target - (front * _distance) };

		camera->SetEye    (_pos);
		camera->SetTarget (_target, Vector3::Zero);
		camera->SetFocus  (_target);
		camera->SetLookAt (_pos, _target, _up);
		camera->DataUpdate();
		});

	//アウトラインウィンドウの構築
	if (outlineWindow)
	{
		//アウトラインウィンドウの設定
		SetNextWindowPos(ImVec2(0, ((UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 255))));
		SetNextWindowSize(ImVec2(350, 255));

		//アウトラインウィンドウの展開
		Begin(u8"アウトライン");

		ImGuiTreeNodeFlags treeNodeFlags{ ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiSelectableFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen };
		ImGuiSelectableFlags selectFlags{ ImGuiSelectableFlags_DontClosePopups };

		//アクターの生成ラムダ関数
		auto createActorFunction{ [&](FrameWork::Actor* owner) {
			ImGui::SetNextWindowSize(ImVec2(300.0f,200.0f));
			if (ImGui::BeginPopupModal("Create Actor Setting"))
			{
				//アクターの名前変更
				static char actName[128]     {};
				std::string createActor_name {};
				ImGui::Text("ActorName"); ImGui::SameLine();
				ImGui::InputText("##edit", actName, IM_ARRAYSIZE(actName));
				createActor_name = actName;

				//アクターの項目設定
				static int item{ 1 };
				std::vector<std::string> actorsCombo =
				{ "actor\0" , "player\0" , "stage\0" , "camera\0" , "IKTarget\0" };
				std::string  allActorsCombo = {};

				//アクターの項目前出力
				for (auto& combo : actorsCombo)
				{
					allActorsCombo += combo + '\0';
				}
				ImGui::Combo("Actor", &item, allActorsCombo.c_str() + '\0');

				//アクターが存在しない場合
				if (createActor_name.size() <= 0)
				{
					ImGui::Text(u8"×無名のアクターは存在できません");
				}

#if 0
				if (ImGui::Button("Create"))
				{
					std::shared_ptr<Actor> chashActor{ nullptr };
					if (createActor_name.size())
					{
						switch (item)
						{
						case 1://normal Actor
							if (owner)
							{
								chashActor = owner->AddChildActor<Actor>();
							}
							else
							{
								Instance<ActorManager>::instance.AddActor<Actor>(actorsCombo[item + 1], Actor::Create());
								chashActor = Instance<ActorManager>::instance.GetActorFromID(actorsCombo[item + 1]);
							}
							break;
						}
						chashActor->SetID(actorsCombo[item + 1]);
						ImGui::CloseCurrentPopup();
					}

				}ImGui::SameLine();
#endif

				//アウトラインウィンドウを閉じる
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		} };

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Create Actor"))
			{
				ImGui::OpenPopup("Create Actor Setting");
			}
			ImGui::EndPopup();
		}

		//全アクターループ
		for (auto& actor : Instance<FrameWork::ActorManager>::instance.GetAllActor())
		{
			//アクターの格納階層
			if (ImGui::TreeNodeEx((actor->GetID() + ("- FIle")).c_str(), treeNodeFlags))
			{
				//右クリックで選択すると展開
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Create Actor"))
					{
						ImGui::OpenPopup("Create Actor Setting");
					}
					//アクターの生成
					createActorFunction(nullptr);

					//BeginPopupContextItemの終了
					ImGui::EndPopup();
				}

				//アクターの選択
				if (ImGui::Selectable(actor->GetID().c_str(), selectAct == actor ,selectFlags | ImGuiTreeNodeFlags_FramePadding))
				{
					//選択されているアクターをselectActにアドレスを渡す
					selectAct = actor;

					//注視点を選択アクターに移す
					if (std::shared_ptr<FrameWork::Transform> transform = actor->GetComponent<FrameWork::Transform>(); transform != nullptr)
					{
						setCameraView(transform.get(), camera.get());
					}
				}

				auto actorTree = FND::Lambda
				{
					[&](auto f, std::vector<std::shared_ptr<FrameWork::Actor>>& act) -> std::vector<std::shared_ptr<FrameWork::Actor>>*
					{
						//子供があればノードを展開して再帰する
						for (auto& chilAct : act)
						{
							//アクターの格納階層
							if (ImGui::TreeNodeEx((chilAct->GetID() + (" - File")).c_str(), treeNodeFlags))
							{
								//右クリックで選択すると展開する
								if (ImGui::BeginPopupContextItem())
								{
									if (ImGui::Button("Create Actor"))
									{
										ImGui::OpenPopup("Create Actor Setting");
									}
									//アクターを生成する
									createActorFunction(chilAct.get());

									//BeginPopupContextItemの終了
									ImGui::EndPopup();
								}

								//アクターの選択
								if (ImGui::Selectable(chilAct->GetID().c_str(), selectAct == chilAct, selectFlags))
								{
									//選択されているアクターをselectActにアドレスを渡す
									selectAct = chilAct;

									//注視点を選択アクターに移す
									if (std::shared_ptr<FrameWork::Transform> transform = chilAct->GetComponent<FrameWork::Transform>(); transform != nullptr)
									{
										setCameraView(transform.get(), camera.get());
									}
								}
								//再帰
								f(chilAct->GetAllChildActor());

								//TreeNodeExを閉じる
								ImGui::TreePop();
							}
						}

						//一応帰りは必要はないはずだが返しておく
						return &act;
				    }
				}(actor->GetAllChildActor());

				ImGui::TreePop();
			}
		}
		//アクターウィンドウの終了
		ImGui::End();
	}

	//コンポーネントウィンドウを展開する
	if (componentWindow)
	{
		//コンポーネントウィンドウの設定
		SetNextWindowSize(ImVec2(350, UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 280.0f));
		SetNextWindowPos(ImVec2 (0.0f, 25.0f));

		//コンポーネントウィンドウの展開
		Begin(u8"コンポーネント");
		//アクターウィンドウで選択されたアクターがあった場合入る
		if (selectAct)
		{
			{   //アクターの名前の設定
				char  name[128]{};
				FND::StrCpy(name, sizeof(name), selectAct->GetID().c_str());
				ImGui::Text("ActorName"); ImGui::SameLine();
				ImGui::InputText(("##" + selectAct->GetID()).c_str(), name, IM_ARRAYSIZE(name));
				selectAct->SetID(name);
			}

			//全コンポーネントループ
			for (auto& component : selectAct->GetAllComponent())
			{
				//コンポーネントのGUIを呼び出す
				component->GUI();
				Separator();
			}
		}
		ImGui::End();
	}
}
#endif

void SceneGame::PrimitiveRender(
	Graphics::DeviceDX11* device,
	Vector3 translate, Vector3 rotate, Vector3 scale, f32 alpha)
{
	//std::shared_ptr<Graphics::Camera> camera{
	//	Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

	//// ワールド行列を作成
	//Matrix W;
	//{
	//	Matrix S, R, T;
	//	S = MatrixScaling(scale.x, scale.y, scale.z);
	//	R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//	T = MatrixTranslation(translate.x, translate.y, translate.z);

	//	W = S * R * T;
	//}

	//primitive->Render
	//(
	//	device->GetD3DContext(),
	//	ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
	//	ConvertToFloat4x4FromVector4x4(W),
	//	DirectX::XMFLOAT4(1, 1, 1, 1),
	//	DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, alpha),
	//	false
	//);
}

void SceneGame::CylinderPrimitiveRender(
	Graphics::DeviceDX11* device,
	Vector3 cp1Translate, Vector3 cp2Translate, Vector3 cyilinderTranslate,
	Vector3 rotate, Vector3 scale, Vector3 cyilinderScale)
{
	//std::shared_ptr<Graphics::Camera> camera{
	//		Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };
	//// Cylinder
	//{
	//	// ワールド行列を作成
	//	Matrix W;
	//	{
	//		Matrix S, R, T;
	//		S = MatrixScaling(cyilinderScale.x, cyilinderScale.y, cyilinderScale.z);
	//		R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//		T = MatrixTranslation(cyilinderTranslate.x, cyilinderTranslate.y, cyilinderTranslate.z);

	//		W = S * R * T;
	//	}

	//	cylinderPrimitive->Render
	//	(
	//		device->GetD3DContext(),
	//		ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
	//		ConvertToFloat4x4FromVector4x4(W),
	//		DirectX::XMFLOAT4(1, 1, 1, 1),
	//		DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
	//		false
	//	);
	//}
}

void __fastcall Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0f * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	m16[0] = temp / temp2;
	m16[1] = 0.0;
	m16[2] = 0.0;
	m16[3] = 0.0;
	m16[4] = 0.0;
	m16[5] = temp / temp3;
	m16[6] = 0.0;
	m16[7] = 0.0;
	m16[8] = (right + left) / temp2;
	m16[9] = (top + bottom) / temp3;
	m16[10] = (-zfar - znear) / temp4;
	m16[11] = -1.0f;
	m16[12] = 0.0;
	m16[13] = 0.0;
	m16[14] = (-temp * zfar) / temp4;
	m16[15] = 0.0;
}

void __fastcall Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
	xmax = ymax * aspectRatio;
	Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}

void __fastcall OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16)
{
	m16[0] = 2 / (r - l);
	m16[1] = 0.0f;
	m16[2] = 0.0f;
	m16[3] = 0.0f;
	m16[4] = 0.0f;
	m16[5] = 2 / (t - b);
	m16[6] = 0.0f;
	m16[7] = 0.0f;
	m16[8] = 0.0f;
	m16[9] = 0.0f;
	m16[10] = 1.0f / (zf - zn);
	m16[11] = 0.0f;
	m16[12] = (l + r) / (l - r);
	m16[13] = (t + b) / (b - t);
	m16[14] = zn / (zn - zf);
	m16[15] = 1.0f;
}