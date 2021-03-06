#pragma once

#include "cvector4d.h"
#include "cquaternion.h"
#include <memory>
#include <string>

class CMatrix4x4 {
public:
	const static int row_size = 4;
	const static int col_size = 4;

	CMatrix4x4() : scaled(1.0f) { set_to_identity(); }
	CMatrix4x4(const float* d) : scaled(1.0f) { memcpy(m, d, sizeof(m)); }
	CMatrix4x4(const float* d, int cols, int rows);
	inline CMatrix4x4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	inline CVector4D column(int index) const;
	inline CMatrix4x4& set_column(int index, const CVector4D& value);
	inline CVector4D row(int index) const;
	inline CMatrix4x4& set_row(int index, const CVector4D& value);

	inline float& operator()(int row, int col);						// return m[column][row]
	inline const float& operator()(int row, int col) const;

	CMatrix4x4 operator * (float b) const;
	CMatrix4x4 operator * (const CMatrix4x4& b) const;
	friend CMatrix4x4 operator * (float a, const CMatrix4x4& b);
	friend CVector4D operator * (const CMatrix4x4& a, const CVector4D& b);
	friend CVector3D operator * (const CMatrix4x4& a, const CVector3D& b);

	friend std::ostream& operator << (std::ostream& out, const CMatrix4x4& b);
	std::string to_string() const;

	// transform
	CMatrix4x4& translate(const CVector3D& v);
	CMatrix4x4& translate(float x, float y, float z = 0.0f);

	CMatrix4x4& scale(const CVector3D& v);
	CMatrix4x4& scale(float x, float y, float z = 1.0f);
	CMatrix4x4& scale(float f);

	CMatrix4x4& rotate(float angle, const CVector3D& v);					
	CMatrix4x4& rotate(float angle, float x, float y, float z = 0.0f);
	CMatrix4x4& rotate_euler(const CVector3D& euler_angle);								// z_y_x -- 以欧拉角旋转
	CMatrix4x4& rotate_euler(float x, float y, float z = 0.0f);
	CMatrix4x4& rotate_quaternion(const CQuaternion& quaternion);						// 以 四元数 旋转
	CVector3D	get_rotate_euler() const;												// 由旋转矩阵得 欧拉角
	CVector4D	get_rotate_angle_axis() const;											// 由旋转矩阵得 轴角
	CQuaternion get_rotate_quaternion() const;											// 由旋转矩阵得 四元数
	// look at
	CMatrix4x4& lookAt(const CVector3D& eye, const CVector3D& center, const CVector3D& world_up);
	CMatrix4x4& lookAt_left(const CVector3D& eye, const CVector3D& center, const CVector3D& world_up);

	// projection -- 右手系 and z belong [-1 ,, 1] 
	CMatrix4x4& ortho(float width, float aspect_ratio, float near, float far);						// aspect_ratio -- width / height
	CMatrix4x4& ortho(float left, float right, float bottom, float top, float near, float far);	
	CMatrix4x4& ortho_2d(float width, float aspect_ratio);	
	CMatrix4x4& ortho_2d(float left, float right, float bottom, float top);							// near = -1 and far = 1
	CMatrix4x4& frustum(float left, float right, float bottom, float top, float near, float far);	// 平截头体
	CMatrix4x4& perspective(float vertical_angle, float aspect_ratio, float near, float far);		// 透视 -- 投影平面为x轴和z轴中心

	CMatrix4x4& set_to_identity();									// 单位矩阵
	CMatrix4x4& set_to_zero();										// 0矩阵
	CMatrix4x4& set_to_transpose();									// 转置矩阵

	float det() const;												// 行列式的值
	float cofactor(int row, int col) const;							// 代数余子式

	CMatrix4x4 adj() const;											// 伴随矩阵
	CMatrix4x4 inverse() const;										// 逆矩阵
	CMatrix4x4 transpose() const;									// 转置矩阵			

	const float* data() const { return *m; }
	static const uint data_size() { return col_size * row_size * sizeof(float); }

private:
	float m[col_size][row_size];									// 列主序 -- m[i][j] -- 第 i 列 第 j 行

	CVector3D scaled;												// 已经缩放的因子

};

inline CMatrix4x4::CMatrix4x4 (
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44
) : scaled(1.0f) {
	m[0][0] = m11; m[0][1] = m21; m[0][2] = m31; m[0][3] = m41;
    m[1][0] = m12; m[1][1] = m22; m[1][2] = m32; m[1][3] = m42;
    m[2][0] = m13; m[2][1] = m23; m[2][2] = m33; m[2][3] = m43;
    m[3][0] = m14; m[3][1] = m24; m[3][2] = m34; m[3][3] = m44;
}

inline CVector4D CMatrix4x4::column(int index) const {
	return CVector4D(m[index][0], m[index][1], m[index][2], m[index][3]); 
}
inline CMatrix4x4& CMatrix4x4::set_column(int index, const CVector4D& value) {
	m[index][0] = value[0]; m[index][1] = value[1]; m[index][2] = value[2]; m[index][3] = value[3]; 
	return (*this);
}
inline CVector4D CMatrix4x4::row(int index) const {
	return CVector4D(m[0][index], m[1][index], m[2][index], m[3][index]);
}
inline CMatrix4x4& CMatrix4x4::set_row(int index, const CVector4D& value) {
	m[0][index] = value[0]; m[1][index] = value[1]; m[2][index] = value[2]; m[3][index] = value[3];
	return (*this);
}

inline float& CMatrix4x4::operator()(int row, int col) {
	if (row < 0) row = 0; if (row >= row_size) row = row_size - 1;
	if (col < 0) col = 0; if (col >= col_size) col = col_size - 1; 
	return m[col][row]; 
}
inline const float& CMatrix4x4::operator()(int row, int col) const {
	if (row < 0) row = 0; if (row >= row_size) row = row_size - 1; 
	if (col < 0) col = 0; if (col >= col_size) col = col_size - 1;
	return m[col][row]; 
}
