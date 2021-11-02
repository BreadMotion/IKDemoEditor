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
        //������
        void RayCastCom::Initialize()
        {
            SetID(GetOwner()->GetID());

            hitFlag = false;
            useFlag = true;
            SetStartPosition({ 0.0f,0.0f ,0.0f });
        }

        //�X�V
        void RayCastCom::Update()
        {
        }

        //�`��
        void RayCastCom::Draw()
        {
        }

        //���C�ƃ��f���̌�������
        bool RayCastCom::IntersectRayVsModel(TerrainManager& terrainManager)
        {
            using Math::Matrix;
            using Math::Vector3;

            //�|���S���̏��L�҂ł���A�N�^�[��Transform���擾����
            //GetWorldTransform��DirtyFlag�����������̂ŐV����WorldTransform���Ԃ��Ă���
            const Matrix WorldTransform       { GetOwner()->GetComponent<Transform>()->GetWorldTransform() };
            const Matrix InverseWorldTransform{ MatrixInverse(WorldTransform)        };

            // ���C�̒���
            //���C�L���X�g�ɕK�v�ȕϐ��̍\�z
            const Vector3 Start { Math::Vector3TransformCoord(start,InverseWorldTransform) };
            const Vector3 End   { Math::Vector3TransformCoord(end  ,InverseWorldTransform) };
            const Vector3 Vec   { Math::Vector3Subtract      (End  ,Start)                 };
            const Vector3 Dir   { Math::Vector3Normalize     (Vec)                         };
            const f32     Length{ Math::Vector3Length        (Vec)                         };

            //�ŒZ�����������ϐ�
            f32 neart{ Length };

            //����O�ɔ��茋�ʂ�������
            hitFlag = false;

            //�Ώۂ̃|���S���̐����[�v����
            ImGui::Begin("TerrainManager");
            ImGui::Text(GetOwner()->GetID().c_str());
            auto targetFace{ terrainManager.GetSpatialFaces(GetOwner()->GetComponent<SpatialIndexComponent>()->GetSpatialIndex()) };
            ImGui::Text("PolygonInSpatial : %d", targetFace.size());
            ImGui::Separator();
            ImGui::End();

            for (auto& face : targetFace)
            {
                //���_�����R�����̂��̂͑������^�[��
                if (face.vertex.size() < 3)
                {
                    continue;
                }

                //�e���_�f�[�^���\�z����ĂȂ���Α������^�[��
                if (!face.vertex.at(0) || !face.vertex.at(1) || !face.vertex.at(2))
                {
                    continue;
                }

                //���_���W���\�z
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

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
               const Vector3 AB { Vector3Subtract(B, A)};
               const Vector3 BC { Vector3Subtract(C, B)};
               const Vector3 CA { Vector3Subtract(A, C)};

                // �O�p�`�̖@���x�N�g�����Z�o
               const Vector3 Normal{/* Vector3Normalize*/(Vector3Cross(AB, BC)) };

                // ���ς̌��ʂ��v���X�Ȃ�Η������Ȃ̂ŃX�L�b�v
                const f32 dot{ Vector3Dot(Dir, Normal) };
                if (dot >= 0.0f) continue;// �����̏ꍇ���X�L�b�v����i�ǂ�����H�j ���Ƃ�90���̎�0�ɂȂ�̂œ��ς�0�ɂȂ�@���� = |A||B|cos��
                                          // �p�x�ɕϊ����Ĕ��f����悤�ɂ���΁A�o���p�x�Ɠo��Ȃ��p�x�𔻒f���ď����ł���H

                 // ���C�ƕ��ʂ̌�_���Z�o
                const f32  T{ Vector3Dot(Vector3Subtract(A, Start), Normal) / dot }; // x�̒����X�J���[�i��_�܂ł̒����j
                if (T < 0.0f && T > neart) continue;       // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������ �傫���Ƃ��̓X�L�b�v

                // ��_���O�p�`�̓����ɂ��邩����
                auto CheckInside = [](const Vector3& vertex, const Vector3& pos, const Vector3& vec,const Vector3& normal) {
                    const Vector3 v    { Vector3Subtract(vertex , pos)    };
                    const Vector3 cross{ Vector3Cross   (v      , vec)    };
                    const f32     dot  { Vector3Dot     (cross, normal)   };

                    return dot < 0.0f;
                };

                // �x�N�g���Ɏn�_�̈ʒu��^����
                const Vector3 Position{ Start + (Dir * T) };

                // �P��
                if (CheckInside(A, Position, AB, Normal)) continue;

                // �Q��
                if (CheckInside(B, Position, BC, Normal))continue;

                // �R��
                if (CheckInside(C, Position, CA, Normal))continue;

                // ��_�Ɩ@�����X�V
                neart = T;   // �ŒZ�������X�V

                // �O�ςƂ��̒���
#if 1
                Vector3 WorldPosition{ Math::Vector3TransformCoord(Position, WorldTransform) };
//#else
                Vector3 WorldPosition1{ Math::MultiplyMatrixVector(WorldTransform,Position) };
#endif
                Vector3 WorldNormal  { Vector3Normalize(Vector3TransformCoord(Normal  , WorldTransform)) };
                const Vector3 WorldCrossVec    { Vector3Subtract(WorldPosition  , start) };
                const f32     WorldCrossLength { Vector3Length  (WorldCrossVec    ) };

                // �q�b�g���ۑ�
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
