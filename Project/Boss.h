#pragma once

#include "GameDefine.h"
#include "Enemy.h"

#define BOSS_PARTS_MAX (3)

class CBoss {
private:
	CMeshContainer		m_Mesh;			//�{�X�{�̂̃��b�V��
	CMeshContainer		m_PartsMesh;	//���͂̃p�[�c�̃��b�V��
	Vector3				m_Pos;			//���W
	Vector3				m_Rot;			//��]�p�x�i���W�A���j
	CMatrix44			m_matWorld;		//�S�̂̃��[���h�}�g���N�X
	bool				m_bShow;		//�\�����

	int					m_HP;			//�̗�
	int					m_ShotWait;		//�e���˂̑҂�����
	Vector3				m_TargetPos;	//�e��]�A�j���[�V�����I����Ɍ������W
	float				m_AnimTime;		//��]�A�j���[�V�����̌o�ߎ���

	CEnemy				m_Parts[BOSS_PARTS_MAX];	//���͂̃p�[�c�Ǘ��pCEnemy�̃C���X�^���X

public:
	CBoss();
	~CBoss();

	/**
	 * �ǂݍ���
	 *
	 */
	bool Load();

	/**
	 * ������
	 *
	 */
	void Initialize();

	/**
	 * �J�n
	 *
	 */
	void Start();

	/**
	 * �X�V
	 *
	 */
	void Update(CEnemyShot* shot, int smax);

	/**
	 * �_���[�W����
	 *
	 */
	void Damage(int dmg);

	/**
	 * �`��
	 *
	 */
	void Render();

	/**
	 * �f�o�b�O�`��
	 *
	 */
	void RenderDebug();

	/**
	 * �\����Ԏ擾
	 *  �߂�l��true�Ȃ�\�����Afalse�Ȃ��\��
	 */
	bool GetShow() { return m_bShow; }

	/**
	 * �Փˋ���Ԃ�
	 *
	 */
	CSphere GetSphere() { return CSphere(m_Pos, 3.1f); }

	/**
	 * �^�[�Q�b�g���W�̐ݒ�
	 *
	 */
	void SetTargetPos(const Vector3& t) { m_TargetPos = t; }

	/**
	 * i�Ԗڂ̃p�[�c���擾
	 *
	 */
	CEnemy& GetParts(int i) { return m_Parts[i]; }

	/**
	 * �\�����̃p�[�c�̐���Ԃ�
	 *
	 */
	int GetPartsCount();

	/**
	 * ���
	 *
	 */
	void Release();

	/**
	 * ����̕����։�]����
	 *
	 */
	void RotateTarget()
	{
		Vector3 direction = m_TargetPos - (m_Pos);
		float dr = atan2(direction.x, direction.z) + MOF_MATH_PI;
		float r = fmodf((dr - m_Rot.y) + MOF_MATH_PI, MOF_MATH_2PI);
		m_Rot.y += ((0 < r) ? r - MOF_MATH_PI : r + MOF_MATH_PI) * 0.1f;
	}

	/**
	 * �S���ʂւ̒e����
	 *
	 */
	void ShotAllDirShot(CEnemyShot* shot, int smax, int sCnt)
	{
		if (m_ShotWait > 0)
		{
			m_ShotWait--;
			return;
		}

		float ad = ((float)rand() / RAND_MAX) * 360.0f / sCnt;
		for (int cnt = 0; cnt < sCnt; cnt++)
		{
			CEnemyShot* newShot = CEnemyShot::FindAvailableShot(shot, smax);
			if (newShot)
			{
				m_ShotWait = 20;

				float rad = MOF_ToRadian(ad);
				Vector3 vt(cos(rad), 0, sin(rad));
				newShot->Fire(m_Pos, vt * 0.2f);
			}
			ad += 360.0f / sCnt;
		}
	}

	/**
	 * �p�[�c�X�V
	 *
	 */
	void UpdateParts(int idx, CEnemyShot* shot, int smax, bool bShot)
	{
		CEnemy& parts = m_Parts[idx];
		float angle = -(MOF_MATH_2PI * idx / BOSS_PARTS_MAX);

		CVector3 p(0.0f, 3.5f, 0.0f);
		p.RotationZ(angle);
		p *= m_matWorld;
		parts.SetPosition(p);

		CVector3 r(m_Rot);
		r.z += angle;
		parts.SetRotation(r);

		if (g_BossAnimPosY[1].Time <= m_AnimTime && bShot)
		{
			parts.SetTargetPos(m_TargetPos);
			parts.Update(shot, smax);
		}
	}
};