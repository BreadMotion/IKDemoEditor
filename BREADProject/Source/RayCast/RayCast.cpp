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
        bool RayCastCom::IntersectRayVsModel()
        {
            using namespace Math;

            std::shared_ptr<Actor>     terain   { targetTarrain->GetOwner()         };
            std::shared_ptr<Transform> transform{ terain->GetComponent<Transform>() };

            for (auto& face : targetFace)
            {
                hitFlag = false;

                Matrix WorldTransform       { transform->GetWorldTransform() };

                Vector3 Start { start };
                Vector3 End   { end   };
                Vector3 Vec   { Vector3Subtract (End       , Start) };
                Vector3 Dir   { Vector3Normalize(Vec)               };
                f32     Length{ Vector3Length   (Vec)               };

                // ���C�̒���
                f32 neart{ Length };

                if (face.vertex.size() < 3)continue;
                if (!face.vertex.at(0) || !face.vertex.at(1) || !face.vertex.at(2))continue;
                Vector3 A{ face.vertex.at(0)};
                Vector3 B{ face.vertex.at(1)};
                Vector3 C{ face.vertex.at(2)};

                // �O�p�`�̎O�Ӄx�N�g�����Z�o
                Vector3 AB { Vector3Subtract(B, A)};
                Vector3 BC { Vector3Subtract(C, B)};
                Vector3 CA { Vector3Subtract(A, C)};

                // �O�p�`�̖@���x�N�g�����Z�o
                Vector3 Normal{ Vector3Cross(AB, BC) };

                // ���ς̌��ʂ��v���X�Ȃ�Η�����
                f32 dot{ Vector3Dot(Dir, Normal) };

                if (dot >= 0.0f) continue;// �����̏ꍇ���X�L�b�v����i�ǂ�����H�j ���Ƃ�90���̎�0�ɂȂ�̂œ��ς�0�ɂȂ�@���� = |A||B|cos��
                                          // �p�x�ɕϊ����Ĕ��f����悤�ɂ���΁A�o���p�x�Ɠo��Ȃ��p�x�𔻒f���ď����ł���H

                 // ���C�ƕ��ʂ̌�_���Z�o
                Vector3 V{ Vector3Subtract(A, Start)   };
                f32     T{ Vector3Dot(V, Normal) / dot }; // x�̒����X�J���[�i��_�܂ł̒����j
                f32     t{ T                           };
                if (t < 0.0f || t > neart) continue;       // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������ �傫���Ƃ��̓X�L�b�v

                Vector3 Position{ Start + (Dir * T) }; // �x�N�g���Ɏn�_�̈ʒu��^����

                // ��_���O�p�`�̓����ɂ��邩����
                // �P��
                Vector3 V1    { Vector3Subtract(A     , Position)};
                Vector3 Cross1{ Vector3Cross   (V1    , AB      )};
                f32     Dot1  { Vector3Dot     (Cross1, Normal  )};
                dot = Dot1;
                if (dot < 0.0f) continue;

                // �Q��
                Vector3 V2    { Vector3Subtract(B, Position)    };
                Vector3 Cross2{ Vector3Cross   (V2, BC)         };
                f32     Dot2  { Vector3Dot     (Cross2, Normal) };
                dot = Dot2;
                if (dot < 0.0f) continue;

                // �R��
                Vector3 V3    { Vector3Subtract(C, Position)    };
                Vector3 Cross3{ Vector3Cross   (V3, CA)         };
                f32     Dot3  { Vector3Dot     (Cross3, Normal) };
                dot = Dot3;
                if (dot < 0.0f) continue;

                // ��_�Ɩ@�����X�V
                Vector3 HitPosition{ Position };
                Vector3 HitNormal  { Normal   };
                neart = t;   // �ŒZ�������X�V

                // �O�ςƂ��̒�������
                Vector3 WorldCrossVec    { Vector3Subtract(HitPosition  , start         ) };
                f32     WorldCrossLength { Vector3Length  (WorldCrossVec                ) };

                // �q�b�g���ۑ�
                if (Vector3Length(Vector3Subtract(end, start)) > WorldCrossLength)
                {
                    hitResult.position = HitPosition;
                    hitResult.normal   = HitNormal;
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
