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
// 平面パーティクル衝突判定・時刻・位置算出関数
// r : パーティクルの半径
// pPre_pos : パーティクルの前の位置
// pPos : パーティクルの次の到達位置
// pNormal : 平面の法線
// pPlane_pos : 平面上の1点
// pOut_t : 衝突時間を格納するFLOAT型へのポインタ
// pOut_colli : パーティクルの衝突位置を格納するD3DXVECTOR型へのポインタ
// 戻り値 : 衝突(true), 非衝突(false)
/* 参照 http://marupeke296.com/COL_main.html */

#define EPSIRON 0.00001f // 誤差

bool Collider::CalcParticlePlaneCollision(
	FLOAT r,
	Vector3 *pPre_pos, Vector3 *pPos,
	Vector3 *pNormal, Vector3 *pPlane_pos,
	FLOAT *t,
	Vector3 *pOut_colli
)
{
	Vector3 C0 = *pPre_pos - *pPlane_pos; // 平面上の一点から現在位置へのベクトル
	Vector3 D = *pPos - *pPre_pos; // 現在位置から予定位置までのベクトル
	Vector3 N = *pNormal; // 法線
	N.Normalize(); // 法線を標準化

				   // 平面と中心点の距離を算出
	FLOAT Dot_C0 = C0.Dot(N);
	FLOAT dist_plane_to_point = fabs(Dot_C0);

	// 進行方向と法線の関係をチェック
	FLOAT Dot = D.Dot(N);

	// 平面と平行に移動してめり込んでいるスペシャルケース
	if ((EPSIRON - fabs(Dot) > 0.0f) && (dist_plane_to_point < r)) {
		// 一生抜け出せないので最大時刻を返す
		*t = FLT_MAX;
		// 衝突位置は仕方ないので今の位置を指定
		*pOut_colli = *pPre_pos;
		return true;
	}

	// 交差時間の算出
	*t = (r - Dot_C0) / Dot;

	// 衝突位置の算出
	*pOut_colli = *pPre_pos + (*t) * D;

	// めり込んでいたら衝突として処理終了
	if (dist_plane_to_point < r)
		return true;

	// 壁に対して移動が逆向きなら衝突していない
	if (Dot >= 0)
		return false;

	// 時間が0～1の間にあれば衝突
	if ((0 <= *t) && (*t <= 1))
		return true;

	return false;
}

///////////////////////////////////////////////////
// パーティクル衝突後速度位置算出関数
//   pColliPos_A : 衝突中のパーティクルAの中心位置
//   pVelo_A     : 衝突の瞬間のパーティクルAの速度
//   pColliPos_B : 衝突中のパーティクルBの中心位置
//   pVelo_B     : 衝突の瞬間のパーティクルBの速度
//   weight_A    : パーティクルAの質量
//   weight_B    : パーティクルBの質量
//   res_A       : パーティクルAの反発率
//   res_B       : パーティクルBの反発率
//   time        : 反射後の移動時間
//   pOut_pos_A  : パーティクルAの反射後位置
//   pOut_velo_A : パーティクルAの反射後速度ベクトル
//   pOut_pos_B  : パーティクルBの反射後位置
//   pOut_velo_B : パーティクルBの反射後速度ベクトル
/* 参照 http://marupeke296.com/COL_main.html */

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

	FLOAT TotalWeight = weight_A + weight_B; // 質量の合計
	FLOAT RefRate = (1 + res_A*res_B); // 反発率
	Vector3 C = *pColliPos_B - *pColliPos_A; // 衝突軸ベクトル
	C.Normalize();
	FLOAT Dot = C.Dot((velocityA - velocityB)); // 内積算出
	Vector3 ConstVec = RefRate*Dot / TotalWeight * C; // 定数ベクトル

													  // 衝突後速度ベクトルの算出
	*pOut_velo_A = -weight_B * ConstVec + velocityA - C;	// +Cはめり込んだとき用
	*pOut_velo_B = weight_A * ConstVec + velocityB;

	// 衝突後位置の算出
	*pOut_pos_A = *pColliPos_A + time * (*pOut_velo_A);
	*pOut_pos_B = *pColliPos_B + time * (*pOut_velo_B);

	return true;
}