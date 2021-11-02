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

//RenderManager�������SceneGame��Draw�֐��̒��g���������肳����\��

using namespace Bread;
using namespace Bread::Math;

using FND::Instance;        //�g�p�N���X : ActorManager    , RenderManager
using FND::MapInstance;     //�g�p�N���X : TerrainManager
using FND::SharedInstance;  //�g�p�N���X : ResourceManager , GraphicsDeviceDX11
using FND::UniqueInstance;  //�g�p�N���X : DisplayWin

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
	//�A�N�^�[�̐�����������
	{
		//ResourceManager�̏����ݒ�
		SharedInstance<OS::ResourceManager>::makeInstancePtr()->Initialize(nullptr);

		//�Q�[�����̏��������A�N�^�[
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("stage")         ->AddComponent<FrameWork::StageComponent>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("stageCollision")->AddComponent<FrameWork::StageCollisionComponent>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("camera")        ->AddComponent<Graphics::Camera>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("player")        ->AddComponent<FrameWork::PlayerComponent>();
		Instance<FrameWork::ActorManager>::instance.AddActor<FrameWork::Actor>("LinkingSphere") ->AddComponent<FrameWork::LinkingSphere>();
	}

	//�J�����̏�����
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

	//RenderManager�̏�����
	Instance<Graphics::RenderManager>::instance.Initialize();
	// GPU�p�[�e�B�N��
	{
		//	testComputeShader->Initialize(graphicsDevice);
		//	bitonicSort->Initialize(graphicsDevice);
		//	gpuParticle->Initialize(graphicsDevice, "PlayerHitEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin.png", true); // particle
	}
}

void SceneGame::Update()
{
	using namespace Bread::FrameWork;

	//���O�X�V
	Instance<ActorManager>::instance.PreUpdate();

	//Guizmo����
	SetupGUI();
	GUI();

	//Actor��Select���N���Ă����ꍇ�̏���
	if (selectAct)
	{
		std::shared_ptr<Transform>
			actorTransform{ selectAct->GetComponent<Transform>() };
		Matrix matrix     { actorTransform->GetWorldTransform()  };

		//Guizmo�̓���
		ImGuizmoUpdate(matrix.f);

		if (auto parentActor{ selectAct->GetParentActor<Actor>() })
		{
			auto parentT{ parentActor->GetComponent<Transform>() };
			Matrix invParentM{ Math::MatrixInverse(parentT->GetWorldTransform()) };
			matrix = invParentM * matrix;
		}

		//Guizmo�ɂ��ύX���ʂ𔽉f
		actorTransform->SetScale    (GetScale   (matrix));
		actorTransform->SetRotate   (GetRotation(matrix));
		actorTransform->SetTranslate(GetLocation(matrix));
	}

	//�X�V
	Instance<ActorManager>::instance.Update();

	//����X�V
	Instance<ActorManager>::instance.NextUpdate();

	//�X�e�[�W�^�C�v�̃A�N�^�[���Ǘ�nfomation��\������
	MapInstance<TerrainManager>::instance["TerrainModelManager"]  .GUI();
	MapInstance<TerrainManager>::instance["CollisionModelManager"].GUI();
}

void SceneGame::Draw()
{
	//�`��
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

		//�ˉe
		if (viewType == Graphics::ViewType::Perspective)
		{
			Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, pro.f);
			ImGuizmo::SetOrthographic(false);
		}
		//���s���e
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

	//Guizmo�̍\�z
	ImGuizmo::BeginFrame();
	{
		//�V�[�P���T�[Window�̐ݒ�
		ImGui::SetNextWindowPos(ImVec2(350, std::fabsf(256 - UniqueInstance<OS::DisplayWin>::instance->GetHeight())));
		ImGui::SetNextWindowSize(ImVec2(UniqueInstance<OS::DisplayWin>::instance->GetWidth() - 350, 256));

		ImGui::Begin(u8"�V�[�P���T�[");
		{
			ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
			//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
			ImGui::Separator();

			ImGuizmo::SetID(0);

			//�A�E�g���C��Window�őI�������A�N�^�[������ꍇ
			if (selectAct)
			{
				std::shared_ptr<FrameWork::Transform> transform{ selectAct->GetComponent<FrameWork::Transform>() };

				//Guizmo�ɂ����͂̎�t
				transform->EditTransform(camera.get(), m.f, camPro.f, ary, true);
				transform->SequenceEditorGUI();
			}
		}ImGui::End();

	}

	//Guizmo�̕\��
	ImGuizmo::ViewManipulate(m.f, camera->GetDistanceFromLookAt(), ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);
}

#if 1
void SceneGame::GUI()
{
	using namespace ImGui;

	ImGuiIO& io{ ImGui::GetIO() };

	std::shared_ptr<Graphics::Camera> camera
	{ Instance<FrameWork::ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

	static bool outlineWindow           { true };  //�A�E�g���C���E�B���h�E�̓W�J�t���O
	static bool componentWindow         { true };  //�R���|�[�l���g�E�B���h�E�̓W�J�t���O
	static bool controllRasterizeWindow { true };  //���X�^���C�U�[�̕\���ݒ�E�B���h�E�̓W�J�t���O

	//���j���[�o�[�̓W�J
	if (BeginMainMenuBar())
	{
		//"File"�I�u�W�F�N�g�W�J
		if (BeginMenu("File"))
		{
			if (MenuItem("dump"))
			{
			}
			ImGui::EndMenu();
		}

		//"Window"�I�u�W�F�N�g�W�J
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

		//Display����\��
		OS::DisplayWin* dis{ dynamic_cast<OS::DisplayWin*>(UniqueInstance<OS::DisplayWin>::instance.get()) };
		ImGui::Text("fps : %f", dis->fpsVal);
		ImGui::Text("frameRate : %f", dis->rate);

		ImGui::EndMainMenuBar();
	}

	//���X�^���C�U�[�E�B���h�E�̍\�z
	int rastIndex{ 0 };
	if (controllRasterizeWindow)
	{
		//���X�^���C�U�[Window�̐ݒ�
		ImGui::SetNextWindowPos(ImVec2(500 , 25));
		ImGui::SetNextWindowSize(ImVec2(100, UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 800));

		//"���X�^���C�U�["�̍\�z
		Begin(u8"���X�^���C�U�[");
		for (; rastIndex <= (int)Graphics::RasterizerState::TypeNum;)
		{
			if (ImGui::Selectable(std::to_string(rastIndex).c_str(), selected == rastIndex))//TODO : �񋓌^�𕶎���ɕϊ��ł���悤�ɂ���
			{
				selected = rastIndex;
				SharedInstance<Graphics::GraphicsDeviceDX11>::instance->rasterizerState = (Graphics::RasterizerState)rastIndex;
			}
			rastIndex++;
		}
		ImGui::End();
	}

	//�J�����̐ݒ�
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

	//�A�E�g���C���E�B���h�E�̍\�z
	if (outlineWindow)
	{
		//�A�E�g���C���E�B���h�E�̐ݒ�
		SetNextWindowPos(ImVec2(0, ((UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 255))));
		SetNextWindowSize(ImVec2(350, 255));

		//�A�E�g���C���E�B���h�E�̓W�J
		Begin(u8"�A�E�g���C��");

		ImGuiTreeNodeFlags treeNodeFlags{ ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiSelectableFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen };
		ImGuiSelectableFlags selectFlags{ ImGuiSelectableFlags_DontClosePopups };

		//�A�N�^�[�̐��������_�֐�
		auto createActorFunction{ [&](FrameWork::Actor* owner) {
			ImGui::SetNextWindowSize(ImVec2(300.0f,200.0f));
			if (ImGui::BeginPopupModal("Create Actor Setting"))
			{
				//�A�N�^�[�̖��O�ύX
				static char actName[128]     {};
				std::string createActor_name {};
				ImGui::Text("ActorName"); ImGui::SameLine();
				ImGui::InputText("##edit", actName, IM_ARRAYSIZE(actName));
				createActor_name = actName;

				//�A�N�^�[�̍��ڐݒ�
				static int item{ 1 };
				std::vector<std::string> actorsCombo =
				{ "actor\0" , "player\0" , "stage\0" , "camera\0" , "IKTarget\0" };
				std::string  allActorsCombo = {};

				//�A�N�^�[�̍��ڑO�o��
				for (auto& combo : actorsCombo)
				{
					allActorsCombo += combo + '\0';
				}
				ImGui::Combo("Actor", &item, allActorsCombo.c_str() + '\0');

				//�A�N�^�[�����݂��Ȃ��ꍇ
				if (createActor_name.size() <= 0)
				{
					ImGui::Text(u8"�~�����̃A�N�^�[�͑��݂ł��܂���");
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

				//�A�E�g���C���E�B���h�E�����
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

		//�S�A�N�^�[���[�v
		for (auto& actor : Instance<FrameWork::ActorManager>::instance.GetAllActor())
		{
			//�A�N�^�[�̊i�[�K�w
			if (ImGui::TreeNodeEx((actor->GetID() + ("- FIle")).c_str(), treeNodeFlags))
			{
				//�E�N���b�N�őI������ƓW�J
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Create Actor"))
					{
						ImGui::OpenPopup("Create Actor Setting");
					}
					//�A�N�^�[�̐���
					createActorFunction(nullptr);

					//BeginPopupContextItem�̏I��
					ImGui::EndPopup();
				}

				//�A�N�^�[�̑I��
				if (ImGui::Selectable(actor->GetID().c_str(), selectAct == actor ,selectFlags | ImGuiTreeNodeFlags_FramePadding))
				{
					//�I������Ă���A�N�^�[��selectAct�ɃA�h���X��n��
					selectAct = actor;

					//�����_��I���A�N�^�[�Ɉڂ�
					if (std::shared_ptr<FrameWork::Transform> transform = actor->GetComponent<FrameWork::Transform>(); transform != nullptr)
					{
						setCameraView(transform.get(), camera.get());
					}
				}

				auto actorTree = FND::Lambda
				{
					[&](auto f, std::vector<std::shared_ptr<FrameWork::Actor>>& act) -> std::vector<std::shared_ptr<FrameWork::Actor>>*
					{
						//�q��������΃m�[�h��W�J���čċA����
						for (auto& chilAct : act)
						{
							//�A�N�^�[�̊i�[�K�w
							if (ImGui::TreeNodeEx((chilAct->GetID() + (" - File")).c_str(), treeNodeFlags))
							{
								//�E�N���b�N�őI������ƓW�J����
								if (ImGui::BeginPopupContextItem())
								{
									if (ImGui::Button("Create Actor"))
									{
										ImGui::OpenPopup("Create Actor Setting");
									}
									//�A�N�^�[�𐶐�����
									createActorFunction(chilAct.get());

									//BeginPopupContextItem�̏I��
									ImGui::EndPopup();
								}

								//�A�N�^�[�̑I��
								if (ImGui::Selectable(chilAct->GetID().c_str(), selectAct == chilAct, selectFlags))
								{
									//�I������Ă���A�N�^�[��selectAct�ɃA�h���X��n��
									selectAct = chilAct;

									//�����_��I���A�N�^�[�Ɉڂ�
									if (std::shared_ptr<FrameWork::Transform> transform = chilAct->GetComponent<FrameWork::Transform>(); transform != nullptr)
									{
										setCameraView(transform.get(), camera.get());
									}
								}
								//�ċA
								f(chilAct->GetAllChildActor());

								//TreeNodeEx�����
								ImGui::TreePop();
							}
						}

						//�ꉞ�A��͕K�v�͂Ȃ��͂������Ԃ��Ă���
						return &act;
				    }
				}(actor->GetAllChildActor());

				ImGui::TreePop();
			}
		}
		//�A�N�^�[�E�B���h�E�̏I��
		ImGui::End();
	}

	//�R���|�[�l���g�E�B���h�E��W�J����
	if (componentWindow)
	{
		//�R���|�[�l���g�E�B���h�E�̐ݒ�
		SetNextWindowSize(ImVec2(350, UniqueInstance<OS::DisplayWin>::instance->GetHeight() - 280.0f));
		SetNextWindowPos(ImVec2 (0.0f, 25.0f));

		//�R���|�[�l���g�E�B���h�E�̓W�J
		Begin(u8"�R���|�[�l���g");
		//�A�N�^�[�E�B���h�E�őI�����ꂽ�A�N�^�[���������ꍇ����
		if (selectAct)
		{
			{   //�A�N�^�[�̖��O�̐ݒ�
				char  name[128]{};
				FND::StrCpy(name, sizeof(name), selectAct->GetID().c_str());
				ImGui::Text("ActorName"); ImGui::SameLine();
				ImGui::InputText(("##" + selectAct->GetID()).c_str(), name, IM_ARRAYSIZE(name));
				selectAct->SetID(name);
			}

			//�S�R���|�[�l���g���[�v
			for (auto& component : selectAct->GetAllComponent())
			{
				//�R���|�[�l���g��GUI���Ăяo��
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

	//// ���[���h�s����쐬
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
	//	// ���[���h�s����쐬
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