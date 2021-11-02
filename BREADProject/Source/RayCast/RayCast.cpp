#include "RayCast.h"
#include "../Source/Graphics/Model/ModelResource.h"
#include "../Source/Graphics/Mesh/Win/Mesh.h"
#include "FrameWork/Component/SpatialIndex.h"
#include "FND/Instance.h"

#define USESIMD

namespace Bread
{
    namespace FrameWork
    {
        //初期化
        void RayCastCom::Initialize()
        {
            SetID(GetOwner()->GetID());

            hitFlag = false;
            useFlag = true;
            SetStartPosition({ 0.0f,0.0f ,0.0f });
        }

        //更新
        void RayCastCom::Update()
        {
        }

        //描画
        void RayCastCom::Draw()
        {
        }

        //レイとモデルの交差判定
        bool RayCastCom::IntersectRayVsModel(TerrainManager& terrainManager)
        {
            using Math::Matrix;
            using Math::Vector3;

            //ポリゴンの所有者であるアクターのTransformを取得する
            //GetWorldTransformはDirtyFlagが実装されるので新しいWorldTransformが返ってくる
            const Matrix WorldTransform       { GetOwner()->GetComponent<Transform>()->GetWorldTransform() };
            const Matrix InverseWorldTransform{ MatrixInverse(WorldTransform)        };

            // レイの長さ
            //レイキャストに必要な変数の構築
            const Vector3 Start { Math::Vector3TransformCoord(start,InverseWorldTransform) };
            const Vector3 End   { Math::Vector3TransformCoord(end  ,InverseWorldTransform) };
            const Vector3 Vec   { Math::Vector3Subtract      (End  ,Start)                 };
            const Vector3 Dir   { Math::Vector3Normalize     (Vec)                         };
            const f32     Length{ Math::Vector3Length        (Vec)                         };

            //最短距離を示す変数
            f32 neart{ Length };

            //判定前に判定結果を初期化
            hitFlag = false;

            //対象のポリゴンの数ループする
            ImGui::Begin("TerrainManager");
            ImGui::Text(GetOwner()->GetID().c_str());
            auto targetFace{ terrainManager.GetSpatialFaces(GetOwner()->GetComponent<SpatialIndexComponent>()->GetSpatialIndex()) };
            ImGui::Text("PolygonInSpatial : %d", targetFace.size());
            ImGui::Separator();
            ImGui::End();

            for (auto& face : targetFace)
            {
                //頂点数が３未満のものは早期リターン
                if (face.vertex.size() < 3)
                {
                    continue;
                }

                //各頂点データが構築されてなければ早期リターン
                if (!face.vertex.at(0) || !face.vertex.at(1) || !face.vertex.at(2))
                {
                    continue;
                }

                //頂点座標を構築
#if 1
                const Vector3 A{ Vector3TransformCoord(face.vertex.at(0),InverseWorldTransform) };
                const Vector3 B{ Vector3TransformCoord(face.vertex.at(1),InverseWorldTransform) };
                const Vector3 C{ Vector3TransformCoord(face.vertex.at(2),InverseWorldTransform) };
#else
                Matrix mA{ Math::MatrixTranslation(face.vertex.at(0).x, face.vertex.at(0).y, face.vertex.at(0).z) };
                Matrix mB{ Math::MatrixTranslation(face.vertex.at(1).x, face.vertex.at(1).y, face.vertex.at(1).z) };
                Matrix mC{ Math::MatrixTranslation(face.vertex.at(2).x, face.vertex.at(2).y, face.vertex.at(2).z) };
                const Vector3 A{ Math::GetLocation( Math::MatrixMultiply(mA, InverseWorldTransform)) };
                const Vector3 B{ Math::GetLocation( Math::MatrixMultiply(mB, InverseWorldTransform)) };
                const Vector3 C{ Math::GetLocation( Math::MatrixMultiply(mC, InverseWorldTransform)) };
#endif

                // 三角形の三辺ベクトルを算出
               const Vector3 AB { Vector3Subtract(B, A)};
               const Vector3 BC { Vector3Subtract(C, B)};
               const Vector3 CA { Vector3Subtract(A, C)};

                // 三角形の法線ベクトルを算出
               const Vector3 Normal{/* Vector3Normalize*/(Vector3Cross(AB, BC)) };

                // 内積の結果がプラスならば裏向きなのでスキップ
                const f32 dot{ Vector3Dot(Dir, Normal) };
                if (dot >= 0.0f) continue;// 垂直の場合もスキップする（壁だから？） ※θは90°の時0になるので内積は0になる　内積 = |A||B|cosθ
                                          // 角度に変換して判断するようにすれば、登れる角度と登れない角度を判断して処理できる？

                 // レイと平面の交点を算出
                const f32  T{ Vector3Dot(Vector3Subtract(A, Start), Normal) / dot }; // xの長さスカラー（交点までの長さ）
                if (T < 0.0f && T > neart) continue;       // 交点までの距離が今までに計算した最近距離より 大きいときはスキップ

                // 交点が三角形の内側にあるか判定
                auto CheckInside = [](const Vector3& vertex, const Vector3& pos, const Vector3& vec,const Vector3& normal) {
                    const Vector3 v    { Vector3Subtract(vertex , pos)    };
                    const Vector3 cross{ Vector3Cross   (v      , vec)    };
                    const f32     dot  { Vector3Dot     (cross, normal)   };

                    return dot < 0.0f;
                };

                // ベクトルに始点の位置を与える
                const Vector3 Position{ Start + (Dir * T) };

                // １つ目
                if (CheckInside(A, Position, AB, Normal)) continue;

                // ２つ目
                if (CheckInside(B, Position, BC, Normal))continue;

                // ３つ目
                if (CheckInside(C, Position, CA, Normal))continue;

                // 交点と法線を更新
                neart = T;   // 最短距離を更新

                // 外積とその長さ
#if 1
                Vector3 WorldPosition{ Math::Vector3TransformCoord(Position, WorldTransform) };
//#else
                Vector3 WorldPosition1{ Math::MultiplyMatrixVector(WorldTransform,Position) };
#endif
                Vector3 WorldNormal  { Vector3Normalize(Vector3TransformCoord(Normal  , WorldTransform)) };
                const Vector3 WorldCrossVec    { Vector3Subtract(WorldPosition  , start) };
                const f32     WorldCrossLength { Vector3Length  (WorldCrossVec    ) };

                // ヒット情報保存
                if (Vector3Length(Vector3Subtract(end, start)) > WorldCrossLength)
                {
                    hitResult.position = WorldPosition;
                    hitResult.normal   = WorldNormal;
                    hitResult.distance = WorldCrossLength;
                    hitResult.start    = start;
                    hitResult.end      = end;

                    hitFlag = true;
                    break;
                }
            }

            if (!targetFace.size() || !hitFlag)
            {
                hitResult.position = { 0.0f,0.0f, 0.0f };
                hitResult.normal   = { 0.0f ,0.0f ,0.0f };
                hitResult.distance = 0.0f;
                hitResult.start    = start;
                hitResult.end      = end;
            }
            return hitFlag;
        }
    }
}
