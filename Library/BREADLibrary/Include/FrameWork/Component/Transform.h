#pragma once
#include <algorithm>

#include "FrameWork/Object/BasicObjectElement/INode.h"
#include "Component.h"
#include "../include/Graphics/Camera.h"
#include "Math/BREADMath.h"

#include "../../ExternalLibrary/ImGui/Include/imgui.h"

#include "../../../ExternalLibrary/ImGuizmo/ImGuizmo.h"
#include "../../../ExternalLibrary/ImGuizmo/ImSequencer.h"
#include "../../../ExternalLibrary/ImGuizmo/ImCurveEdit.h"
#include "../../../ExternalLibrary/ImGuizmo/ImGradient.h"

#include "FND/Util.h"
#include "FND/STD.h"
//#include "FND/DirtyFlag.h"

namespace Bread
{
	namespace FrameWork
	{
		class VelocityMap;
		class Transform : public Component, public FND::DirtyFlag
		{
		private:
			Math::Vector3    scale;     //ジョイントのスケール値
			Math::Quaternion rotate;    //ジョイントの回転値
			Math::Vector3    translate; //ジョイントの平行移動値

			Math::Vector3    oldTranslate{ Math::Vector3::Zero    };
			Math::Quaternion oldRotate   { Math::Quaternion::Zero };
			Math::Vector3    oldScale    { Math::Vector3::OneAll  };

			Math::Matrix localTransform;//ジョイントのローカル行列
			Math::Matrix worldTransform;//ジョイントのワールド行列

			bool modedPast{ false }; //過去フレームの変更を知らせる
			s32  myNumber { 0     };

		public:
			static int   thisEntityNum;

		public:
			explicit  Transform()
			{
				thisEntityNum++; myNumber = thisEntityNum;
			}
			~Transform() override {}

		public:// Component Interface
			// 初期化
			void Initialize() override
			{
				translate      = Math::Vector3   ::Zero;
				rotate         = Math::Quaternion::Zero;
				scale          = Math::Vector3   ::OneAll;
				localTransform = Math::Matrix    ::One;
				worldTransform = Math::Matrix    ::One;
			}

			//事前更新
			void __fastcall PreUpdate() override { modedPast = false; }

			// 更新
			void __fastcall Update()    override {}

			//事前更新
			void __fastcall NextUpdate()override {}

			// GUI
			void GUI()override
			{
				std::string guiName = "Transform : " + GetID();
				if (ImGui::CollapsingHeader(u8"トランスフォーム", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"名前"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					ImGui::DragFloat3("pos",    &translate.x);
					ImGui::DragFloat4("rotate", &rotate.x);

					static Math::Vector3 euler;
					euler = Math::ConvertToRollPitchYawFromQuaternion(rotate);
					ImGui::DragFloat3("euler", &euler.x);
					ImGui::DragFloat3("scale",            &scale.x);
				}
			}

		public://Transform interface
			// 移動値の設定
			void __fastcall SetTranslate(Math::Vector3 translate) { this->translate = translate; }

			// 回転値の設定
			void __fastcall SetRotate(Math::Quaternion rotate)    { this->rotate = rotate; }

			// スケール値の設定
			void __fastcall SetScale(Math::Vector3 scale)         { this->scale = scale; }

			const Math::Matrix& GetLocalTransform()
			{
				//DirtyFlagの条件を満たしているか調査する
				ResearchDirty();

				if (IsDirty())//変更があった場合
				{
					//拡大行列、回転行列、移動行列の生成
					Math::Matrix S{ Math::MatrixScaling(scale.x, scale.y, scale.z)                 };
					Math::Matrix R{ Math::MatrixRotationQuaternion(rotate)                         };
					Math::Matrix T{ Math::MatrixTranslation(translate.x, translate.y, translate.z) };

					//拡大行列 * 回転行列 * 移動行列
					localTransform = S * R * T;

					modedPast = true; //今回のフレームは更新が入ったことを知らせる
					ErasePast();      //old変数を更新する

					//childのワールド行列を変更
					UpdateChildTransform(GetOwner());
				}
				else //変更が無かった場合
				{
				}
				CleanDirtyFlag();//DirtyFlagをfalseに
				return localTransform;
			}
			virtual const Math::Matrix& GetWorldTransform()
			{
				if (auto parent = GetOwner()->GetParentActor<Actor>())
				{
					return worldTransform = GetLocalTransform() * parent->GetComponent<Transform>()->GetWorldTransform();
				}
				else
				{
					return worldTransform = GetLocalTransform();

				}
			}

			private://DirtyFlag Function
			//外部から値の変更があったか探索する
			//あったらフラグをセットし早期リターン
				void ResearchDirty()override
				{
					bool flag = false;
					if (flag = (oldTranslate != translate))
					{
						SetDirty(flag); return;
					}
					if (flag = (oldRotate != rotate))
					{
						SetDirty(flag); return;
					}
					if (flag = (oldScale != scale))
					{
						SetDirty(flag); return;
					}

					SetDirty(flag);
				}

				//過去値を更新
				void ErasePast()
				{
					oldScale     = scale;
					oldRotate    = rotate;
					oldTranslate = translate;
				}

		public:
			//過去の変更フラグを与える
			const bool& GetModedPast()
			{
				return modedPast;
			}

		private://TransformComponent内で使われる関数
			void UpdateChildTransform(std::shared_ptr<Actor> actor)
			{
				actor->GetComponent<Transform>()->GetWorldTransform();

				//子アクターの数再帰処理
				for (auto child : actor->GetAllChildActor())
				{
					//childActorのTransformComponentを取得
					//子に向けて再帰処理
					child->GetComponent<Transform>()->UpdateChildTransform(child);
				}
			}

		public://ImGui,ImGuizmo用関数
			//gizmoによるTransformの編集
			void __fastcall EditTransform(Bread::Graphics::Camera* camera, const float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
			{
#if 0
				//回転軸の入れ替え
				using namespace Bread::Math;
				{
					Vector3 translation{ GetLocation(matrix)                                      },
						    rotation   { ConvertToRollPitchYawFromQuaternion(GetRotation(matrix)) },
						    scale      { GetScale(matrix)                                         };

					Matrix localMatrix
					{
						MatrixScaling(scale.x, scale.y, scale.z) *
						MatrixRotationQuaternion(ConvertToQuaternionFromRollPitchYaw(rotation.x, rotation.y, rotation.z)) *
						MatrixTranslation(translation.x, translation.y, translation.z)
					};

					matrix = localMatrix.f;
				}
#endif

				//ImGuiで管理しているインデックス番号
				constexpr Bread::u32 tkey {84};
				constexpr Bread::u32 rkey {82};
				constexpr Bread::u32 ekey {69};

				//Guizmoの操作内容の設定
				static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
				static ImGuizmo::MODE      mCurrentGizmoMode(ImGuizmo::LOCAL);

				//Sequenceウィンドウの情報
				static bool  useSnap        { false                                 };
				static float snap[3]        { 1.0f, 1.0f, 1.0f                      };
				static float bounds[]       { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
				static float boundsSnap[]   { 0.1f, 0.1f, 0.1f                      };
				static bool  boundSizing    { false                                 };
				static bool  boundSizingSnap{ false                                 };

				if (editTransformDecomposition)
				{
					//操作する行列の設定
					if (ImGui::IsKeyPressed(tkey))
						mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
					if (ImGui::IsKeyPressed(rkey))
						mCurrentGizmoOperation = ImGuizmo::ROTATE;
					if (ImGui::IsKeyPressed(ekey))
						mCurrentGizmoOperation = ImGuizmo::SCALE;

					f32 matrixTranslation[3], matrixRotation[3], matrixScale[3];

					//行列をvector3(float[3])に変換
					ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

					{
						//ImGui側からの入力があれば更新
						ImGui::InputFloat3("Tr", matrixTranslation, 3);
						ImGui::InputFloat3("Rt", matrixRotation   , 3);
						ImGui::InputFloat3("Sc", matrixScale      , 3);

						//Vector3(float3)を各行列に変換
						ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);
					}

					//どちらの空間で値を変更するか設定
					if (mCurrentGizmoOperation != ImGuizmo::SCALE)
					{
						if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
							mCurrentGizmoMode = ImGuizmo::LOCAL;
						ImGui::SameLine();
						if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
							mCurrentGizmoMode = ImGuizmo::WORLD;
					}
					if (ImGui::IsKeyPressed(83))
						useSnap = !useSnap;
					ImGui::Checkbox("", &useSnap);
					ImGui::SameLine();

					//Guizmoの操作設定の内容別で変更する
					switch (mCurrentGizmoOperation)
					{
					case ImGuizmo::TRANSLATE:
						ImGui::InputFloat3("Snap",      &snap[0]);
						break;
					case ImGuizmo::ROTATE:
						ImGui::InputFloat("Angle Snap", &snap[0]);
						break;
					case ImGuizmo::SCALE:
						ImGui::InputFloat("Scale Snap", &snap[0]);
						break;
					}
					ImGui::Checkbox("Bound Sizing", &boundSizing);
					if (boundSizing)
					{
						ImGui::PushID(3);
						ImGui::Checkbox("", &boundSizingSnap);
						ImGui::SameLine();
						ImGui::InputFloat3("Snap", boundsSnap);
						ImGui::PopID();
					}
				}
				ImGuiIO& io{ ImGui::GetIO() };
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix,
					NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
			}

			//TODO : カメラの情報をどうやって持ってくるかを考える
			//トランスフォームを編集する(gizumo)
			void __fastcall TransformEditorGUI(Bread::Graphics::Camera* camera, const float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
			{
				static int lastUsing = -1;
				static const float identityMatrix[16] =
				{   1.f, 0.f, 0.f, 0.f,
				 	0.f, 1.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f
				};

				ImGuiIO& io{ ImGui::GetIO() };
				ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
				//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
				ImGui::Separator();

				for (int matId = 0; matId < thisEntityNum; matId++)
				{
					ImGuizmo::SetID(matId);
					EditTransform(camera, cameraView, cameraProjection, &matrix[matId], editTransformDecomposition);

					if (ImGuizmo::IsUsing())
					{
						lastUsing = matId;
					}
				}
			}

			void SequenceEditorGUI()
			{
				static int selectedEntry{ -1   };
				static int firstFrame   { 0    };
				static bool expanded    { true };
				static int currentFrame { 100  };

				ImGui::PushItemWidth(130);
				ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
				ImGui::SameLine();
				ImGui::InputInt("Frame ", &currentFrame);
				ImGui::SameLine();
				ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
				ImGui::PopItemWidth();

				ImSequencer::Sequencer(&mySequence, &currentFrame, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);
				// add a UI to edit that particular item
				if (selectedEntry != -1)
				{
					const MySequence::MySequenceItem& item = mySequence.myItems[selectedEntry];
					ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.mType]);
					// switch (type) ....
				}

			}

		public: // sequencerで使用する構造体
			const int maxSequenceItem{ 10 };
			static const char* SequencerItemTypeNames[10];

			struct RampEdit : public ImCurveEdit::Delegate
			{
				RampEdit()
				{
					mPts[0][0] = ImVec2(-10.f, 0);
					mPts[0][1] = ImVec2(20.f, 0.6f);
					mPts[0][2] = ImVec2(25.f, 0.2f);
					mPts[0][3] = ImVec2(70.f, 0.4f);
					mPts[0][4] = ImVec2(120.f, 1.f);
					mPointCount[0] = 5;

					mPts[1][0] = ImVec2(-50.f, 0.2f);
					mPts[1][1] = ImVec2(33.f, 0.7f);
					mPts[1][2] = ImVec2(80.f, 0.2f);
					mPts[1][3] = ImVec2(82.f, 0.8f);
					mPointCount[1] = 4;


					mPts[2][0] = ImVec2(40.f, 0);
					mPts[2][1] = ImVec2(60.f, 0.1f);
					mPts[2][2] = ImVec2(90.f, 0.82f);
					mPts[2][3] = ImVec2(150.f, 0.24f);
					mPts[2][4] = ImVec2(200.f, 0.34f);
					mPts[2][5] = ImVec2(250.f, 0.12f);
					mPointCount[2] = 6;
					mbVisible[0] = mbVisible[1] = mbVisible[2] = true;
					mMax = ImVec2(1.f, 1.f);
					mMin = ImVec2(0.f, 0.f);
				}
				size_t GetCurveCount()
				{
					return 3;
				}

				bool IsVisible(size_t curveIndex)
				{
					return mbVisible[curveIndex];
				}
				size_t GetPointCount(size_t curveIndex)
				{
					return mPointCount[curveIndex];
				}

				uint32_t GetCurveColor(size_t curveIndex)
				{
					uint32_t cols[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
					return cols[curveIndex];
				}
				ImVec2* GetPoints(size_t curveIndex)
				{
					return mPts[curveIndex];
				}
				virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const { return ImCurveEdit::CurveSmooth; }
				virtual int EditPoint(size_t curveIndex, int pointIndex, ImVec2 value)
				{
					mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
					SortValues(curveIndex);
					for (size_t i = 0; i < GetPointCount(curveIndex); i++)
					{
						if (mPts[curveIndex][i].x == value.x)
							return (int)i;
					}
					return pointIndex;
				}
				virtual void AddPoint(size_t curveIndex, ImVec2 value)
				{
					if (mPointCount[curveIndex] >= 8)
						return;
					mPts[curveIndex][mPointCount[curveIndex]++] = value;
					SortValues(curveIndex);
				}
				virtual ImVec2& GetMax() { return mMax; }
				virtual ImVec2& GetMin() { return mMin; }
				virtual unsigned int GetBackgroundColor() { return 0; }
				ImVec2 mPts[3][8];
				size_t mPointCount[3];
				bool mbVisible[3];
				ImVec2 mMin;
				ImVec2 mMax;
			private:
				void SortValues(size_t curveIndex)
				{
					auto b = std::begin(mPts[curveIndex]);
					auto e = std::begin(mPts[curveIndex]) + GetPointCount(curveIndex);
					std::sort(b, e, [](ImVec2 a, ImVec2 b) { return a.x < b.x; });

				}
			};

			struct MySequence : public ImSequencer::SequenceInterface
			{
				// interface with sequencer

				virtual int GetFrameMin() const {
					return mFrameMin;
				}
				virtual int GetFrameMax() const {
					return mFrameMax;
				}
				virtual int GetItemCount() const { return (int)myItems.size(); }

				virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
				virtual const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
				virtual const char* GetItemLabel(int index) const
				{
					static char tmps[512];
					sprintf_s(tmps, "[%02d] %s", index, SequencerItemTypeNames[myItems[index].mType]);
					return tmps;
				}

				virtual void Get(int index, int** start, int** end, int* type, unsigned int* color)
				{
					MySequenceItem& item = myItems[index];
					if (color)
						*color = 0xFFAA8080; // same color for everyone, return color based on type
					if (start)
						*start = &item.mFrameStart;
					if (end)
						*end = &item.mFrameEnd;
					if (type)
						*type = item.mType;
				}
				virtual void Add(int type) { myItems.push_back(MySequenceItem{ type, 0, 10, false }); };
				virtual void Del(int index) { myItems.erase(myItems.begin() + index); }
				virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

				virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

				// my datas
				MySequence() : mFrameMin(0), mFrameMax(0) {
					mFrameMin = -100;
					mFrameMax = 1000;
					myItems.push_back(MySequence::MySequenceItem{ 0, 10, 30, false });
					myItems.push_back(MySequence::MySequenceItem{ 1, 20, 30, true });
					myItems.push_back(MySequence::MySequenceItem{ 3, 12, 60, false });
					myItems.push_back(MySequence::MySequenceItem{ 2, 61, 90, false });
					myItems.push_back(MySequence::MySequenceItem{ 4, 90, 99, false });
				}
				int mFrameMin, mFrameMax;
				struct MySequenceItem
				{
					int  mType;
					int  mFrameStart, mFrameEnd;
					bool mExpanded;
				};
				std::vector<MySequenceItem> myItems;
				RampEdit                    rampEdit;

				virtual void DoubleClick(int index) {
					if (myItems[index].mExpanded)
					{
						myItems[index].mExpanded = false;
						return;
					}
					for (auto& item : myItems)
						item.mExpanded = false;
					myItems[index].mExpanded = !myItems[index].mExpanded;
				}
			};//初期化のシステムの組み方考えろ
			MySequence  mySequence;
		};
	}
}