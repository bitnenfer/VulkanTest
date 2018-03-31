#include "math.h"
#include <math.h>

/* mat2d */
gunk::Matrix2D* gunk::math::Identity(gunk::Matrix2D* pOut)
{
    pOut->data[0] = 1.0f;
    pOut->data[1] = 0.0f;
    pOut->data[2] = 0.0f;
    pOut->data[3] = 1.0f;
    pOut->data[4] = 0.0f;
    pOut->data[5] = 0.0f;
    return pOut;
}
gunk::Matrix2D* gunk::math::Mul(gunk::Matrix2D* pOut, gunk::Matrix2D* pM0, gunk::Matrix2D* pM1)
{
    const float32_t* matrixA = pM0->data;
    const float32_t* matrixB = pM1->data;
    float32_t* matrix = pOut->data;

    float32_t a = matrixA[0];
    float32_t b = matrixA[1];
    float32_t c = matrixA[2];
    float32_t d = matrixA[3];
    float32_t tx = matrixA[4];
    float32_t ty = matrixA[5];

    float32_t a0 = matrixB[0];
    float32_t b0 = matrixB[1];
    float32_t c0 = matrixB[2];
    float32_t d0 = matrixB[3];
    float32_t tx0 = matrixB[4];
    float32_t ty0 = matrixB[5];

    matrix[0] = a * a0 + b * c0;
    matrix[1] = a * b0 + b * d0;
    matrix[2] = c * a0 + d * c0;
    matrix[3] = c * b0 + d * d0;
    matrix[4] = tx * a0 + ty * c0 + tx0;
    matrix[5] = tx * b0 + ty * d0 + ty0;

    return pOut;
}
gunk::Vector2* gunk::math::Mul(gunk::Vector2* pOut, gunk::Matrix2D* pM0, gunk::Vector2* pV1)
{
    pOut->x = pV1->x * pM0->a + pV1->y * pM0->c + pM0->tx;
    pOut->y = pV1->x * pM0->b + pV1->y * pM0->d + pM0->ty;
    return pOut;
}
gunk::Matrix2D* gunk::math::Translate(gunk::Matrix2D* pOut, const gunk::Matrix2D* pM, float32_t x, float32_t y)
{
    pOut->data[4] = pM->data[0] * x + pM->data[2] * y + pM->data[4];
    pOut->data[5] = pM->data[1] * x + pM->data[3] * y + pM->data[5];
    return pOut;
}
gunk::Matrix2D* gunk::math::Scale(gunk::Matrix2D* pOut, gunk::Matrix2D* pM, float32_t x, float32_t y)
{
    pOut->data[0] = pM->data[0] * x;
    pOut->data[1] = pM->data[1] * x;
    pOut->data[2] = pM->data[2] * y;
    pOut->data[3] = pM->data[3] * y;
    return pOut;
}
gunk::Matrix2D* gunk::math::Rotate(gunk::Matrix2D* pOut, gunk::Matrix2D* pM, float32_t radian)
{
    float32_t s = sinf(radian);
    float32_t c = cosf(radian);
    float32_t m0 = pM->data[0];
    float32_t m1 = pM->data[1];
    float32_t m2 = pM->data[2];
    float32_t m3 = pM->data[3];

    pOut->data[0] = m0 * c + m2 * s;
    pOut->data[1] = m1 * c + m3 * s;
    pOut->data[2] = m0 * -s + m2 * c;
    pOut->data[3] = m1 * -s + m3 * c;
    return pOut;
}

gunk::Matrix2D* gunk::math::Invert(Matrix2D* pOut, Matrix2D* pM)
{
    float32_t aa = pM->data[0], ab = pM->data[1], ac = pM->data[2], ad = pM->data[3];
    float32_t atx = pM->data[4], aty = pM->data[5];
    float32_t det = aa * ad - ab * ac;

    if (!det)
    {
        return NULL;
    }

    det = 1.0f / det;

    pOut->data[0] = ad * det;
    pOut->data[1] = -ab * det;
    pOut->data[2] = -ac * det;
    pOut->data[3] = aa * det;
    pOut->data[4] = (ac * aty - ad * atx) * det;
    pOut->data[5] = (ab * atx - aa * aty) * det;

    return pOut;
}

/* mat4 */
gunk::Matrix4* gunk::math::Identity(gunk::Matrix4* pOut)
{
    pOut->data[0] = 1.0f;
    pOut->data[1] = 0.0f;
    pOut->data[2] = 0.0f;
    pOut->data[3] = 0.0f;
    pOut->data[4] = 0.0f;
    pOut->data[5] = 1.0f;
    pOut->data[6] = 0.0f;
    pOut->data[7] = 0.0f;
    pOut->data[8] = 0.0f;
    pOut->data[9] = 0.0f;
    pOut->data[10] = 1.0f;
    pOut->data[11] = 0.0f;
    pOut->data[12] = 0.0f;
    pOut->data[13] = 0.0f;
    pOut->data[14] = 0.0f;
    pOut->data[15] = 1.0f;
    return pOut;
}
gunk::Matrix4* gunk::math::Mul(gunk::Matrix4* pOut, gunk::Matrix4* pM0, gunk::Matrix4* pM1)
{
    float32_t 
        a00 = pM0->data[0], a01 = pM0->data[1], a02 = pM0->data[2], a03 = pM0->data[3],
        a04 = pM0->data[4], a05 = pM0->data[5], a06 = pM0->data[6], a07 = pM0->data[7],
        a08 = pM0->data[8], a09 = pM0->data[9], a10 = pM0->data[10], a11 = pM0->data[11],
        a12 = pM0->data[12], a13 = pM0->data[13], a14 = pM0->data[14], a15 = pM0->data[15];

    float32_t
        b00 = pM1->data[0], b01 = pM1->data[1], b02 = pM1->data[2], b03 = pM1->data[3],
        b04 = pM1->data[4], b05 = pM1->data[5], b06 = pM1->data[6], b07 = pM1->data[7],
        b08 = pM1->data[8], b09 = pM1->data[9], b10 = pM1->data[10], b11 = pM1->data[11],
        b12 = pM1->data[12], b13 = pM1->data[13], b14 = pM1->data[14], b15 = pM1->data[15];

    float32_t* pMatrixData = pOut->data;

    pMatrixData[0x0] = b00 * a00 + b01 * a04 + b02 * a08 + b03 * a12;
    pMatrixData[0x1] = b00 * a01 + b01 * a05 + b02 * a09 + b03 * a13;
    pMatrixData[0x2] = b00 * a02 + b01 * a06 + b02 * a10 + b03 * a14;
    pMatrixData[0x3] = b00 * a03 + b01 * a07 + b02 * a11 + b03 * a15;
    pMatrixData[0x4] = b04 * a00 + b05 * a04 + b06 * a08 + b07 * a12;
    pMatrixData[0x5] = b04 * a01 + b05 * a05 + b06 * a09 + b07 * a13;
    pMatrixData[0x6] = b04 * a02 + b05 * a06 + b06 * a10 + b07 * a14;
    pMatrixData[0x7] = b04 * a03 + b05 * a07 + b06 * a11 + b07 * a15;
    pMatrixData[0x8] = b08 * a00 + b09 * a04 + b10 * a08 + b11 * a12;
    pMatrixData[0x9] = b08 * a01 + b09 * a05 + b10 * a09 + b11 * a13;
    pMatrixData[0xA] = b08 * a02 + b09 * a06 + b10 * a10 + b11 * a14;
    pMatrixData[0xB] = b08 * a03 + b09 * a07 + b10 * a11 + b11 * a15;
    pMatrixData[0xC] = b12 * a00 + b13 * a04 + b14 * a08 + b15 * a12;
    pMatrixData[0xD] = b12 * a01 + b13 * a05 + b14 * a09 + b15 * a13;
    pMatrixData[0xE] = b12 * a02 + b13 * a06 + b14 * a10 + b15 * a14;
    pMatrixData[0xF] = b12 * a03 + b13 * a07 + b14 * a11 + b15 * a15;
    
    return pOut;
}
gunk::Vector3* gunk::math::Mul(gunk::Vector3* pOut, gunk::Matrix4* pM0, gunk::Vector3* pV1)
{
    pOut->x = pM0->data[0] * pV1->x + pM0->data[4] * pV1->y + pM0->data[8] * pV1->z + pM0->data[12];
    pOut->y = pM0->data[1] * pV1->x + pM0->data[5] * pV1->y + pM0->data[9] * pV1->z + pM0->data[13];
    pOut->z = pM0->data[2] * pV1->x + pM0->data[6] * pV1->y + pM0->data[10] * pV1->z + pM0->data[14];
    return pOut;
}
gunk::Matrix4* gunk::math::Transpose(gunk::Matrix4* pOut, gunk::Matrix4* pM)
{
    pOut->data[0] = pM->data[0];
    pOut->data[1] = pM->data[4];
    pOut->data[2] = pM->data[8];
    pOut->data[3] = pM->data[12];
    pOut->data[4] = pM->data[1];
    pOut->data[5] = pM->data[5];
    pOut->data[6] = pM->data[9];
    pOut->data[7] = pM->data[13];
    pOut->data[8] = pM->data[2];
    pOut->data[9] = pM->data[6];
    pOut->data[10] = pM->data[10];
    pOut->data[11] = pM->data[14];
    pOut->data[12] = pM->data[3];
    pOut->data[13] = pM->data[7];
    pOut->data[14] = pM->data[11];
    pOut->data[15] = pM->data[15];
    return pOut;
}
gunk::Matrix4* gunk::math::Invert(gunk::Matrix4* pOut, gunk::Matrix4* pM)
{
    float32_t d0 = pM->data[0] * pM->data[5] - pM->data[1] * pM->data[4];
    float32_t d1 = pM->data[0] * pM->data[6] - pM->data[2] * pM->data[4];
    float32_t d2 = pM->data[0] * pM->data[7] - pM->data[3] * pM->data[4];
    float32_t d3 = pM->data[1] * pM->data[6] - pM->data[2] * pM->data[5];
    float32_t d4 = pM->data[1] * pM->data[7] - pM->data[3] * pM->data[5];
    float32_t d5 = pM->data[2] * pM->data[7] - pM->data[3] * pM->data[6];
    float32_t d6 = pM->data[8] * pM->data[13] - pM->data[9] * pM->data[12];
    float32_t d7 = pM->data[8] * pM->data[14] - pM->data[10] * pM->data[12];
    float32_t d8 = pM->data[8] * pM->data[15] - pM->data[11] * pM->data[12];
    float32_t d9 = pM->data[9] * pM->data[14] - pM->data[10] * pM->data[13];
    float32_t d10 = pM->data[9] * pM->data[15] - pM->data[11] * pM->data[13];
    float32_t d11 = pM->data[10] * pM->data[15] - pM->data[11] * pM->data[14];
    float32_t determinant = d0 * d11 - d1 * d10 + d2 * d9 + d3 * d8 - d4 * d7 + d5 * d6;

    if (determinant == 0.0f) return pM;

    determinant = 1.0f / determinant;
    pOut->data[0] = (pM->data[5] * d11 - pM->data[6] * d10 + pM->data[7] * d9) * determinant;
    pOut->data[1] = (pM->data[2] * d10 - pM->data[1] * d11 - pM->data[3] * d9) * determinant;
    pOut->data[2] = (pM->data[13] * d5 - pM->data[14] * d4 + pM->data[15] * d3) * determinant;
    pOut->data[3] = (pM->data[10] * d4 - pM->data[9] * d5 - pM->data[11] * d3) * determinant;
    pOut->data[4] = (pM->data[6] * d8 - pM->data[4] * d11 - pM->data[7] * d7) * determinant;
    pOut->data[5] = (pM->data[0] * d11 - pM->data[2] * d8 + pM->data[3] * d7) * determinant;
    pOut->data[6] = (pM->data[14] * d2 - pM->data[12] * d5 - pM->data[15] * d1) * determinant;
    pOut->data[7] = (pM->data[8] * d5 - pM->data[10] * d2 + pM->data[11] * d1) * determinant;
    pOut->data[8] = (pM->data[4] * d10 - pM->data[5] * d8 + pM->data[7] * d6) * determinant;
    pOut->data[9] = (pM->data[1] * d8 - pM->data[0] * d10 - pM->data[3] * d6) * determinant;
    pOut->data[10] = (pM->data[12] * d4 - pM->data[13] * d2 + pM->data[15] * d0) * determinant;
    pOut->data[11] = (pM->data[9] * d2 - pM->data[8] * d4 - pM->data[11] * d0) * determinant;
    pOut->data[12] = (pM->data[5] * d7 - pM->data[4] * d9 - pM->data[6] * d6) * determinant;
    pOut->data[13] = (pM->data[0] * d9 - pM->data[1] * d7 + pM->data[2] * d6) * determinant;
    pOut->data[14] = (pM->data[13] * d1 - pM->data[12] * d3 - pM->data[14] * d0) * determinant;
    pOut->data[15] = (pM->data[8] * d3 - pM->data[9] * d1 + pM->data[10] * d0) * determinant;
    return pOut;
}
gunk::Matrix4* gunk::math::RotateX(gunk::Matrix4* pOut, gunk::Matrix4* pM, float32_t radian)
{
    float32_t
        a00 = pM->data[0], a01 = pM->data[1], a02 = pM->data[2], a03 = pM->data[3],
        a04 = pM->data[4], a05 = pM->data[5], a06 = pM->data[6], a07 = pM->data[7],
        a08 = pM->data[8], a09 = pM->data[9], a10 = pM->data[10], a11 = pM->data[11],
        a12 = pM->data[12], a13 = pM->data[13], a14 = pM->data[14], a15 = pM->data[15];
    
    float32_t* pMatrixData = pOut->data;
    float32_t sn = sinf(radian);
    float32_t cn = cosf(radian);
    
    pMatrixData[0x0] = a00;
    pMatrixData[0x1] = a01;
    pMatrixData[0x2] = a02;
    pMatrixData[0x3] = a03;
    pMatrixData[0xC] = a12;
    pMatrixData[0xD] = a13;
    pMatrixData[0xE] = a14;
    pMatrixData[0xF] = a15;
    pMatrixData[0x4] = a04 * cn + a08 * sn;
    pMatrixData[0x5] = a05 * cn + a09 * sn;
    pMatrixData[0x6] = a06 * cn + a10 * sn;
    pMatrixData[0x7] = a07 * cn + a11 * sn;
    pMatrixData[0x8] = a08 * cn - a04 * sn;
    pMatrixData[0x9] = a09 * cn - a05 * sn;
    pMatrixData[0xA] = a10 * cn - a06 * sn;
    pMatrixData[0xB] = a11 * cn - a07 * sn;
    
    return pOut;
}
gunk::Matrix4* gunk::math::RotateY(gunk::Matrix4* pOut, gunk::Matrix4* pM, float32_t radian)
{
    float32_t
        a00 = pM->data[0], a01 = pM->data[1], a02 = pM->data[2], a03 = pM->data[3],
        a04 = pM->data[4], a05 = pM->data[5], a06 = pM->data[6], a07 = pM->data[7],
        a08 = pM->data[8], a09 = pM->data[9], a10 = pM->data[10], a11 = pM->data[11],
        a12 = pM->data[12], a13 = pM->data[13], a14 = pM->data[14], a15 = pM->data[15];

    float32_t* pMatrixData = pOut->data;
    float32_t sn = sinf(radian);
    float32_t cn = cosf(radian);

    pMatrixData[0x0] = a00 * cn - a08 * sn;
    pMatrixData[0x1] = a01 * cn - a09 * sn;
    pMatrixData[0x2] = a02 * cn - a10 * sn;
    pMatrixData[0x3] = a03 * cn - a11 * sn;
    pMatrixData[0x4] = a04;
    pMatrixData[0x5] = a05;
    pMatrixData[0x6] = a06;
    pMatrixData[0x7] = a07;
    pMatrixData[0x8] = a00 * sn + a08 * cn;
    pMatrixData[0x9] = a01 * sn + a09 * cn;
    pMatrixData[0xA] = a02 * sn + a10 * cn;
    pMatrixData[0xB] = a03 * sn + a11 * cn;
    pMatrixData[0xC] = a12;
    pMatrixData[0xD] = a13;
    pMatrixData[0xE] = a14;
    pMatrixData[0xF] = a15;

    return pOut;
}
gunk::Matrix4* gunk::math::RotateZ(gunk::Matrix4* pOut, gunk::Matrix4* pM, float32_t radian)
{
    float32_t
        a00 = pM->data[0], a01 = pM->data[1], a02 = pM->data[2], a03 = pM->data[3],
        a04 = pM->data[4], a05 = pM->data[5], a06 = pM->data[6], a07 = pM->data[7],
        a08 = pM->data[8], a09 = pM->data[9], a10 = pM->data[10], a11 = pM->data[11],
        a12 = pM->data[12], a13 = pM->data[13], a14 = pM->data[14], a15 = pM->data[15];

    float32_t* pMatrixData = pOut->data;
    float32_t sn = sinf(radian);
    float32_t cn = cosf(radian);

    pMatrixData[0x0] = a00 * cn + a04 * sn;
    pMatrixData[0x1] = a01 * cn + a05 * sn;
    pMatrixData[0x2] = a02 * cn + a06 * sn;
    pMatrixData[0x3] = a03 * cn + a07 * sn;
    pMatrixData[0x4] = a04 * cn - a00 * sn;
    pMatrixData[0x5] = a05 * cn - a01 * sn;
    pMatrixData[0x6] = a06 * cn - a02 * sn;
    pMatrixData[0x7] = a07 * cn - a03 * sn;
    pMatrixData[0x8] = a08;
    pMatrixData[0x9] = a09;
    pMatrixData[0xA] = a10;
    pMatrixData[0xB] = a11;
    pMatrixData[0xC] = a12;
    pMatrixData[0xD] = a13;
    pMatrixData[0xE] = a14;
    pMatrixData[0xF] = a15;

    return pOut;
}
gunk::Matrix4* gunk::math::Rotate(gunk::Matrix4* pOut, gunk::Matrix4* pM, gunk::Vector3* axis, float32_t radian)
{
    float32_t length = Length(axis);

    if (fabsf(length) < 0.0000001f) return NULL;

    float32_t
        a00 = pM->data[0], a01 = pM->data[1], a02 = pM->data[2], a03 = pM->data[3],
        a04 = pM->data[4], a05 = pM->data[5], a06 = pM->data[6], a07 = pM->data[7],
        a08 = pM->data[8], a09 = pM->data[9], a10 = pM->data[10], a11 = pM->data[11],
        a12 = pM->data[12], a13 = pM->data[13], a14 = pM->data[14], a15 = pM->data[15];
    float32_t* pMatrixData = pOut->data;

    length = 1 / length;
    axis->x *= length;
    axis->y *= length;
    axis->z *= length;
    float32_t sn = sinf(radian);
    float32_t cn = cosf(radian);
    float32_t theta = 1 - cn;
    float32_t d0 = axis->x * axis->x * theta + cn;
    float32_t d4 = axis->y * axis->y * theta + cn;
    float32_t d8 = axis->z * axis->z * theta + cn;
    float32_t d1 = axis->y * axis->x * theta + axis->z * sn;
    float32_t d2 = axis->z * axis->x * theta - axis->y * sn;
    float32_t d3 = axis->x * axis->y * theta - axis->z * sn;
    float32_t d5 = axis->z * axis->y * theta + axis->x * sn;
    float32_t d6 = axis->x * axis->z * theta + axis->y * sn;
    float32_t d7 = axis->y * axis->z * theta - axis->x * sn;

    pMatrixData[0x0] = a00 * d0 + a04 * d1 + a08 * d2;
    pMatrixData[0x1] = a01 * d0 + a05 * d1 + a09 * d2;
    pMatrixData[0x2] = a02 * d0 + a06 * d1 + a10 * d2;
    pMatrixData[0x3] = a03 * d0 + a07 * d1 + a11 * d2;
    pMatrixData[0x4] = a00 * d3 + a04 * d4 + a08 * d5;
    pMatrixData[0x5] = a01 * d3 + a05 * d4 + a09 * d5;
    pMatrixData[0x6] = a02 * d3 + a06 * d4 + a10 * d5;
    pMatrixData[0x7] = a03 * d3 + a07 * d4 + a11 * d5;
    pMatrixData[0x8] = a00 * d6 + a04 * d7 + a08 * d8;
    pMatrixData[0x9] = a01 * d6 + a05 * d7 + a09 * d8;
    pMatrixData[0xA] = a02 * d6 + a06 * d7 + a10 * d8;
    pMatrixData[0xB] = a03 * d6 + a07 * d7 + a11 * d8;
    pMatrixData[0xC] = a12;
    pMatrixData[0xD] = a13;
    pMatrixData[0xE] = a14;
    pMatrixData[0xF] = a15;

    return pOut;
}
gunk::Matrix4* gunk::math::Scale(gunk::Matrix4* pOut, gunk::Matrix4* pM, gunk::Vector3* scale)
{
    pOut->data[0] = pM->data[0] * scale->x;
    pOut->data[1] = pM->data[1] * scale->x;
    pOut->data[2] = pM->data[2] * scale->x;
    pOut->data[3] = pM->data[3] * scale->x;
    pOut->data[4] = pM->data[4] * scale->y;
    pOut->data[5] = pM->data[5] * scale->y;
    pOut->data[6] = pM->data[6] * scale->y;
    pOut->data[7] = pM->data[7] * scale->y;
    pOut->data[8] = pM->data[8] * scale->z;
    pOut->data[9] = pM->data[9] * scale->z;
    pOut->data[10] = pM->data[10] * scale->z;
    pOut->data[11] = pM->data[11] * scale->z;
    pOut->data[12] = pM->data[12];
    pOut->data[13] = pM->data[13];
    pOut->data[14] = pM->data[14];
    pOut->data[15] = pM->data[15];
    return pOut;
}
gunk::Matrix4* gunk::math::Translate(gunk::Matrix4* pOut, gunk::Matrix4* pM, gunk::Vector3* translate)
{
    pOut->data[0] = pM->data[0];
    pOut->data[1] = pM->data[1];
    pOut->data[2] = pM->data[2];
    pOut->data[3] = pM->data[3];
    pOut->data[4] = pM->data[4];
    pOut->data[5] = pM->data[5];
    pOut->data[6] = pM->data[6];
    pOut->data[7] = pM->data[7];
    pOut->data[8] = pM->data[8];
    pOut->data[9] = pM->data[9];
    pOut->data[10] = pM->data[10];
    pOut->data[11] = pM->data[11];
    pOut->data[12] = pM->data[0] * translate->x + pM->data[4] * translate->y + pM->data[8] * translate->z + pM->data[12];
    pOut->data[13] = pM->data[1] * translate->x + pM->data[5] * translate->y + pM->data[9] * translate->z + pM->data[13];
    pOut->data[14] = pM->data[2] * translate->x + pM->data[6] * translate->y + pM->data[10] * translate->z + pM->data[14];
    pOut->data[15] = pM->data[3] * translate->x + pM->data[7] * translate->y + pM->data[11] * translate->z + pM->data[15];
    return pOut;
}
gunk::Matrix4* gunk::math::Frustum(gunk::Matrix4* pOut, float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t frustum_near, float32_t frustum_far)
{
    float32_t right_left = 1.0f / (right - left);
    float32_t top_bottom = 1.0f / (top - bottom);
    float32_t near_far = 1.0f / (frustum_near - frustum_far);
    pOut->data[0] = (frustum_near * 2.0f) * right_left;
    pOut->data[1] = 0.0f;
    pOut->data[2] = 0.0f;
    pOut->data[3] = 0.0f;
    pOut->data[4] = 0.0f;
    pOut->data[5] = (frustum_near * 2.0f) * top_bottom;
    pOut->data[6] = 0.0f;
    pOut->data[7] = 0.0f;
    pOut->data[8] = (right + left) * right_left;
    pOut->data[9] = (top + bottom) * top_bottom;
    pOut->data[10] = (frustum_far + frustum_near) * near_far;
    pOut->data[11] = -1.0f;
    pOut->data[12] = 0.0f;
    pOut->data[13] = 0.0f;
    pOut->data[14] = (frustum_far * frustum_near * 2.0f) * near_far;
    pOut->data[15] = 0.0f;
    return pOut;
}
gunk::Matrix4* gunk::math::Perspective(gunk::Matrix4* pOut, float32_t fov_y, float32_t aspect, float32_t perspNear, float32_t perspFar)
{
    float32_t fov = 1.0f / tanf(fov_y / 2.0f);
    float32_t near_far = 1.0f / (perspNear - perspFar);
    pOut->data[0] = fov / aspect;
    pOut->data[1] = 0.0f;
    pOut->data[2] = 0.0f;
    pOut->data[3] = 0.0f;
    pOut->data[4] = 0.0f;
    pOut->data[5] = fov;
    pOut->data[6] = 0.0f;
    pOut->data[7] = 0.0f;
    pOut->data[8] = 0.0f;
    pOut->data[9] = 0.0f;
    pOut->data[10] = (perspFar + perspNear) * near_far;
    pOut->data[11] = -1.0f;
    pOut->data[12] = 0.0f;
    pOut->data[13] = 0.0f;
    pOut->data[14] = (2.0f * perspFar * perspNear) * near_far;
    pOut->data[15] = 0.0f;
    return pOut;
}
gunk::Matrix4* gunk::math::Orthographic(gunk::Matrix4* pOut, float32_t left, float32_t right, float32_t bottom, float32_t top, float32_t orthoNear, float32_t orthoFar)
{
    float32_t left_right = 1.0f / (left - right);
    float32_t bottom_top = 1.0f / (bottom - top);
    float32_t near_far = 1.0f / (orthoNear - orthoFar);
    pOut->data[0] = -2.0f * left_right;
    pOut->data[1] = 0.0f;
    pOut->data[2] = 0.0f;
    pOut->data[3] = 0.0f;
    pOut->data[4] = 0.0f;
    pOut->data[5] = -2.0f * bottom_top;
    pOut->data[6] = 0.0f;
    pOut->data[7] = 0.0f;
    pOut->data[8] = 0.0f;
    pOut->data[9] = 0.0f;
    pOut->data[10] = 2.0f * near_far;
    pOut->data[11] = 0.0f;
    pOut->data[12] = (left + right) * left_right;
    pOut->data[13] = (top + bottom) * bottom_top;
    pOut->data[14] = (orthoFar + orthoNear) * near_far;
    pOut->data[15] = 1.0f;
    return pOut;
}
gunk::Matrix4* gunk::math::LookAt(gunk::Matrix4* pOut, gunk::Vector3* p_eye, gunk::Vector3* p_center, gunk::Vector3* p_up)
{
    if (fabsf(p_eye->x - p_center->x) < 0.0000001f &&
        fabsf(p_eye->y - p_center->y) < 0.0000001f &&
        fabsf(p_eye->z - p_center->z) < 0.0000001f)
    {
        return Identity(pOut);
    }
    float32_t z0 = p_eye->x - p_center->x;
    float32_t z1 = p_eye->y - p_center->y;
    float32_t z2 = p_eye->z - p_center->z;
    float32_t len = 1.0f / sqrtf(z0 * z0 + z1 * z1 + z2 * z2);
    z0 *= len;
    z1 *= len;
    z2 *= len;
    float32_t x0 = p_up->y * z2 - p_up->z * z1;
    float32_t x1 = p_up->z * z0 - p_up->x * z2;
    float32_t x2 = p_up->x * z1 - p_up->y * z0;
    len = sqrtf(x0 * x0 + x1 * x1 + x2 * x2);
    if (!len)
    {
        x0 = 0.0f;
        x1 = 0.0f;
        x2 = 0.0f;
    }
    else
    {
        len = 1.0f / len;
        x0 *= len;
        x1 *= len;
        x2 *= len;
    }
    float32_t y0 = z1 * x2 - z2 * x1;
    float32_t y1 = z2 * x0 - z0 * x2;
    float32_t y2 = z0 * x1 - z1 * x0;
    len = sqrtf(y0 * y0 + y1 * y1 + y2 * y2);
    if (!len)
    {
        y0 = 0.0f;
        y1 = 0.0f;
        y2 = 0.0f;
    }
    else
    {
        len = 1.0f / len;
        y0 *= len;
        y1 *= len;
        y2 *= len;
    }
    pOut->data[0] = x0;
    pOut->data[1] = y0;
    pOut->data[2] = z0;
    pOut->data[3] = 0.0f;
    pOut->data[4] = x1;
    pOut->data[5] = y1;
    pOut->data[6] = z1;
    pOut->data[7] = 0.0f;
    pOut->data[8] = x2;
    pOut->data[9] = y2;
    pOut->data[10] = z2;
    pOut->data[11] = 0.0f;
    pOut->data[12] = -(x0 * p_eye->x + x1 * p_eye->y + x2 * p_eye->z);
    pOut->data[13] = -(y0 * p_eye->x + y1 * p_eye->y + y2 * p_eye->z);
    pOut->data[14] = -(z0 * p_eye->x + z1 * p_eye->y + z2 * p_eye->z);
    pOut->data[15] = 1.0f;
    return pOut;
}

/* vec2 */
gunk::Vector2* gunk::math::Add(gunk::Vector2* pOut, gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    pOut->x = pV0->x + pV1->x;
    pOut->y = pV0->y + pV1->y;
    return pOut;
}
gunk::Vector2* gunk::math::Sub(gunk::Vector2* pOut, gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    pOut->x = pV0->x - pV1->x;
    pOut->y = pV0->y - pV1->y;
    return pOut;
}
gunk::Vector2* gunk::math::Mul(gunk::Vector2* pOut, gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    pOut->x = pV0->x * pV1->x;
    pOut->y = pV0->y * pV1->y;
    return pOut;
}
gunk::Vector2* gunk::math::Invert(gunk::Vector2* pOut, gunk::Vector2* pV)
{
    pOut->x = 1.0f / pV->x;
    pOut->y = 1.0f / pV->y;
    return pOut;
}
gunk::Vector2* gunk::math::Negate(gunk::Vector2* pOut, gunk::Vector2* pV)
{
    pOut->x = pV->x * -1.0f;
    pOut->y = pV->y * -1.0f;
    return pOut;
}
gunk::Vector2* gunk::math::Normalize(gunk::Vector2* pOut, gunk::Vector2* pV)
{
    float32_t l = pV->x * pV->x + pV->y * pV->y;
    if (l > 0.0f)
    {
        l = 1.0f / sqrtf(l);
        pOut->x = pV->x * l;
        pOut->y = pV->y * l;
    }
    return pOut;
}
int32_t gunk::math::Equal(gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    return (pV0->x == pV1->x && pV0->y == pV1->y);
}
gunk::Vector3* gunk::math::CrossProduct(gunk::Vector3* pOut, gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    float32_t x0 = pV0->x;
    float32_t y0 = pV0->y;
    float32_t x1 = pV1->x;
    float32_t y1 = pV1->y;
    pOut->x = 0;
    pOut->y = 0;
    pOut->z = x0 * y1 - y0 * x1;
    return pOut;
}
float32_t gunk::math::Distance(gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    float32_t x = pV0->x - pV1->x;
    float32_t y = pV0->y - pV1->y;
    return sqrtf(x * x + y * y);
}
float32_t gunk::math::DotProduct(gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    return pV0->x * pV1->x + pV0->y * pV1->y;
}
float32_t gunk::math::Length(gunk::Vector2* pV)
{
    return sqrtf(pV->x * pV->x + pV->y * pV->y);
}
float32_t gunk::math::Length2(gunk::Vector2* pV)
{
    return (pV->x * pV->x + pV->y * pV->y);
}
float32_t gunk::math::Distance2(gunk::Vector2* pV0, gunk::Vector2* pV1)
{
    float32_t x = pV0->x - pV1->x;
    float32_t y = pV0->y - pV1->y;
    return (x * x + y * y);
}

/* vec3 */
gunk::Vector3* gunk::math::Add(gunk::Vector3* pOut, gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    pOut->x = pV0->x + pV1->x;
    pOut->y = pV0->y + pV1->y;
    pOut->z = pV0->z + pV1->z;
    return pOut;
}
gunk::Vector3* gunk::math::Sub(gunk::Vector3* pOut, gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    pOut->x = pV0->x - pV1->x;
    pOut->y = pV0->y - pV1->y;
    pOut->z = pV0->z - pV1->z;
    return pOut;
}
gunk::Vector3* gunk::math::Mul(gunk::Vector3* pOut, gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    pOut->x = pV0->x * pV1->x;
    pOut->y = pV0->y * pV1->y;
    pOut->z = pV0->z * pV1->z;
    return pOut;
}
gunk::Vector3* gunk::math::Invert(gunk::Vector3* pOut, gunk::Vector3* pV)
{
    pOut->x = 1.0f / pV->x;
    pOut->y = 1.0f / pV->y;
    pOut->z = 1.0f / pV->z;
    return pOut;
}
gunk::Vector3* gunk::math::Negate(gunk::Vector3* pOut, gunk::Vector3* pV)
{
    pOut->x = -1.0f * pV->x;
    pOut->y = -1.0f * pV->y;
    pOut->z = -1.0f * pV->z;
    return pOut;
}
gunk::Vector3* gunk::math::Normalize(gunk::Vector3* pOut, gunk::Vector3* pV)
{
    float32_t x = pV->x;
    float32_t y = pV->y;
    float32_t z = pV->z;
    float32_t l = x * x + y * y + z * z;
    if (l > 0.0f)
    {
        l = 1.0f / sqrtf(l);
        pOut->x = x * l;
        pOut->y = y * l;
        pOut->z = z * l;
    }
    return pOut;
}
int32_t gunk::math::Equal(gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    return (pV0->x == pV1->x && pV0->y == pV1->y && pV0->z == pV1->z);
}
gunk::Vector3* gunk::math::CrossProduct(gunk::Vector3* pOut, gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    float32_t lx = pV0->x;
    float32_t ly = pV0->y;
    float32_t lz = pV0->z;
    float32_t rx = pV1->x;
    float32_t ry = pV1->y;
    float32_t rz = pV1->z;

    pOut->x = ly * rz - lz * ry;
    pOut->y = lz * rx - lx * rz;
    pOut->z = lx * ry - ly * rx;

    return pOut;
}
float32_t gunk::math::Distance(gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    float32_t x = pV0->x - pV1->x;
    float32_t y = pV0->y - pV1->y;
    float32_t z = pV0->z - pV1->z;

    return sqrtf(x * x + y * y + z * z);
}
float32_t gunk::math::DotProduct(gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    return pV0->x * pV1->x + pV0->y * pV1->y + pV0->z * pV1->z;
}
float32_t gunk::math::Length(gunk::Vector3* pV)
{
    float32_t x = pV->x;
    float32_t y = pV->y;
    float32_t z = pV->z;
    return sqrtf(x * x + y * y + z * z);
}
float32_t gunk::math::Length2(gunk::Vector3* pV)
{
    float32_t x = pV->x;
    float32_t y = pV->y;
    float32_t z = pV->z;
    return (x * x + y * y + z * z);
}
float32_t gunk::math::Distance2(gunk::Vector3* pV0, gunk::Vector3* pV1)
{
    float32_t x = pV0->x - pV1->x;
    float32_t y = pV0->y - pV1->y;
    float32_t z = pV0->z - pV1->z;

    return (x * x + y * y + z * z);
}

/* quat */
gunk::Quaternion* gunk::math::Identity(gunk::Quaternion* pOut)
{
    pOut->x = 0;
    pOut->y = 0;
    pOut->z = 0;
    pOut->w = 1;
    return pOut;
}
gunk::Quaternion* gunk::math::Add(gunk::Quaternion* pOut, gunk::Quaternion* pQ0, gunk::Quaternion* pQ1)
{
    pOut->w = pQ0->w + pQ1->w;
    pOut->x = pQ0->x + pQ1->x;
    pOut->y = pQ0->y + pQ1->y;
    pOut->z = pQ0->z + pQ1->z;
    return pOut;
}
gunk::Quaternion* gunk::math::Mul(gunk::Quaternion* pOut, gunk::Quaternion* pQ0, gunk::Quaternion* pQ1)
{
    float32_t q0x = pQ0->x, q0y = pQ0->y, q0z = pQ0->z, q0w = pQ0->w;
    float32_t q1x = pQ1->x, q1y = pQ1->y, q1z = pQ1->z, q1w = pQ1->w;

    pOut->x = q0x * q1w + q0w * q1x + q0y * q1z - q0z * q1y;
    pOut->y = q0y * q1w + q0w * q1y + q0z * q1x - q0x * q1z;
    pOut->z = q0z * q1w + q0w * q1z + q0x * q1y - q0y * q1x;
    pOut->w = q0w * q1w - q0x * q1x - q0y * q1y - q0z * q1z;
    
    return pOut;
}
gunk::Quaternion* gunk::math::SetAxisAngle(gunk::Quaternion* pOut, gunk::Vector3* axis, float32_t radian)
{
    float32_t sn = sinf(radian * 0.5f);
    pOut->x = sn * axis->x;
    pOut->y = sn * axis->y;
    pOut->z = sn * axis->z;
    pOut->w = cosf(radian * 0.5f);
    return pOut;
}
gunk::Quaternion* gunk::math::RotateX(gunk::Quaternion* pOut, gunk::Quaternion* pQ, float32_t radian)
{
    float32_t qx = pQ->x, qy = pQ->y, qz = pQ->z, qw = pQ->w;
    float32_t sn = sinf(radian * 0.5f);
    float32_t cn = cosf(radian * 0.5f);
    pOut->x = qx * sn + qw * cn;
    pOut->y = qy * sn + qz * cn;
    pOut->z = qz * sn - qy * cn;
    pOut->w = qw * sn - qx * cn;
    return pOut;
}
gunk::Quaternion* gunk::math::RotateY(gunk::Quaternion* pOut, gunk::Quaternion* pQ, float32_t radian)
{
    float32_t qx = pQ->x, qy = pQ->y, qz = pQ->z, qw = pQ->w;
    float32_t sn = sinf(radian * 0.5f);
    float32_t cn = cosf(radian * 0.5f);
    pOut->x = qx * cn - qz * sn;
    pOut->y = qy * cn + qw * sn;
    pOut->z = qz * cn + qx * sn;
    pOut->w = qw * cn - qy * sn;
    return pOut;
}
gunk::Quaternion* gunk::math::RotateZ(gunk::Quaternion* pOut, gunk::Quaternion* pQ, float32_t radian)
{
    float32_t qx = pQ->x, qy = pQ->y, qz = pQ->z, qw = pQ->w;
    float32_t sn = sinf(radian * 0.5f);
    float32_t cn = cosf(radian * 0.5f);
    pOut->x = qx * cn + qy * sn;
    pOut->y = qy * cn - qx * sn;
    pOut->z = qz * cn + qw * sn;
    pOut->w = qw * cn - qz * sn;
    return pOut;
}
gunk::Quaternion* gunk::math::CalculateW(gunk::Quaternion* pOut, gunk::Quaternion* pQ)
{
    float32_t qx = pQ->x, qy = pQ->y, qz = pQ->z, qw = pQ->w;
    pOut->x = qx;
    pOut->y = qy;
    pOut->z = qz;
    pOut->w = sqrtf(fabsf(1.0f - qx * qx - qy * qy - qz * qz));
    return pOut;
}
gunk::Quaternion* gunk::math::Invert(gunk::Quaternion* pOut, gunk::Quaternion* pQ)
{
    float32_t dot = pQ->x * pQ->x + pQ->y * pQ->y + pQ->z * pQ->z + pQ->w * pQ->w;
    if (dot) dot = 1.0f / dot;
    pOut->x = -pQ->x * dot;
    pOut->y = -pQ->y * dot;
    pOut->z = -pQ->z * dot;
    pOut->w = pQ->w * dot;
    return pOut;
}
gunk::Quaternion* gunk::math::Conjugate(gunk::Quaternion* pOut, gunk::Quaternion* pQ)
{
    pOut->x = -pQ->x;
    pOut->y = -pQ->y;
    pOut->z = -pQ->z;
    pOut->w = pQ->w;
    return pOut;
}
gunk::Matrix4* gunk::math::QuaternionToMatrix4(gunk::Matrix4* pOut, gunk::Quaternion* pQ)
{
    float32_t x2 = pQ->x + pQ->x;
    float32_t y2 = pQ->y + pQ->y;
    float32_t z2 = pQ->z + pQ->z;
    float32_t xx = pQ->x * x2;
    float32_t yx = pQ->y * x2;
    float32_t yy = pQ->y * y2;
    float32_t zx = pQ->z * x2;
    float32_t zy = pQ->z * y2;
    float32_t zz = pQ->z * z2;
    float32_t wx = pQ->w * x2;
    float32_t wy = pQ->w * y2;
    float32_t wz = pQ->w * z2;

    pOut->data[0] = 1.0f - yy - zz;
    pOut->data[1] = yx + wz;
    pOut->data[2] = zx - wy;
    pOut->data[3] = 0.0f;
    pOut->data[4] = yx - wz;
    pOut->data[5] = 1.0f - xx - zz;
    pOut->data[6] = zy + wx;
    pOut->data[7] = 0.0f;
    pOut->data[8] = zx + wy;
    pOut->data[9] = zy - wx;
    pOut->data[10] = 1.0f - xx - yy;
    pOut->data[11] = 0.0f;
    pOut->data[12] = 0.0f;
    pOut->data[13] = 0.0f;
    pOut->data[14] = 0.0f;
    pOut->data[15] = 1.0f;
    return pOut;
}
float32_t gunk::math::GetAxisAngle(gunk::Vector3* pOutAxis, gunk::Quaternion* pQ)
{
    float32_t radian = acosf(pQ->w) * 2.0f;
    float32_t sn = sinf(radian * 0.5f);
    if (sn != 0.0f)
    {
        pOutAxis->x = pQ->x / sn;
        pOutAxis->y = pQ->y / sn;
        pOutAxis->z = pQ->z / sn;
    }
    else
    {
        pOutAxis->x = 1.0f;
        pOutAxis->y = 0.0f;
        pOutAxis->z = 0.0f;
    }
    return radian;
}