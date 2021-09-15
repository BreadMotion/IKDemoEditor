#include "RayCast.h"
#include "../Source/Graphics/Model/ModelResource.h"
#include "../Source/Graphics/Mesh/Win/Mesh.h"
#include <thread>

namespace Bread
{
    namespace FrameWork
    {
        //������
        void RayCastCom::Initialize()
        {
            SetID(GetOwner()->GetID());

            hitFlag  = false;
            useFlag = true;
            SetStartPosition({ 0.0f,0.0f ,0.0f });
        }

        //�X�V
        void RayCastCom::Update(const f32& dt)
        {
        }

        //�`��
        void RayCastCom::Draw(const f32& dt)
        {
        }

        //���C�ƃ��f���̌�������
        bool RayCastCom::IntersectRayVsModel()
        {
            using namespace Math;

            Vector3 WorldStart = start;
            Vector3 WorldEnd   = end;

            std::shared_ptr<Actor>     terain    = targetTarrain->GetOwner();
            std::shared_ptr<Transform> transform = terain->GetComponent<Transform>();

            std::vector<ModelObject::Face::VertexIndex>& face = targetTarrain->GetFaces()->at(0).face;

            for (auto& index : *targetFaceIndex)
            {
                hitFlag = false;
                // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
                Matrix WorldTransform        = transform->GetWorldTransform();
                Matrix InverseWorldTransform = MatrixInverse(WorldTransform); // �t�s���

                Vector3 Start = Vector3TransformCoord(WorldStart, InverseWorldTransform);
                Vector3 End   = Vector3TransformCoord(WorldEnd, InverseWorldTransform);
                Vector3 Vec   = Vector3Subtract(End, Start);
                Vector3 Dir   = Vector3Normalize(Vec);
                f32 Length    = Vector3Length(Vec);

                // ���C�̒���
                f32 neart = Length;

                Vector3 HitPosition;
                Vector3 HitNormal;

                Vector3 A = face.at(index).vertex[0];
                Vector3 B = face.at(index).vertex[1];
                Vector3 C = face.at(index).vertex[2];

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
                Vector3 AB = Vector3Subtract(B, A);
                Vector3 BC = Vector3Subtract(C, B);
                Vector3 CA = Vector3Subtract(A, C);

                // �O�p�`�̖@���x�N�g�����Z�o
                Vector3 Normal = Vector3Cross(AB, BC);

                // ���ς̌��ʂ��v���X�Ȃ�Η�����
                f32 dot = Vector3Dot(Dir, Normal);

                if (dot >= 0.0f) continue;// �����̏ꍇ���X�L�b�v����i�ǂ�����H�j ���Ƃ�90���̎�0�ɂȂ�̂œ��ς�0�ɂȂ�@���� = |A||B|cos��
                    // �p�x�ɕϊ����Ĕ��f����悤�ɂ���΁A�o���p�x�Ɠo��Ȃ��p�x�𔻒f���ď����ł���H

                 // ���C�ƕ��ʂ̌�_���Z�o
                Vector3 V = Vector3Subtract(A, Start);
                f32     T = Vector3Dot(V, Normal) / dot; // x�̒����X�J���[�i��_�܂ł̒����j
                f32 t = T;
                if (t < 0.0f || t > neart) continue;       // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������ �傫���Ƃ��̓X�L�b�v

                Vector3 Position = Start + (Dir * T); // �x�N�g���Ɏn�_�̈ʒu��^����

                // ��_���O�p�`�̓����ɂ��邩����
                // �P��
                Vector3 V1     = Vector3Subtract(A, Position);
                Vector3 Cross1 = Vector3Cross(V1, AB);
                f32     Dot1   = Vector3Dot(Cross1, Normal);
                dot = Dot1;
                if (dot < 0.0f) continue;

                // �Q��
                Vector3 V2     = Vector3Subtract(B, Position);
                Vector3 Cross2 = Vector3Cross(V2, BC);
                f32     Dot2   = Vector3Dot(Cross2, Normal);
                dot = Dot2;
                if (dot < 0.0f) continue;

                // �R��
                Vector3 V3     = Vector3Subtract(C, Position);
                Vector3 Cross3 = Vector3Cross(V3, CA);
                f32     Dot3   = Vector3Dot(Cross3, Normal);
                dot = Dot3;
                if (dot < 0.0f) continue;

                // ��_�Ɩ@�����X�V
                HitPosition = Position;
                HitNormal   = Normal;
                neart       = t;   // �ŒZ�������X�V

                // ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
                Vector3 WorldPosition  = Vector3TransformCoord(HitPosition, WorldTransform);
                Vector3 WorldNormal    = Vector3TransformCoord(HitNormal, WorldTransform);
                Vector3 WorldCrossVec  = Vector3Subtract(WorldPosition, WorldStart);
                f32 WorldCrossLength   = Vector3Length(WorldCrossVec);
                f32 distance = WorldCrossLength;

                // �q�b�g���ۑ�
                if (hitResult.distance > distance)
                {

                    hitResult.position = WorldPosition;
                    hitResult.normal   = WorldNormal;
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

            std::shared_ptr<Actor>     terain    = targetTarrain->GetOwner();
            std::shared_ptr<Transform> transform = terain->GetComponent<Transform>();

            std::vector<ModelObject::Face::VertexIndex>& face = targetTarrain->GetFaces()->at(0).face;

            Vector WorldRayVec = end - start;
            WorldRayVec     *= WorldRayVec;
            Vector vDistance = _mm_sqrt_ps(_mm_dp_ps(WorldRayVec.v, WorldRayVec.v, 0x7f));

            // ���[���h��Ԃ̃��C�̒���
            SetDistance(vDistance.StoreVector().x);

            auto __fastcall faceRoadFunc([&](const u32& firstIt, const u32& endIt) {
                for (u32 index = firstIt; index <= endIt - 1; index++)
                {
                    Matrix WorldTransform = transform->GetWorldTransform();
                    Matrix InverseWorldTransform = MatrixInverse(WorldTransform); // �t�s���

                    Vector3 Start = Vector3TransformCoord(start, InverseWorldTransform);
                    Vector3 End = Vector3TransformCoord(end, InverseWorldTransform);
                    Vector3 Vec = Vector3Subtract(End, Start);
                    Vector3 Dir = Vector3Normalize(Vec);
                    f32 Length  = Vector3Length(Vec);

                    if (face.at(index).vertex.size() <= 2)continue;

                    Vector3 A = face.at(index).vertex[0];
                    Vector3 B = face.at(index).vertex[1];
                    Vector3 C = face.at(index).vertex[2];

                    Vector3 ave = (A + B + C) / 3.0f;
                    Vector3 aveVec = Vector3Subtract(ave, Start);
                    f32 aveLength = Vector3Length(aveVec);


                    if (aveLength <= Length + VariableLengthSearch)
                    {
                        targetFaceIndex->emplace_back(index);
                    }
                }
                });

            targetFaceIndex->clear();

            constexpr u32 threadNum = 10;
            constexpr u32 nextNum = 1;
            u32 faceRate = (face.size() / threadNum);
            std::thread th1(faceRoadFunc, 0 * faceRate, (0 + nextNum) * faceRate);
            std::thread th2(faceRoadFunc, 1 * faceRate, (1 + nextNum) * faceRate);
            std::thread th3(faceRoadFunc, 2 * faceRate, (2 + nextNum) * faceRate);
            std::thread th4(faceRoadFunc, 3 * faceRate, (3 + nextNum) * faceRate);
            std::thread th5(faceRoadFunc, 4 * faceRate, (4 + nextNum) * faceRate);
            std::thread th6(faceRoadFunc, 5 * faceRate, (5 + nextNum) * faceRate);
            std::thread th7(faceRoadFunc, 6 * faceRate, (6 + nextNum) * faceRate);
            std::thread th8(faceRoadFunc, 7 * faceRate, (7 + nextNum) * faceRate);
            std::thread th9(faceRoadFunc, 8 * faceRate, (8 + nextNum) * faceRate);
            std::thread th10(faceRoadFunc, 9 * faceRate, (9 + nextNum) * faceRate);

            th1.join();
            th2.join();
            th3.join();
            th4.join();
            th5.join();
            th6.join();
            th7.join();
            th8.join();
            th9.join();
            th10.join();
        }
    }
}
