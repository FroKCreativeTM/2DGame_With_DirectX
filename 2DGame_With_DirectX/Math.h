#pragma once

#include <d3dx9.h>

#define VECTOR2 D3DXVECTOR2

// �浹 ��� ���� ���� ���� �Լ����� �־�� ���ӽ����̽��Դϴ�.
namespace NSMath {
	inline float Vector2Length(const VECTOR2 * v) {
		return D3DXVec2Length(v);
	}

	inline float Vector2Dot(const VECTOR2* v1, const VECTOR2* v2) {
		return D3DXVec2Dot(v1, v2);
	}

	inline void Vector2Normalize(VECTOR2 * v) {
		D3DXVec2Normalize(v, v);
	}
}