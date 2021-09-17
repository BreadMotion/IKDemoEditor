#include "RayCast.h"
#include "../Source/Graphics/Model/ModelResource.h"
#include "../Source/Graphics/Mesh/Win/Mesh.h"
#include <thread>

#define USESIMD

namespace Bread
{
    namespace FrameWork
    {
        //初期化
        void RayCastCom::Initialize()
        {
            SetID(GetOwner()->GetID());

            hitFlag  = false;
            useFlag = true;
            SetStartPosition({ 0.0f,0.0f ,0.0f });
        }

        //更新
        void RayCastCom::Update(const f32& dt)
        {
        }

        //描画
        void RayCastCom::Draw(const f32& dt)
        {
        }

        //レイとモデルの交差判定
        bool RayCastCom::IntersectRayVsModel()
        {
            using namespace Math;

            Vector3 WorldStart{ start };
            Vector3 WorldEnd{ end };

            std::shared_ptr<Actor>     terain{ targetTarrain->GetOwner() };
            std::shared_ptr<Transform> transform{ terain->GetComponent<Transform>() };

            std::vector<ModelObject::Face::VertexIndex>& face{ targetTarrain->GetFaces()->at(0).face };

            for (auto& index : *targetFaceIndex)
            {
                hitFlag = false;
                // レイをワールド空間からローカル空間へ変換
                Matrix WorldTransform{ transform->GetWorldTransform() };
                Matrix InverseWorldTransform{ MatrixInverse(WorldTransform) }; // 逆行列へ

                Vector3 Start{ Vector3TransformCoord(WorldStart, InverseWorldTransform) };
                Vector3 End{   Vector3TransformCoord(WorldEnd, InverseWorldTransform) };
                Vector3 Vec{   Vector3Subtract(End, Start) };
                Vector3 Dir{   Vector3Normalize(Vec) };
                f32 Length{    Vector3Length(Vec) };

                // レイの長さ
                f32 neart{ Length };

                Vector3 A{ face.at(index).vertex[0]};
                Vector3 B{ face.at(index).vertex[1]};
                Vector3 C{ face.at(index).vertex[2]};

                // 三角形の三辺ベクトルを算出
                Vector3 AB { Vector3Subtract(B, A)};
                Vector3 BC { Vector3Subtract(C, B)};
                Vector3 CA { Vector3Subtract(A, C)};

                // 三角形の法線ベクトルを算出
                Vector3 Normal = Vector3Cross(AB, BC);

                // 内積の結果がプラスならば裏向き
                f32 dot = Vector3Dot(Dir, Normal);

                if (dot >= 0.0f) continue;// 垂直の場合もスキップする（壁だから？） ※θは90°の時0になるので内積は0になる　内積 = |A||B|cosθ
                    // 角度に変換して判断するようにすれば、登れる角度と登れない角度を判断して処理できる？

                 // レイと平面の交点を算出
                Vector3 V{ Vector3Subtract(A, Start) };
                f32     T{ Vector3Dot(V, Normal) / dot }; // xの長さスカラー（交点までの長さ）
                f32 t{ T };
                if (t < 0.0f || t > neart) continue;       // 交点までの距離が今までに計算した最近距離より 大きいときはスキップ

                Vector3 Position{ Start + (Dir * T) }; // ベクトルに始点の位置を与える

                // 交点が三角形の内側にあるか判定
                // １つ目
                Vector3 V1{     Vector3Subtract(A, Position) };
                Vector3 Cross1{ Vector3Cross(V1, AB) };
                f32     Dot1{   Vector3Dot(Cross1, Normal) };
                dot = Dot1;
                if (dot < 0.0f) continue;

                // ２つ目
                Vector3 V2{     Vector3Subtract(B, Position) };
                Vector3 Cross2{ Vector3Cross(V2, BC) };
                f32     Dot2{   Vector3Dot(Cross2, Normal) };
                dot = Dot2;
                if (dot < 0.0f) continue;

                // ３つ目
                Vector3 V3{     Vector3Subtract(C, Position) };
                Vector3 Cross3{ Vector3Cross(V3, CA) };
                f32     Dot3{   Vector3Dot(Cross3, Normal) };
                dot = Dot3;
                if (dot < 0.0f) continue;

                // 交点と法線を更新
                Vector3 HitPosition{ Position };
                Vector3 HitNormal{ Normal };
                neart       = t;   // 最短距離を更新

                // ローカル空間からワールド空間へ変換
                Vector3 WorldPosition{ Vector3TransformCoord(HitPosition, WorldTransform) };
                Vector3 WorldNormal  { Vector3TransformCoord(HitNormal, WorldTransform) };
                Vector3 WorldCrossVec{ Vector3Subtract(WorldPosition, WorldStart) };
                f32 WorldCrossLength{  Vector3Length(WorldCrossVec) };
                f32 distance{ WorldCrossLength };

                // ヒット情報保存
                if (hitResult.distance > distance)
                {

                    hitResult.position = WorldPosition;
                    hitResult.normal   = Vector3Normalize(WorldNormal);
                    hitResult.distance = distance;
                    hitResult.start    = start;
                    hitResult.end      = end;

                    hitFlag = true;
                    break;
                }
            }

            if (!targetFaceIndex->size() || !hitFlag)
            {
                hitResult.position = { 0.0f,0.0f, 0.0f };
                hitResult.normal   = { 0.0f ,0.0f ,0.0f };
                hitResult.distance = 0.0f;
                hitResult.start    = start;
                hitResult.end      = end;
            }
            return hitFlag;
        }

        void RayCastCom::TargetFaceRegister()
        {
            using namespace Math;

            std::shared_ptr<Actor>     terain{ targetTarrain->GetOwner() };
            std::shared_ptr<Transform> transform{ terain->GetComponent<Transform>() };

            std::vector<ModelObject::Face::VertexIndex>& face{ targetTarrain->GetFaces()->at(0).face };

#if !defined USESIMD
            f32 vDistance = VectorLength(Vector{ end - start }.v);

            // ワールド空間のレイの長さ
            SetDistance(vDistance);

            auto __fastcall faceRoadFunc([&](const u32& firstIt, const u32& endIt) {
                for (u32 index = firstIt; index <= endIt - 1; index++)
                {
                    Matrix WorldTransform        = transform->GetWorldTransform();
                    Matrix InverseWorldTransform = MatrixInverse(WorldTransform); // 逆行列へ

                    Vector Start{ Vector3TransformCoord(start, InverseWorldTransform) };
                    Vector End{ Vector3TransformCoord(end, InverseWorldTransform) };
                    Vector Vec{ End - Start };
                    Vector Dir{ VectorNormalize(Vec)};
                    f32 Length{ VectorLength(Vec)};

                    if (face.at(index).vertex.size() <= 2)continue;

                    Vector A{ face.at(index).vertex[0]};
                    Vector B = face.at(index).vertex[1];
                    Vector C = face.at(index).vertex[2];

                    constexpr f32 polygonVertexNum = 3;
                    f32 aveLength{ VectorLength((((A + B + C) / polygonVertexNum) - Start).v) };

                    if (aveLength <= Length + VariableLengthSearch)
                    {
                        targetFaceIndex->emplace_back(index);
                    }
                }
                });
#else
            f32 vDistance = VectorLength(Vector{ end - start }.v);

            // ワールド空間のレイの長さ
            SetDistance(vDistance);

            auto __fastcall faceRoadFunc([&](const u32& firstIt, const u32& endIt) {
                for (u32 index = firstIt; index <= endIt - 1; index++)
                {
                    Matrix WorldTransform{ transform->GetWorldTransform() };
                    Matrix InverseWorldTransform{ MatrixInverse(WorldTransform) }; // 逆行列へ

                    Vector3 Start{ Vector3TransformCoord(start, InverseWorldTransform) };
                    Vector3 End{ Vector3TransformCoord(end, InverseWorldTransform) };
                    Vector3 Vec{ End - Start };
                    Vector3 Dir{ Vector3Normalize(Vec) };
                    f32 Length{ Vector3Length(Vec) };

                    if (face.at(index).vertex.size() <= 2)continue;

                    Vector3 A{ face.at(index).vertex[0] };
                    Vector3 B{ face.at(index).vertex[1] };
                    Vector3 C{ face.at(index).vertex[2] };

                    constexpr f32 polygonVertexNum{ 3 };
                    f32 aveLength{ Vector3Length((((A + B + C) / polygonVertexNum) - Start).v) };

                    if (aveLength <= Length + VariableLengthSearch)
                    {
                        targetFaceIndex->emplace_back(index);
                    }
                }
                });
#endif
            targetFaceIndex->clear();

            std::vector<std::thread> threads;
            constexpr u32 threadNum{ 10 };
            constexpr u32 nextNum{ 1 };
            u32 faceRate = (face.size() / threadNum);

            for (u32 i = 0; i < threadNum; i++)
            {
                threads.emplace_back(faceRoadFunc, i * faceRate, (i + nextNum) * faceRate);
            }
            for (u32 i = 0; i < threadNum; i++)
            {
                threads[i].join();
            }
        }
    }
}
