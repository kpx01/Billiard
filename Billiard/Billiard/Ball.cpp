#include "pch.h"
#include "Ball.h"

using namespace DirectX;
using namespace SimpleMath;

using Microsoft::WRL::ComPtr;

Ball::Ball(const int &id, const Vector3 &pos, const Vector3 &velocity, const float &weight, const float &scale, const bool& freeze) :
	Collider(pos, velocity, weight, scale, freeze),
	m_id(id),
	m_radius(1.f)
{
}

void Ball::Initialize() {
	m_bFallBall = false;
	m_vFallHoll = Vector3::Zero;
}

void Ball::Initialize(const Vector3 &pos) {
	SetPosition(pos);
	m_velocity = Vector3::Zero;
	m_bFallBall = false;
	m_vFallHoll = Vector3::Zero;
}

void Ball::Update(float elapsedTime)
{
	base::Update(elapsedTime);
}

void Ball::Render(GeometricPrimitive *m_Shape, XMMATRIX& m_World, XMMATRIX& m_View, XMMATRIX& m_Projection) {
	XMVECTOR qid = XMQuaternionIdentity();
	const XMVECTORF32 scale = { m_radius, m_radius, m_radius };
	const XMVECTORF32 translate = { m_pos.x, m_pos.y, m_pos.z };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(-m_velocity.z * 0.5f * m_time, 0, -m_velocity.x * 0.5f * m_time);//(XM_PI / 2.f) * m_Time
	XMMATRIX local = XMMatrixMultiply(m_World, XMMatrixTransformation(g_XMZero, qid, scale, g_XMZero, rotate, translate));
	XMVECTOR color = Colors::White;
	if (m_id == 0) color = Colors::White;
	else if (m_id == 1) color = Colors::GreenYellow;
	else if (m_id == 2) color = Colors::Blue;
	else if (m_id == 3) color = Colors::Red;
	else if (m_id == 4) color = Colors::Purple;
	else if (m_id == 5) color = Colors::Orange;
	else if (m_id == 6) color = Colors::Green;
	else if (m_id == 7) color = Colors::Brown;
	else if (m_id == 8) color = Colors::Black;
	else if (m_id == 9) color = Colors::Yellow;
	else color = Colors::White;
	m_Shape->Draw(local, m_View, m_Projection, color, nullptr);
}

void Ball::Clear() {

}

float Ball::GetRadius() const {
	return m_radius;
}

bool Ball::GetFallBall() const {
	return m_bFallBall;
}

// �{�[���ƃe�[�u���̏Փ˔���
void Ball::CollisionTable(Table &table) {
	m_hitNormal = Vector3::Zero;
	m_hitImpulseNormal = Vector3::Zero;
	bool hit = false;

	table.CalcCollisionTable(m_radius, &m_prepos, &m_pos, &m_preHitNormal, &m_hitNormal, &m_hitImpulseNormal, &hit,  &m_hitImpulse, &m_hitTime, &m_hitPoint, &m_bFallBall, &m_vFallHoll);

	m_hit = hit;
}

// �{�[���Ԃ̏Փ˔���
void Ball::CollisionBall(Ball &ball) {
	float stime = 0.01f;
	float hittime = 1.f;
	Vector3 outColpos;
	Vector3 outColpos1;
	Vector3 outColpos2;

	if (CalcIntervalSphereSphere(stime, *this, ball, hittime, outColpos, &outColpos1, &outColpos2)) {
		if (m_hitBallTime >= hittime) {
			m_hitBallTime = hittime;
			m_hitBall = true;
			m_outHitPosition = outColpos1;
			m_hitBallPosition = ball.GetPosition();
			m_hitBallVelocity = ball.GetVelocity();
			m_hitBallWeight = ball.GetWeight();
			m_hitBallRestitution = ball.GetRestitution();
		}
	}
}

// 2�̋��̏Փ˂܂ł̎��Ԃƈʒu���擾
// �o���̋��͎w��̃x�N�g�������ɓ����Ői�ނƉ���
// stepSec : �o���̃p�[�e�B�N�����i�ގ���
// s1 : �p�[�e�B�N��A
// s2 : �p�[�e�B�N��B
// outSec : �Փˎ���
// outColPos : �Փˈʒu�i�ړ_�j
// outColPosS1(option) : �Փˎ��̃p�[�e�B�N��A�̒��S���W
// outColPosS2(option) : �Փˎ��̃p�[�e�B�N��B�̒��S���W
/* �Q�� http://marupeke296.com/COL_main.html */

bool Ball::CalcIntervalSphereSphere(
	float stepSec,
	const Ball &s1,
	const Ball &s2,
	float &outSec,
	Vector3 &outColPos,
	Vector3 *outColPosS1 = 0,
	Vector3 *outColPosS2 = 0
) {
	Vector3 s1Position = s1.GetPosition();
	Vector3 s1Velocity = s1.GetVelocity();
	s1Velocity.y = 0;
	Vector3 s2Position = s2.GetPosition();
	Vector3 s2Velocity = s2.GetVelocity();
	s2Velocity.y = 0;

	// �O�ʒu�y�ѓ��B�ʒu�ɂ�����p�[�e�B�N���Ԃ̃x�N�g�����Z�o
	Vector3 C0 = s2.GetPosition() - s1.GetPosition();
	Vector3 A1 = s1.GetPosition() + s1Velocity * stepSec;
	Vector3 B1 = s2.GetPosition() + s2.GetPosition() * stepSec;
	Vector3 C1 = B1 - A1;
	Vector3 D = C1 - C0;
	float rAB = s1.GetRadius() + s2.GetRadius();
	float rABSq = rAB * rAB;
	float P = D.LengthSquared();

	// �Փ˔���ɉ��̌������g���邩�H
	if (P == 0) {
		// ���s�ړ� //
		// t = 0�ŏՓ˂��Ă��邩�H
		if ((s2.GetPosition() - s1.GetPosition()).LengthSquared() > rABSq) {
			return false;
		}

		outSec = 0.0f;
		if (outColPosS1 != 0)
			*outColPosS1 = s1.GetPosition();
		if (outColPosS2 != 0)
			*outColPosS2 = s2.GetPosition();

		if (s2.GetPosition() == s1.GetPosition()) {
			// ���S�_���ꏏ�Ȃ̂Œ��S�_���Փ˓_�Ƃ��ĕԂ�
			outColPos = s1.GetPosition();
			return true;
		}

		// ��A->B�̃x�N�g�������ɒ���rA�̏����Փ˓_�Ƃ���
		outColPos = s1.GetPosition() + (s1.GetRadius() / rAB) * C0;
		return true; // ���������Ɉړ�
	}

	// �Փˌ��m�\ //

	// �ŏ�����Փ˂��Ă���H
	if ((s2.GetPosition() - s1.GetPosition()).LengthSquared() <= rABSq) {
		outSec = 0.0f;
		outColPos = s1.GetPosition() + s1.GetRadius() / rAB * C0;
		if (outColPosS1 != 0)
			*outColPosS1 = s1.GetPosition();
		if (outColPosS2 != 0)
			*outColPosS2 = s2.GetPosition();
		return true;
	}

	float Q = C0.Dot(D);
	float R = C0.LengthSquared();

	// �Փ˔��莮
	float judge = Q * Q - P * (R - rAB * rAB);
	if (judge < 0) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}

	// �Փˎ��Ԃ̎Z�o
	float judge_rt = sqrtf(judge);
	float t_plus = (-Q + judge_rt) / P;
	float t_minus = (-Q - judge_rt) / P;
	if (t_minus > t_plus) {
		// t_minus������������
		float tmp = t_minus;
		t_minus = t_plus;
		t_plus = tmp;
	}

	// ���ԊO�Փ˂��H
	if (t_minus < 0.0f || t_minus > 1.0f) {
		return false;
	}

	// �Փˈʒu�̌���
	outSec = t_minus * stepSec;
	Vector3 Atc = s1.GetPosition() + s1Velocity * stepSec * t_minus;
	Vector3 Btc = s2.GetPosition() + s2Velocity * stepSec * t_minus;
	outColPos = Atc + s1.GetRadius() / rAB * (Btc - Atc);

	if (outColPosS1 != 0)
		*outColPosS1 = Atc;
	if (outColPosS2 != 0)
		*outColPosS2 = Btc;

	return true; // �Փ�
}
