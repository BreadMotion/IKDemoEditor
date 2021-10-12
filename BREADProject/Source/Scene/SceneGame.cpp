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
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Component/VelocityMap.h"
#include "FrameWork/Component/CCDIK.h"

#include "../Player/PlayerComponent.h"
#include "../Stage/StageComponent.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Component/ComponentManager.h"
#include "FrameWork/Object/TerrainManager.h"

#include "FrameWork/Input/InputDevice.h"
#include "FrameWork/Shader/SkyMapShader.h"

#include "Graphics/RenderManager.h"

//RenderManagerを作ってSceneGameのDraw関数の中身をすっきりさせる予定

using namespace Bread;
using namespace Bread::Math;

using FND::Instance;
using FND::SharedInstance;
using FND::UniqueInstance;
using FND::MapInstance;

int FrameWork::Transform::thisEntityNum = 0;
const char* FrameWork::Transform::SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

void  __fastcall Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16);
void  __fastcall Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16);
void  __fastcall OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16);
template<class... T> int __fastcall count_arguments(T... args) { return sizeof...(args); }

void SceneGame::Construct(SceneSystem* sceneSystem)
{
	this->sceneSystem = sceneSystem;
}

void SceneGame::Initialize()
{
	//アクターの生成＆初期化
	{
		using namespace Bread::FrameWork;
		SharedInstance<OS::ResourceManager>::makeInstancePtr()->Initialize(nullptr);

		Instance<ActorManager>::instance.AddActor<Actor>("stage")->AddComponent<StageComponent>();
		Instance<ActorManager>::instance.AddActor<Actor>("camera")->AddComponent<Graphics::Camera>();
		Instance<ActorManager>::instance.AddActor<Actor>("player")->AddComponent<PlayerComponent>();
	}

	//カメラの初期化
	{
		std::shared_ptr<Graphics::Camera> camera{
			Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };
		camera->SetEye({ 600.0f ,-500.0f ,0.0f });
		camera->SetRotateX(0.5f);
		camera->SetRotateY(ToRadian(180.0f));
		camera->SetTargetPos({ 600.0f ,0.0f ,0.0f }, Vector3::Zero);
		camera->SetTarget({ 600.0f ,0.0f ,0.0f },    Vector3::Zero);
		camera->SetLookAt({ 600.0f ,-500.0f ,0.0f }, { 600.0f ,0.0f ,0.0f }, Vector3{ 0.0f,1.0f,0.0f });

		camera->Update();
	}


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

	Instance<ActorManager>::instance.PreUpdate();

	SetupGUI();
	GUI();

	if (selectAct)
	{
		for (auto& act : Instance<ActorManager>::instance.GetAllActor())
		{
			if (selectAct == act)
			{
				std::shared_ptr<Transform> actorTransform{ act->GetComponent<Transform>() };
				Matrix matrix{ actorTransform->GetWorldTransform() };

				ImGuizmoUpdate(matrix.f);

				actorTransform->SetScale(GetScale(matrix));
				actorTransform->SetRotate(GetRotation(matrix));
				actorTransform->SetTranslate(GetLocation(matrix));
				actorTransform->Update();

				break;
			}
		}
	}
	Instance<ActorManager>::instance.Update();
	Instance<ActorManager>::instance.NextUpdate();
}

void SceneGame::Draw()
{
	Instance<Graphics::RenderManager>::instance.Render();
}

void SceneGame::SetupGUI()
{
	using namespace ImGui;

	ImGuiIO& io{ ImGui::GetIO() };

	std::shared_ptr<Graphics::Camera> camera{
		Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };
	f32    fov{ camera->GetFovY() };
	Matrix pro{ camera->GetProjection() };
	float  viewWidth{ 10.f }; // for orthographic

	{//setUP viewtype
		Graphics::ViewType viewType{ camera->GetViewType() };
		if (viewType == Graphics::ViewType::Perspective)
		{
			Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, pro.f);
			ImGuizmo::SetOrthographic(false);
		}
		else if ((viewType == Graphics::ViewType::Orthographic))
		{
			float viewHeight{ viewWidth * io.DisplaySize.y / io.DisplaySize.x };
			OrthoGraphic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, pro.f);
			ImGuizmo::SetOrthographic(true);
		}
	}
}

void SceneGame::ImGuizmoUpdate(float* ary)
{
	using namespace Bread::FrameWork;
	ImGuiIO& io = ImGui::GetIO();

	std::shared_ptr<Graphics::Camera> camera {
		Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

	Bread::Math::Matrix m{ camera->GetView() };

	Actor* actor{ Instance<FrameWork::ActorManager>::instance.GetActorFromID("player").get() };
	std::shared_ptr<Transform> transform = actor->GetComponent<Transform>();

	static bool editTransform = true;

	const Matrix camMat{ camera->GetView() };
	Matrix camPro{ camera->GetProjection() };

	ImGuizmo::BeginFrame();
	ImGui::SetNextWindowPos(ImVec2(350, std::fabsf(256 - UniqueInstance<OS::DisplayWin>::instance->GetHeight())));
	ImGui::SetNextWindowSize(ImVec2(UniqueInstance<OS::DisplayWin>::instance->GetWidth() - 350, 256));

	ImGui::Begin(u8"シーケンサー");

	ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
	//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	ImGui::Separator();

	ImGuizmo::SetID(0);
	transform->EditTransform(camera.get(), camMat.f, camPro.f, ary, true);

	transform->SequenceEditorGUI();
	ImGui::End();

	const float disatnce = camera->GetDistanceFromLookAt();
	ImGuizmo::ViewManipulate(m.f, disatnce, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);
}

#if 1
void SceneGame::GUI()
{
	using namespace ImGui;
	using namespace Bread;
	using namespace Bread::FrameWork;
	ImGuiIO& io{ ImGui::GetIO() };

	std::shared_ptr<Graphics::Camera> camera {
		Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>()};

	static bool outlineWindow           { true};
	static bool componentWindow         { true};
	static bool controllRasterizeWindow { true};

	if (BeginMainMenuBar())
	{
		if (BeginMenu("File"))
		{
			if (MenuItem("dump"))
			{
			}
			ImGui::EndMenu();
		}
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
		OS::DisplayWin* dis{ dynamic_cast<OS::DisplayWin*>(UniqueInstance<OS::DisplayWin>::instance.get()) };
		ImGui::Text("fps : %f", dis->fpsVal);
		ImGui::Text("frameRate : %f", dis->rate);
		ImGui::EndMainMenuBar();
	}


	int rastIndex{ 0 };
	if (controllRasterizeWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(500 , 25));
		ImGui::SetNextWindowSize(ImVec2(100, UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 800));

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

	auto setCameraView([](Transform* m, Graphics::Camera* camera) {
		Vector3 target{ GetLocation(m->GetWorldTransform()) };
		f32 xSin { sinf(camera->GetRotateX())};
		f32 xCos { cosf(camera->GetRotateX())};
		f32 ySin { sinf(camera->GetRotateY())};
		f32 yCos { cosf(camera->GetRotateY())};

		Vector3 front  { -xCos * ySin, -xSin, -xCos * yCos };
		Vector3 _right { yCos, 0.0f, -ySin };
		Vector3 _up    { Math::Vector3Cross(_right, front) };

		f32     distance { camera->GetDistanceFromLookAt() };
		Vector3 _target  { target };
		Vector3 _distance{ distance, distance, distance };
		Vector3 _pos     { _target - (front * _distance) };

		camera->SetEye(_pos);
		camera->SetTarget(_target, Vector3::Zero);
		camera->SetFocus(_target);
		camera->SetLookAt(_pos, _target, _up);
		camera->DataUpdate();
		});

	if (outlineWindow)
	{
		SetNextWindowPos(ImVec2(0, ((UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 255))));
		SetNextWindowSize(ImVec2(350, 255));
		Begin(u8"アウトライン");

		ImGuiTreeNodeFlags treeNodeFlags{ ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiSelectableFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen };
		ImGuiSelectableFlags selectFlags{ ImGuiSelectableFlags_DontClosePopups };

		auto createActorFunction{ [&](Actor* owner) {
			ImGui::SetNextWindowSize(ImVec2(300.0f,200.0f));
			if (ImGui::BeginPopupModal("Create Actor Setting"))
			{
				static char actName[128]     {};
				std::string createActor_name {};
				ImGui::Text("ActorName"); ImGui::SameLine();
				ImGui::InputText("##edit", actName, IM_ARRAYSIZE(actName));
				createActor_name = actName;

				static int item{ 1 };
				std::vector<std::string> actorsCombo =
				{ "actor\0" , "player\0" , "stage\0" , "camera\0" , "IKTarget\0" };
				std::string  allActorsCombo = {};
				for (auto& combo : actorsCombo)
				{
					allActorsCombo += combo + '\0';
				}

				ImGui::Combo("Actor", &item, allActorsCombo.c_str() + '\0');

				if (createActor_name.size() <= 0)
				{
					ImGui::Text(u8"×無名のアクターは存在できません");
				}

				//if (ImGui::Button("Create"))
				//{
				//	std::shared_ptr<Actor> chashActor{ nullptr };
				//	if (createActor_name.size())
				//	{
				//		switch (item)
				//		{
				//		case 1://normal Actor
				//			if (owner)
				//			{
				//				chashActor = owner->AddChildActor<Actor>();
				//			}
				//			else
				//			{
				//				Instance<ActorManager>::instance.AddActor<Actor>(actorsCombo[item + 1], Actor::Create());
				//				chashActor = Instance<ActorManager>::instance.GetActorFromID(actorsCombo[item + 1]);
				//			}
				//			break;
				//		}
				//		chashActor->SetID(actorsCombo[item + 1]);
				//		ImGui::CloseCurrentPopup();
				//	}

				//}ImGui::SameLine();

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

		for (auto& actor : Instance<ActorManager>::instance.GetAllActor())
		{
			//アクターの格納階層
			if (ImGui::TreeNodeEx((actor->GetID() + ("- FIle")).c_str(), treeNodeFlags))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Create Actor"))
					{
						ImGui::OpenPopup("Create Actor Setting");
					}
					createActorFunction(nullptr);
					ImGui::EndPopup();
				}

				//アクターの選択
				if (ImGui::Selectable(actor->GetID().c_str(), selectAct == actor ,selectFlags | ImGuiTreeNodeFlags_FramePadding))
				{
					selectAct = actor;

					//注視点を選択アクターに移す
					if (std::shared_ptr<Transform> transform = actor->GetComponent<Transform>(); transform != nullptr)
					{
						setCameraView(transform.get(), camera.get());
					}
				}

				auto actorTree = FND::Lambda{ [&](auto f, std::vector<std::shared_ptr<Actor>>& act) -> std::vector<std::shared_ptr<Actor>>*{
				for (auto& chilAct : act)
				{
					//アクターの格納階層
					if (ImGui::TreeNodeEx((chilAct->GetID() + (" - File")).c_str(), treeNodeFlags))
					{
						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::Button("Create Actor"))
							{
								ImGui::OpenPopup("Create Actor Setting");
							}
							createActorFunction(chilAct.get());
							ImGui::EndPopup();
						}

						//アクターの選択
						if (ImGui::Selectable(chilAct->GetID().c_str(), selectAct == chilAct, selectFlags))
						{
							selectAct = chilAct;

							//注視点を選択アクターに移す
							if (std::shared_ptr<Transform> transform = chilAct->GetComponent<Transform>(); transform != nullptr)
							{
								setCameraView(transform.get(), camera.get());
							}
						}

						//再帰
						f(chilAct->GetAllChildActor());

						ImGui::TreePop();
					}
				}
				return &act;
				} }(actor->GetAllChildActor());

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	if (componentWindow)
	{
		SetNextWindowSize(ImVec2(350, UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 280.0f));
		SetNextWindowPos(ImVec2(0.0f, 25.0f));
		Begin(u8"コンポーネント");
		if (selectAct)
		{
			char  name[128] {};
			FND::StrCpy(name, sizeof(name), selectAct->GetID().c_str());
			ImGui::Text("ActorName"); ImGui::SameLine();
			ImGui::InputText(("##" + selectAct->GetID()).c_str(), name, IM_ARRAYSIZE(name));
			selectAct->SetID(name);

			for (auto& component : selectAct->GetAllComponent())
			{
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