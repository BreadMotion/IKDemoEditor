#include "RayCast.h"
#include "../Source/Graphics/Model/ModelResource.h"
#include "../Source/Graphics/Mesh/Win/Mesh.h"
#include <thread>

#define USESIMD

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

            Vector3 WorldStart{ start };
            Vector3 WorldEnd{ end };

            std::shared_ptr<Actor>     terain{ targetTarrain->GetOwner() };
            std::shared_ptr<Transform> transform{ terain->GetComponent<Transform>() };

            std::vector<ModelObject::Face::VertexIndex>& face{ targetTarrain->GetFaces()->at(0).face };

            for (auto& index : *targetFaceIndex)
            {
                hitFlag = false;
                // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
                Matrix WorldTransform{ transform->GetWorldTransform() };
                Matrix InverseWorldTransform{ MatrixInverse(WorldTransform) }; // �t�s���

                Vector3 Start{ Vector3TransformCoord(WorldStart, InverseWorldTransform) };
                Vector3 End{   Vector3TransformCoord(WorldEnd, InverseWorldTransform) };
                Vector3 Vec{   Vector3Subtract(End, Start) };
                Vector3 Dir{   Vector3Normalize(Vec) };
                f32 Length{    Vector3Length(Vec) };

                // ���C�̒���
                f32 neart{ Length };

                Vector3 A{ face.at(index).vertex[0]};
                Vector3 B{ face.at(index).vertex[1]};
                Vector3 C{ face.at(index).vertex[2]};

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
                Vector3 AB { Vector3Subtract(B, A)};
                Vector3 BC { Vector3Subtract(C, B)};
                Vector3 CA { Vector3Subtract(A, C)};

                // �O�p�`�̖@���x�N�g�����Z�o
                Vector3 Normal = Vector3Cross(AB, BC);

                // ���ς̌��ʂ��v���X�Ȃ�Η�����
                f32 dot = Vector3Dot(Dir, Normal);

                if (dot >= 0.0f) continue;// �����̏ꍇ���X�L�b�v����i�ǂ�����H�j ���Ƃ�90���̎�0�ɂȂ�̂œ��ς�0�ɂȂ�@���� = |A||B|cos��
                    // �p�x�ɕϊ����Ĕ��f����悤�ɂ���΁A�o���p�x�Ɠo��Ȃ��p�x�𔻒f���ď����ł���H

                 // ���C�ƕ��ʂ̌�_���Z�o
                Vector3 V{ Vector3Subtract(A, Start) };
                f32     T{ Vector3Dot(V, Normal) / dot }; // x�̒����X�J���[�i��_�܂ł̒����j
                f32 t{ T };
                if (t < 0.0f || t > neart) continue;       // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������ �傫���Ƃ��̓X�L�b�v

                Vector3 Position{ Start + (Dir * T) }; // �x�N�g���Ɏn�_�̈ʒu��^����

                // ��_���O�p�`�̓����ɂ��邩����
                // �P��
                Vector3 V1{     Vector3Subtract(A, Position) };
                Vector3 Cross1{ Vector3Cross(V1, AB) };
                f32     Dot1{   Vector3Dot(Cross1, Normal) };
                dot = Dot1;
                if (dot < 0.0f) continue;

                // �Q��
                Vector3 V2{     Vector3Subtract(B, Position) };
                Vector3 Cross2{ Vector3Cross(V2, BC) };
                f32     Dot2{   Vector3Dot(Cross2, Normal) };
                dot = Dot2;
                if (dot < 0.0f) continue;

                // �R��
                Vector3 V3{     Vector3Subtract(C, Position) };
                Vector3 Cross3{ Vector3Cross(V3, CA) };
                f32     Dot3{   Vector3Dot(Cross3, Normal) };
                dot = Dot3;
                if (dot < 0.0f) continue;

                // ��_�Ɩ@�����X�V
                Vector3 HitPosition{ Position };
                Vector3 HitNormal{ Normal };
                neart       = t;   // �ŒZ�������X�V

                // ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
                Vector3 WorldPosition{ Vector3TransformCoord(HitPosition, WorldTransform) };
                Vector3 WorldNormal  { Vector3TransformCoord(HitNormal, WorldTransform) };
                Vector3 WorldCrossVec{ Vector3Subtract(WorldPosition, WorldStart) };
                f32 WorldCrossLength{  Vector3Length(WorldCrossVec) };
                f32 distance{ WorldCrossLength };

                // �q�b�g���ۑ�
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

            // ���[���h��Ԃ̃��C�̒���
            SetDistance(vDistance);

            auto __fastcall faceRoadFunc([&](const u32& firstIt, const u32& endIt) {
                for (u32 index = firstIt; index <= endIt - 1; index++)
                {
                    Matrix WorldTransform        = transform->GetWorldTransform();
                    Matrix InverseWorldTransform = MatrixInverse(WorldTransform); // �t�s���

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

            // ���[���h��Ԃ̃��C�̒���
            SetDistance(vDistance);

            auto __fastcall faceRoadFunc([&](const u32& firstIt, const u32& endIt) {
                for (u32 index = firstIt; index <= endIt - 1; index++)
                {
                    Matrix WorldTransform{ transform->GetWorldTransform() };
                    Matrix InverseWorldTransform{ MatrixInverse(WorldTransform) }; // �t�s���

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
