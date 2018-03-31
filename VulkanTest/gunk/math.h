#pragma once

#include <inttypes.h>

typedef float float32_t;
#define GUNK_INLINE __forceinline

namespace gunk
{

const float32_t PI = 3.141592653589793f;

GUNK_INLINE float32_t RadToDeg(float32_t rad)
{
    return rad * 180.0f / PI;
}
GUNK_INLINE float32_t DegToRad(float32_t deg)
{
    return deg * PI / 180.0f;
}

struct Quaternion
{
    void Set(float32_t _x, float32_t _y, float32_t _z, float32_t _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    float32_t x, y, z, w;
};

const Quaternion kQuatIdentity = { 0.0f, 0.0f, 0.0f, 1.0f };

struct Vector2
{
    Vector2(float32_t _x = 0.0f, float32_t _y = 0.0f) :
        x(_x), y(_y)
    {}
    void Set(float32_t _x, float32_t _y)
    {
        x = _x;
        y = _y;
    }
    void Set(float32_t _x)
    {
        x = _x;
        y = _x;
    }
    float32_t x;
    float32_t y;
};

const Vector2 kVec2Zero= { 0.0f, 0.0f };

struct Vector3
{
    void Set(float32_t _x, float32_t _y, float32_t _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    union
    {
        struct
        {
            float32_t x;
            float32_t y;
            float32_t z;
        };
    };
};
const Vector3 kVec3Zero = { 0.0f, 0.0f, 0.0f };

struct Matrix2D
{
    union
    {
        struct
        {
            float32_t a, b, c;
            float32_t d, tx, ty;
        };
        float32_t data[6];
    };
};

const Matrix2D kMat2DIdentity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

struct Matrix4
{
    void Set(
        float32_t a = 1.0f, float32_t b = 0.0f, float32_t c = 0.0f, float32_t d = 0.0f,
        float32_t e = 0.0f, float32_t f = 1.0f, float32_t g = 0.0f, float32_t h = 0.0f,
        float32_t i = 0.0f, float32_t j = 0.0f, float32_t k = 1.0f, float32_t l = 0.0f,
        float32_t m = 0.0f, float32_t n = 0.0f, float32_t o = 0.0f, float32_t p = 1.0f
        )
    {
        data[0] = a;
        data[1] = b;
        data[2] = c;
        data[3] = d;
        data[4] = e;
        data[5] = f;
        data[6] = g;
        data[7] = h;
        data[8] = i;
        data[9] = j;
        data[10] = k;
        data[11] = l;
        data[12] = m;
        data[13] = n;
        data[14] = o;
        data[15] = p;
    }
    union
    {
        struct
        {
            float32_t a, b, c, d;
            float32_t e, f, g, h;
            float32_t i, j, k, l;
            float32_t m, n, o, p;
        };
        float32_t data[16];
    };
};

const Matrix4 kMat4Identity = { 
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

namespace math
{
/* mat2d */
Matrix2D* Identity(Matrix2D* pOut);
Matrix2D* Mul(Matrix2D* pOut, Matrix2D* pM0, Matrix2D* pM1);
Vector2* Mul(Vector2* pOut, Matrix2D* pM0, Vector2* pV1);
Matrix2D* Translate(Matrix2D* pOut, const Matrix2D* pM, float32_t x, float32_t y);
Matrix2D* Scale(Matrix2D* pOut, Matrix2D* pM, float32_t x, float32_t y);
Matrix2D* Rotate(Matrix2D* pOut, Matrix2D* pM, float32_t radian);
Matrix2D* Invert(Matrix2D* pOut, Matrix2D* pM);

/* mat4 */
Matrix4* Identity(Matrix4* pOut);
Matrix4* Mul(Matrix4* pOut, Matrix4* pM0, Matrix4* pM1);
Vector3* Mul(Vector3* pOut, Matrix4* pM0, Vector3* pV1);
Matrix4* Transpose(Matrix4* pOut, Matrix4* pM);
Matrix4* Invert(Matrix4* pOut, Matrix4* pM);
Matrix4* RotateX(Matrix4* pOut, Matrix4* pM, float32_t radian);
Matrix4* RotateY(Matrix4* pOut, Matrix4* pM, float32_t radian);
Matrix4* RotateZ(Matrix4* pOut, Matrix4* pM, float32_t radian);
Matrix4* Rotate(Matrix4* pOut, Matrix4* pM, Vector3* pAxis, float32_t radian);
Matrix4* Scale(Matrix4* pOut, Matrix4* pM, Vector3* pScale);
Matrix4* Translate(Matrix4* pOut, Matrix4* pM, Vector3* pTranslate);
Matrix4* Frustum(Matrix4* pOut, float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t frustumNear, float32_t frustumFar);
Matrix4* Perspective(Matrix4* pOut, float32_t fov_y, float32_t aspect, float32_t perspNear, float32_t perspFar);
Matrix4* Orthographic(Matrix4* pOut, float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t orthoNear, float32_t orthoFar);
Matrix4* LookAt(Matrix4* pOut, Vector3* pEye, Vector3* pCenter, Vector3* pUp);

/* vec2 */
Vector2* Add(Vector2* pOut, Vector2* pV0, Vector2* pV1);
Vector2* Sub(Vector2* pOut, Vector2* pV0, Vector2* pV1);
Vector2* Mul(Vector2* pOut, Vector2* pV0, Vector2* pV1);
Vector2* Invert(Vector2* pOut, Vector2* pV);
Vector2* Negate(Vector2* pOut, Vector2* pV);
Vector2* Normalize(Vector2* pOut, Vector2* pV);
int32_t Equal(Vector2* pV0, Vector2* pV1);
Vector3* CrossProduct(Vector3* pOut, Vector2* pV0, Vector2* pV1);
float32_t Distance(Vector2* pV0, Vector2* pV1);
float32_t DotProduct(Vector2* pV0, Vector2* pV1);
float32_t Length(Vector2* pV);
float32_t Length2(Vector2* pV);
float32_t Distance2(Vector2* pV0, Vector2* pV1);

/* vec3 */
Vector3* Add(Vector3* pOut, Vector3* pV0, Vector3* pV1);
Vector3* Sub(Vector3* pOut, Vector3* pV0, Vector3* pV1);
Vector3* Mul(Vector3* pOut, Vector3* pV0, Vector3* pV1);
Vector3* Invert(Vector3* pOut, Vector3* pV);
Vector3* Negate(Vector3* pOut, Vector3* pV);
Vector3* Normalize(Vector3* pOut, Vector3* pV);
int32_t Equal(Vector3* pV0, Vector3* pV1);
Vector3* CrossProduct(Vector3* pOut, Vector3* pV0, Vector3* pV1);
float32_t Distance(Vector3* pV0, Vector3* pV1);
float32_t DotProduct(Vector3* pV0, Vector3* pV1);
float32_t Length(Vector3* pV);
float32_t Length2(Vector3* pV);
float32_t Distance2(Vector3* pV0, Vector3* pV1);

/* quat */
Quaternion* Identity(Quaternion* pOut);
Quaternion* Add(Quaternion* pOut, Quaternion* pQ0, Quaternion* pQ1);
Quaternion* Mul(Quaternion* pOut, Quaternion* pQ0, Quaternion* pQ1);
Quaternion* SetAxisAngle(Quaternion* pOut, Vector3* pAxis, float32_t radian);
Quaternion* RotateX(Quaternion* pOut, Quaternion* pQ, float32_t radian);
Quaternion* RotateY(Quaternion* pOut, Quaternion* pQ, float32_t radian);
Quaternion* RotateZ(Quaternion* pOut, Quaternion* pQ, float32_t radian);
Quaternion* CalculateW(Quaternion* pOut, Quaternion* pQ);
Quaternion* Invert(Quaternion* pOut, Quaternion* pQ);
Quaternion* Conjugate(Quaternion* pOut, Quaternion* pQ);
Matrix4* QuaternionToMatrix4(Matrix4* pOut, Quaternion* pQ);
float32_t GetAxisAngle(Vector3* pOutAxis, Quaternion* pQ);
}
}
