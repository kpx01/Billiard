#include "pch.h"
#include "Collider.h"

Vector3 calcVel(Vector3, Vector3);

Collider::Collider(Vector3 pos, Vector3 velocity, float weight, float scale, bool freeze) :
	m_pos(pos),
	m_velocity(velocity),
	m_force(Vector3::Zero),
	m_gravity(Vector3(0.f, -10.f, 0.f)),
	m_time(0.f),
	m_scale(scale),
	m_weight(weight),
	m_restitution(1.f),
	m_freeze(freeze),
	m_hit(false),
	m_hitImpulse(false),
	m_hitBall(false),
	m_hitBallTime(1.f),
	m_preHitNormal(Vector3::Zero),
	m_hitTime(0.f)
{
}

Collider::~Collider()
{
}

void Collider::Update(float elapsedTime)
{
	m_prepos = m_pos;

	if (m_freeze) {
		m_velocity = Vector3::Zero;
	}
	else {
		m_velocity = m_velocity + (m_gravity - 1.f * m_velocity + m_force * (1.f / m_weight)) * elapsedTime;
	}

	if (m_hitBall) {
		Vector3 m_outPositionA = Vector3::Zero;
		Vector3 m_outPositionB = Vector3::Zero;
		Vector3 m_outVelocityA = Vector3::Zero;
		Vector3 m_outVelocityB = Vector3::Zero;

		CalcParticleColliAfterPos(
			&m_outHitPosition, &m_velocity,
			&m_hitBallPosition, &m_hitBallVelocity,
			m_weight, m_hitBallWeight,
			m_restitution, m_hitBallRestitution,
			elapsedTime - m_hitBallTime,
			&m_outPositionA, &m_outVelocityA,
			&m_outPositionB, &m_outVelocityB
		);
		m_outPositionA.y = m_pos.y;
		m_pos = m_outPositionA;
		m_velocity = m_outVelocityA;
	}

	if (m_hit) {
		if (m_hitImpulse) {
			float x = (m_hitImpulseNormal.x == 0) ? 1.f : -1.f;
			float y = (m_hitImpulseNormal.y == 0) ? 1.f : -1.f;
			float z = (m_hitImpulseNormal.z == 0) ? 1.f : -1.f;
			Vector3 impulseNormal = Vector3(x, y, z);
			m_velocity = m_velocity * impulseNormal * m_restitution;
			m_velocity = calcVel(m_velocity, m_hitNormal - m_hitImpulseNormal);
			m_hitImpulse = false;
			if(!m_hitBall)
				m_pos += m_velocity * elapsedTime;
		}
		else {
			m_velocity = calcVel(m_velocity, m_hitNormal);
			if (!m_hitBall)
				m_pos += m_velocity * elapsedTime;
		}
	}
	else
		m_pos += m_velocity * elapsedTime;

	if (sqrt(m_velocity.Dot(m_velocity)) < 0.1f) m_velocity = Vector3::Zero;
	m_preHitNormal = m_hitNormal;
	m_force = Vector3::Zero;
	m_time += elapsedTime;
	m_hitBall = false;
}

void Collider::SetPosition(const Vector3 &position) {
	m_pos = position;
}

Vector3 Collider::GetPosition() const {
	return m_pos;
}

Vector3 Collider::GetVelocity() const {
	return m_velocity;
}

float Collider::GetWeight() const {
	return m_weight;
}

float Collider::GetRestitution() const {
	return m_restitution;
}

void Collider::AddForce(Vector3 power) {
	m_force += power;
}

Vector3 calcVel(Vector3 velocity, Vector3 normal) {
	Vector3 calcVelocity = Vector3::Zero;
	if (!((normal.x > 0.f && velocity.x < 0.f) || (normal.x < 0.f && velocity.x > 0.f)))
		calcVelocity.x = velocity.x;
	if (!((normal.y > 0.f && velocity.y < 0.f) || (normal.y < 0.f && velocity.y > 0.f)))
		calcVelocity.y = velocity.y;
	if (!((normal.z > 0.f && velocity.z < 0.f) || (normal.z < 0.f && velocity.z > 0.f)))
		calcVelocity.z = velocity.z;
	return calcVelocity;
}

///////////////////////////////////////////////////
// ���ʃp�[�e�B�N���Փ˔���E�����E�ʒu�Z�o�֐�
// r : �p�[�e�B�N���̔��a
// pPre_pos : �p�[�e�B�N���̑O�̈ʒu
// pPos : �p�[�e�B�N���̎��̓��B�ʒu
// pNormal : ���ʂ̖@��
// pPlane_pos : ���ʏ��1�_
// pOut_t : �Փˎ��Ԃ��i�[����FLOAT�^�ւ̃|�C���^
// pOut_colli : �p�[�e�B�N���̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
// �߂�l : �Փ�(true), ��Փ�(false)
/* �Q�� http://marupeke296.com/COL_main.html */

#define EPSIRON 0.00001f // �덷

bool Collider::CalcParticlePlaneCollision(
	FLOAT r,
	Vector3 *pPre_pos, Vector3 *pPos,
	Vector3 *pNormal, Vector3 *pPlane_pos,
	FLOAT *t,
	Vector3 *pOut_colli
)
{
	Vector3 C0 = *pPre_pos - *pPlane_pos; // ���ʏ�̈�_���猻�݈ʒu�ւ̃x�N�g��
	Vector3 D = *pPos - *pPre_pos; // ���݈ʒu����\��ʒu�܂ł̃x�N�g��
	Vector3 N = *pNormal; // �@��
	N.Normalize(); // �@����W����

				   // ���ʂƒ��S�_�̋������Z�o
	FLOAT Dot_C0 = C0.Dot(N);
	FLOAT dist_plane_to_point = fabs(Dot_C0);

	// �i�s�����Ɩ@���̊֌W���`�F�b�N
	FLOAT Dot = D.Dot(N);

	// ���ʂƕ��s�Ɉړ����Ă߂荞��ł���X�y�V�����P�[�X
	if ((EPSIRON - fabs(Dot) > 0.0f) && (dist_plane_to_point < r)) {
		// �ꐶ�����o���Ȃ��̂ōő厞����Ԃ�
		*t = FLT_MAX;
		// �Փˈʒu�͎d���Ȃ��̂ō��̈ʒu���w��
		*pOut_colli = *pPre_pos;
		return true;
	}

	// �������Ԃ̎Z�o
	*t = (r - Dot_C0) / Dot;

	// �Փˈʒu�̎Z�o
	*pOut_colli = *pPre_pos + (*t) * D;

	// �߂荞��ł�����Փ˂Ƃ��ď����I��
	if (dist_plane_to_point < r)
		return true;

	// �ǂɑ΂��Ĉړ����t�����Ȃ�Փ˂��Ă��Ȃ�
	if (Dot >= 0)
		return false;

	// ���Ԃ�0�`1�̊Ԃɂ���ΏՓ�
	if ((0 <= *t) && (*t <= 1))
		return true;

	return false;
}

///////////////////////////////////////////////////
// �p�[�e�B�N���Փˌ㑬�x�ʒu�Z�o�֐�
//   pColliPos_A : �Փ˒��̃p�[�e�B�N��A�̒��S�ʒu
//   pVelo_A     : �Փ˂̏u�Ԃ̃p�[�e�B�N��A�̑��x
//   pColliPos_B : �Փ˒��̃p�[�e�B�N��B�̒��S�ʒu
//   pVelo_B     : �Փ˂̏u�Ԃ̃p�[�e�B�N��B�̑��x
//   weight_A    : �p�[�e�B�N��A�̎���
//   weight_B    : �p�[�e�B�N��B�̎���
//   res_A       : �p�[�e�B�N��A�̔�����
//   res_B       : �p�[�e�B�N��B�̔�����
//   time        : ���ˌ�̈ړ�����
//   pOut_pos_A  : �p�[�e�B�N��A�̔��ˌ�ʒu
//   pOut_velo_A : �p�[�e�B�N��A�̔��ˌ㑬�x�x�N�g��
//   pOut_pos_B  : �p�[�e�B�N��B�̔��ˌ�ʒu
//   pOut_velo_B : �p�[�e�B�N��B�̔��ˌ㑬�x�x�N�g��
/* �Q�� http://marupeke296.com/COL_main.html */

bool Collider::CalcParticleColliAfterPos(
	Vector3 *pColliPos_A, Vector3 *pVelo_A,
	Vector3 *pColliPos_B, Vector3 *pVelo_B,
	FLOAT weight_A, FLOAT weight_B,
	FLOAT res_A, FLOAT res_B,
	FLOAT time,
	Vector3 *pOut_pos_A, Vector3 *pOut_velo_A,
	Vector3 *pOut_pos_B, Vector3 *pOut_velo_B
)
{
	Vector3 velocityA = *pVelo_A;
	Vector3 velocityB = *pVelo_B;
	velocityA.y = 0.f;
	velocityB.y = 0.f;

	FLOAT TotalWeight = weight_A + weight_B; // ���ʂ̍��v
	FLOAT RefRate = (1 + res_A*res_B); // ������
	Vector3 C = *pColliPos_B - *pColliPos_A; // �Փˎ��x�N�g��
	C.Normalize();
	FLOAT Dot = C.Dot((velocityA - velocityB)); // ���ώZ�o
	Vector3 ConstVec = RefRate*Dot / TotalWeight * C; // �萔�x�N�g��

													  // �Փˌ㑬�x�x�N�g���̎Z�o
	*pOut_velo_A = -weight_B * ConstVec + velocityA - C;	// +C�͂߂荞�񂾂Ƃ��p
	*pOut_velo_B = weight_A * ConstVec + velocityB;

	// �Փˌ�ʒu�̎Z�o
	*pOut_pos_A = *pColliPos_A + time * (*pOut_velo_A);
	*pOut_pos_B = *pColliPos_B + time * (*pOut_velo_B);

	return true;
}