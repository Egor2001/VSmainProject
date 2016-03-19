#ifndef MATH3D_H_INCLUDED
#define MATH3D_H_INCLUDED

#include "stdafx.h"

struct Vect
    {
    float x, y, z, p;

    Vect ();
    Vect (float x_set, float y_set, float z_set);
    Vect (float x_set, float y_set, float z_set, float p_set);

    ~Vect ();

    inline  Vect  operator - () const;
    inline  Vect  operator ! () const;

    inline float    Length() const;
    inline void     Simple();
    inline Vect  GetSimple() const;
    };

inline bool  operator == (const Vect&, const Vect&);
inline void  operator *= (      Vect&,       float);
inline Vect  operator *  (const Vect&,       float);
inline Vect  operator *  (      float, const Vect&);
inline void  operator /= (      Vect&,       float);
inline Vect  operator /  (const Vect&,       float);
inline void  operator += (      Vect&, const Vect&);
inline Vect  operator +  (const Vect&, const Vect&);
inline void  operator -= (      Vect&, const Vect&);
inline Vect  operator -  (const Vect&, const Vect&);
inline float operator %  (const Vect&, const Vect&);
inline void  operator *= (      Vect&, const Vect&);
inline Vect  operator *  (const Vect&, const Vect&);

Vect::Vect () :
    x (0), y (0), z (0), p (1) {}

Vect::Vect (float x_set, float y_set, float z_set) :
    x (x_set), y (y_set), z (z_set), p (1) {}

Vect::Vect (float x_set, float y_set, float z_set, float p_set) :
    x (x_set), y (y_set), z (z_set), p (p_set) {}

Vect::~Vect ()
    {}

Vect Vect::operator - () const
    {
    return Vect (-x, -y, -z, p);
    }

Vect Vect::operator ! () const
    {
    float len = Length();

    if (len == 0.0f) return Vect ();

    return (*this) / len;
    }

float Vect::Length() const
    {
    return sqrtf (x * x + y * y + z * z) / p;
    }

void Vect::Simple()
    {
    x /= p;
    y /= p;
    z /= p;
    p = 1;
    }

Vect Vect::GetSimple() const
    {
    Vect norm = (*this);

    norm.Simple ();

    return norm;
    }

struct Matrix
    {
    float matrix [4][4];

    Matrix ();

    Matrix (float Set00, float Set01, float Set02, float Set03,
            float Set10, float Set11, float Set12, float Set13,
            float Set20, float Set21, float Set22, float Set23,
            float Set30, float Set31, float Set32, float Set33);

    Matrix (float Set00, float Set01, float Set02,
            float Set10, float Set11, float Set12,
            float Set20, float Set21, float Set22);

    Matrix (float Set03, float Set13, float Set23);

    ~Matrix ();

    inline const float* operator[] (int num) const;
    inline       float* operator[] (int num);

    inline void Inv_Direct (Matrix& change, Matrix& result) const;
    inline void Inv_Reverse (Matrix& change, Matrix& result) const;

    inline Matrix operator ~ () const;
    };

inline void   operator *= (      Matrix&,         float);
inline Matrix operator *  (const Matrix&,         float);
inline Matrix operator *  (        float, const Matrix&);
inline void   operator /= (      Matrix&,         float);
inline Matrix operator /  (const Matrix&,         float);
inline void   operator *= (      Matrix&, const Matrix&);
inline Matrix operator *  (const Matrix&, const Matrix&);
inline void   operator /= (      Matrix&, const Matrix&);
inline Matrix operator /  (const Matrix&, const Matrix&);
inline void   operator *= (        Vect&, const Matrix&);
inline Vect   operator *  (const   Vect&, const Matrix&);
inline void   operator /= (        Vect&, const Matrix&);
inline Vect   operator /  (const   Vect&, const Matrix&);
inline Vect   operator *  (const Matrix&, const   Vect&);

Matrix::Matrix()
    {
    memset (matrix, 0, sizeof (matrix));

    for (int i = 0; i < 4; i++)
        matrix [i][i] = 1.0;
    }

Matrix::Matrix (float Set00, float Set01, float Set02, float Set03,
                float Set10, float Set11, float Set12, float Set13,
                float Set20, float Set21, float Set22, float Set23,
                float Set30, float Set31, float Set32, float Set33)
    {
    matrix [0][0] = Set00; matrix [0][1] = Set01; matrix [0][2] = Set02; matrix [0][3] = Set03;
    matrix [1][0] = Set10; matrix [1][1] = Set11; matrix [1][2] = Set12; matrix [1][3] = Set13;
    matrix [2][0] = Set20; matrix [2][1] = Set21; matrix [2][2] = Set22; matrix [2][3] = Set23;
    matrix [3][0] = Set30; matrix [3][1] = Set31; matrix [3][2] = Set32; matrix [3][3] = Set33;
    }

Matrix::Matrix (float Set00, float Set01, float Set02,
                float Set10, float Set11, float Set12,
                float Set20, float Set21, float Set22)
    {
    matrix [0][0] = Set00; matrix [0][1] = Set01; matrix [0][2] = Set02; matrix [0][3] = 0.0f;
    matrix [1][0] = Set10; matrix [1][1] = Set11; matrix [1][2] = Set12; matrix [1][3] = 0.0f;
    matrix [2][0] = Set20; matrix [2][1] = Set21; matrix [2][2] = Set22; matrix [2][3] = 0.0f;
    matrix [3][0] = 0.0f ; matrix [3][1] = 0.0f ; matrix [3][2] = 0.0f ; matrix [3][3] = 1.0f;
    }

Matrix::Matrix (float Set03, float Set13, float Set23)
    {
    matrix [0][0] = 1.0f; matrix [0][1] = 0.0f; matrix [0][2] = 0.0f; matrix [0][3] = Set03;
    matrix [1][0] = 0.0f; matrix [1][1] = 1.0f; matrix [1][2] = 0.0f; matrix [1][3] = Set13;
    matrix [2][0] = 0.0f; matrix [2][1] = 0.0f; matrix [2][2] = 1.0f; matrix [2][3] = Set23;
    matrix [3][0] = 0.0f; matrix [3][1] = 0.0f; matrix [3][2] = 0.0f; matrix [3][3] = 1.0f ;
    }

Matrix::~Matrix ()
    {}

const float* Matrix::operator[] (int num) const
    {
    return & (matrix[num][0]);
    }

float* Matrix::operator[] (int num)
    {
    return & (matrix[num][0]);
    }

Matrix Matrix::operator ~ () const
    {
    Matrix result = Matrix ();
    Matrix change = Matrix ();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            change[i][j] = matrix[i][j];

    Inv_Direct  (change, result);
    Inv_Reverse (change, result);

    return result;
    }

void Matrix::Inv_Direct (Matrix& change, Matrix& result) const
    {
    float factor = 0.0f;

    for (int i = 0; i < 4; i++)
        {
        factor = 1 / change[i][i];

        for (int k = 0; k < 4; k++)
            {
            change[i][k] *= factor;
            result[i][k] *= factor;
            }

        for (int j = i + 1; j < 4; j++)
            {
            factor = - (change[j][i] / change[i][i]);

            for (int k = 0; k < 4; k++)
                {
                change[j][k] += change[i][k] * factor;
                result[j][k] += result[i][k] * factor;
                }
            }
        }
    }

void Matrix::Inv_Reverse (Matrix& change, Matrix& result) const
    {
    float factor = 0.0f;

    for (int i = 3; i >= 0; i--)
        {
        for (int j = i - 1; j >= 0; j--)
            {
            factor = - (change[j][i] / change[i][i]);

            for (int k = 0; k < 4; k++)
                {
                change[j][k] += change[i][k] * factor;
                result[j][k] += result[i][k] * factor;
                }
            }
        }
    }

struct Quaternion
    {
    float x, y, z, w;

    Quaternion ();
    Quaternion (const Vect& v_set, float w_set);
    Quaternion (float x_set, float y_set, float z_set, float w_set);
   ~Quaternion ();

    Quaternion operator - () const;

    void SetRotate (const Vect& set_v, float ang);

    inline float      Norm() const;
    inline float Magnitude() const;

    inline Quaternion Conjugated() const;
    inline Quaternion   Inverted() const;

    inline void          Simple();
    inline Quaternion GetSimple() const;
    };

inline void       operator *= (      Quaternion&,             float);
inline Quaternion operator *  (const Quaternion&,             float);
inline Quaternion operator *  (            float, const Quaternion&);
inline void       operator /= (      Quaternion&,             float);
inline Quaternion operator /  (const Quaternion&,             float);
inline void       operator += (      Quaternion&, const Quaternion&);
inline Quaternion operator +  (const Quaternion&, const Quaternion&);
inline void       operator -= (      Quaternion&, const Quaternion&);
inline Quaternion operator -  (const Quaternion&, const Quaternion&);
inline void       operator *= (      Quaternion&, const Quaternion&);
inline Quaternion operator *  (const Quaternion&, const Quaternion&);
inline void       operator /= (      Quaternion&, const Quaternion&);
inline Quaternion operator /  (const Quaternion&, const Quaternion&);
inline void       operator *= (            Vect&, const Quaternion&);
inline Vect       operator *  (const       Vect&, const Quaternion&);
inline void       operator /= (            Vect&, const Quaternion&);
inline Vect       operator /  (const       Vect&, const Quaternion&);
inline Vect       operator *  (const Quaternion&, const       Vect&);
inline float      operator %  (const Quaternion&, const Quaternion&);

inline Quaternion ToQuaternion (const     Matrix& convert);
inline     Matrix ToMatrix (const Quaternion& convert);

Matrix GetScaleMatrix (float factor);
Matrix GetTranslateMatrix (const Vect& dist);
Matrix GetRotateMatrix (const Vect& dir  , float ang);
Matrix ShortArc (const Vect& from , const Vect& to);

Quaternion::Quaternion() :
    x (0.0f), y (0.0f), z (0.0f), w (0.0f) {}

Quaternion::Quaternion (const Vect& v_set, float w_set) :
    x (v_set.x / v_set.p), y (v_set.y / v_set.p), z (v_set.z / v_set.p), w (w_set) {}

Quaternion::Quaternion (float x_set, float y_set, float z_set, float w_set) :
    x (x_set), y (y_set), z (z_set), w (w_set) {}

Quaternion::~Quaternion()
    {}

Quaternion Quaternion::operator - () const
    {
    return Quaternion (-x, -y, -z, -w);
    }

void Quaternion::SetRotate (const Vect& set_v, float ang)
    {
    Vect simp = (!set_v * sin (ang / 2)).GetSimple();

    x = simp.x;
    y = simp.y;
    z = simp.z;

    w = cos (ang / 2);
    }

float Quaternion::Norm() const
    {
    return x * x + y * y + z * z + w * w;
    }

float Quaternion::Magnitude() const
    {
    return sqrtf (Norm());
    }

Quaternion Quaternion::Conjugated() const
    {
    return Quaternion (-x, -y, -z, w);
    }

Quaternion Quaternion::Inverted() const
    {
    return Quaternion (Conjugated() / Norm());
    }

void Quaternion::Simple()
    {
    float magn = Magnitude();

    if (magn == 0.0f) (*this) = Quaternion (0, 0, 0, 1);

    x /= magn;
    y /= magn;
    z /= magn;
    w /= magn;
    }

Quaternion Quaternion::GetSimple() const
    {
    Quaternion norm = (*this);

    norm.Simple();

    return norm;
    }
//============================================
bool operator == (const Vect& v1, const Vect& v2)
    {
    Vect comp1 = v1.GetSimple (),
         comp2 = v2.GetSimple ();

    return !((comp1.x != comp2.x) && (comp1.y != comp2.y) && (comp1.z != comp2.z));
    }

void operator *= (Vect& vec, float mul)
    {
    if (mul == 0.0f)
        {
        vec = Vect ();

        return;
        }

    vec.p /= mul;
    }

Vect operator * (const Vect& vec, float mul)
    {
    Vect result = vec;

    result *= mul;

    return result;
    }

Vect operator * (float mul, const Vect& vec)
    {
    return vec * mul;
    }

void operator /= (Vect& vec, float mul)
    {
    vec.p *= mul;
    }

Vect operator / (const Vect& vec, float mul)
    {
    Vect result = vec;

    result /= mul;

    return result;
    }

void operator += (Vect& vec1, const Vect& vec2)
    {
    vec1.   Simple();
    Vect sum2 = vec2.GetSimple();

    vec1.x += sum2.x;
    vec1.y += sum2.y;
    vec1.z += sum2.z;
    }

Vect operator + (const Vect& vec1, const Vect& vec2)
    {
    Vect sum1  = vec1;
    sum1 += vec2;

    return sum1;
    }

void operator -= (Vect& vec1, const Vect& vec2)
    {
    vec1.   Simple();
    Vect sum2 = vec2.GetSimple();

    vec1.x -= sum2.x;
    vec1.y -= sum2.y;
    vec1.z -= sum2.z;
    }

Vect operator - (const Vect& vec1, const Vect& vec2)
    {
    Vect sum1  = vec1;
    sum1 -= vec2;

    return sum1;
    }

float operator % (const Vect& vec1, const Vect& vec2)
    {
    return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z) / (vec1.p * vec2.p);
    }

void operator *= (Vect& vec1, const Vect& vec2)
    {
    Vect mul1 = vec1;

    vec1.x = (mul1.y * vec2.z) - (mul1.z * vec2.y);
    vec1.y = (mul1.z * vec2.x) - (mul1.x * vec2.z);
    vec1.z = (mul1.x * vec2.y) - (mul1.y * vec2.x);
    vec1.p = (mul1.p * vec2.p);
    }

Vect operator * (const Vect& vec1, const Vect& vec2)
    {
    Vect mul1 = vec1;

    mul1 *= vec2;

    return mul1;
    }
//-----------------------------------------------
void operator *= (Matrix& mat, float mult)
    {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] *= mult;
    }

Matrix operator * (const Matrix& mat, float mult)
    {
    Matrix result = mat;

    result *= mult;

    return result;
    }

Matrix operator * (float mult, const Matrix& mat)
    {
    return mat * mult;
    }

void operator /= (Matrix& mat, float mult)
    {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] /= mult;
    }

Matrix operator / (const Matrix& mat, float mult)
    {
    Matrix result = mat;

    result /= mult;

    return result;
    }

void operator *= (Matrix& mult1, const Matrix& mult2)
    {
    Matrix result;

    memset (result.matrix, 0, sizeof (result.matrix));

    for (int a = 0; a < 4; a++)
    for (int b = 0; b < 4; b++)
        for (int c = 0; c < 4; c++)
            result.matrix[a][b] += mult1.matrix[a][c] * mult2.matrix[c][b];

    mult1 = result;
    }

Matrix operator * (const Matrix& mult1, const Matrix& mult2)
    {
    Matrix result = mult1;

    result *= mult2;

    return result;
    }

void operator /= (Matrix& mult1, const Matrix& mult2)
    {
    mult1 *= ~mult2;
    }

Matrix operator / (const Matrix& mult1, const Matrix& mult2)
    {
    return mult1 * ~mult2;
    }

void operator *= (Vect& vec, const Matrix& mat)
    {
    Vect mult = vec;

    vec.x = mat.matrix[0][0] * mult.x + mat.matrix[0][1] * mult.y +
            mat.matrix[0][2] * mult.z + mat.matrix[0][3] * mult.p;

    vec.y = mat.matrix[1][0] * mult.x + mat.matrix[1][1] * mult.y +
            mat.matrix[1][2] * mult.z + mat.matrix[1][3] * mult.p;

    vec.z = mat.matrix[2][0] * mult.x + mat.matrix[2][1] * mult.y +
            mat.matrix[2][2] * mult.z + mat.matrix[2][3] * mult.p;

    vec.p = mat.matrix[3][0] * mult.x + mat.matrix[3][1] * mult.y +
            mat.matrix[3][2] * mult.z + mat.matrix[3][3] * mult.p;
    }

Vect operator * (const Vect& vec, const Matrix& mat)
    {
    Vect result = vec;

    result *= mat;

    return result;
    }

void operator /= (Vect& vec, const Matrix& mat)
    {
    vec *= ~mat;
    }

Vect operator / (const Vect& vec, const Matrix& mat)
    {
    return vec * ~mat;
    }

Vect operator * (const Matrix& mat, const Vect& vec)
    {
    return vec * mat;
    }
//----------------------------------------
void operator *= (Quaternion& quat, float mult)
    {
    quat.x *= mult;
    quat.y *= mult;
    quat.z *= mult;
    quat.w *= mult;
    }

Quaternion operator * (const Quaternion& quat, float mult)
    {
    return Quaternion (quat.x * mult, quat.y * mult, quat.z * mult, quat.w * mult);
    }

Quaternion operator * (float mult, const Quaternion& quat)
    {
    return quat * mult;
    }

void operator /= (Quaternion& quat, float mult)
    {
    quat.x /= mult;
    quat.y /= mult;
    quat.z /= mult;
    quat.w /= mult;
    }

Quaternion operator / (const Quaternion& quat, float mult)
    {
    return Quaternion (quat.x / mult, quat.y / mult, quat.z / mult, quat.w / mult);
    }

void operator += (Quaternion& add1, const Quaternion& add2)
    {
    add1.x += add2.x;
    add1.y += add2.y;
    add1.z += add2.z;
    add1.w += add2.w;
    }

Quaternion operator + (const Quaternion& add1, const Quaternion& add2)
    {
    return Quaternion (add1.x + add2.x, add1.y + add2.y, add1.z + add2.z, add1.w + add2.w);
    }

void operator -= (Quaternion& add1, const Quaternion& add2)
    {
    add1.x -= add2.x;
    add1.y -= add2.y;
    add1.z -= add2.z;
    add1.w -= add2.w;
    }

Quaternion operator - (const Quaternion& add1, const Quaternion& add2)
    {
    return Quaternion (add1.x - add2.x, add1.y - add2.y, add1.z - add2.z, add1.w - add2.w);
    }

void operator *= (Quaternion& mult1, const Quaternion& mult2)
    {
    mult1 = Quaternion ( (mult1.y * mult2.z) - (mult1.z * mult2.y) + (mult2.x * mult1.w) + (mult1.x * mult2.w),
                         (mult1.z * mult2.x) - (mult1.x * mult2.z) + (mult2.y * mult1.w) + (mult1.y * mult2.w),
                         (mult1.x * mult2.y) - (mult1.y * mult2.x) + (mult2.z * mult1.w) + (mult1.z * mult2.w),
                         (mult1.w * mult2.w) - (mult1.x * mult2.x) - (mult1.y * mult2.y) - (mult1.z * mult2.z));
    }

Quaternion operator * (const Quaternion& mult1, const Quaternion& mult2)
    {
    return Quaternion ((mult1.y * mult2.z) - (mult1.z * mult2.y) + (mult2.x * mult1.w) + (mult1.x * mult2.w),
                       (mult1.z * mult2.x) - (mult1.x * mult2.z) + (mult2.y * mult1.w) + (mult1.y * mult2.w),
                       (mult1.x * mult2.y) - (mult1.y * mult2.x) + (mult2.z * mult1.w) + (mult1.z * mult2.w),
                       (mult1.w * mult2.w) - (mult1.x * mult2.x) - (mult1.y * mult2.y) - (mult1.z * mult2.z));
    }

void operator /= (Quaternion& mult1, const Quaternion& mult2)
    {
    mult1 *= mult2.Inverted ();
    }

Quaternion operator / (const Quaternion& mult1, const Quaternion& mult2)
    {
    return mult1 * mult2.Inverted ();
    }

void operator *= (Vect& vec, const Quaternion& quat)
    {
    Quaternion res = (quat * Quaternion (vec, 0) * quat.Inverted ());

    vec = Vect (res.x, res.y, res.z);
    }

Vect operator * (const Vect& vec, const Quaternion& quat)
    {
    Quaternion res = (quat * Quaternion (vec, 0) * quat.Inverted ());

    return Vect (res.x, res.y, res.z);
    }

void operator /= (Vect& vec, const Quaternion& quat)
    {
    vec *= quat.Inverted ();
    }

Vect operator / (const Vect& vec, const Quaternion& quat)
    {
    return vec * quat.Inverted ();
    }

Vect operator * (const Quaternion& quat, const Vect& vec)
    {
    return vec * quat;
    }

float operator % (const Quaternion& mult1, const Quaternion& mult2)
    {
    return mult1.x * mult2.x + mult1.y * mult2.y + mult1.z * mult2.z + mult1.w * mult2.w;
    }
//-------------------------------------------

inline Quaternion ToQuaternion (const Matrix& convert)
    {
    Quaternion result = Quaternion ();

    float trace = convert[0][0] + convert[1][1] + convert[2][2];

    if (trace > 0)
        result = Quaternion (Vect (convert[1][2] - convert[2][1],
                                   convert[2][0] - convert[0][2],
                                   convert[0][1] - convert[1][0]), 1.0f + trace);

    else if (convert[0][0] > convert[1][1] && convert[0][0] > convert[2][2])
        result = Quaternion (Vect (1.0f + convert[0][0] - convert[1][1] - convert[2][2],
                                          convert[0][1] + convert[1][0],
                                          convert[0][2] + convert[2][0]),
                                          convert[1][2] - convert[2][1]);

    else if (convert[1][1] > convert[2][2])
        result = Quaternion (Vect (convert[1][0] + convert[0][1],
                            1.0f + convert[1][1] - convert[0][0] - convert[2][2],
                                   convert[1][2] + convert[2][1]),
                                   convert[2][0] - convert[0][2]);

    else
        result = Quaternion (Vect (convert[2][0] + convert[0][2],
                                   convert[2][1] + convert[1][2],
                            1.0f + convert[2][2] - convert[0][0] - convert[1][1]),
                                   convert[0][1] - convert[1][0]);

    return result;
    }

inline Matrix ToMatrix (const Quaternion& convert)
    {
    Matrix result = Matrix();

    float x2, y2, z2,
          xx, xy, xz,
          yy, yz, zz,
          wx, wy, wz;

    float factor = 2.0f / convert.Norm();

    x2 = convert.x * factor;
    y2 = convert.y * factor;
    z2 = convert.z * factor;

    xx = convert.x * x2;
    yy = convert.y * y2;
    zz = convert.z * z2;

    xy = convert.x * y2;
    yz = convert.y * z2;
    xz = convert.x * z2;

    wx = convert.w * x2;
    wy = convert.w * y2;
    wz = convert.w * z2;

    return Matrix (1.0f - (yy + zz),         xy + wz ,         xz - wy,
                           xy - wz , 1.0f - (xx + zz),         yz + wx,
                           xz + wy ,         yz - wx , 1.0f - (xx + yy));
    }

Matrix GetScaleMatrix (float factor)
    {
    Matrix result (factor, 0.0f  , 0.0f  ,
                   0.0f  , factor, 0.0f  ,
                   0.0f  , 0.0f  , factor);

    return result;
    }

Matrix GetTranslateMatrix (const Vect& dist)
    {
    Vect norm = dist.GetSimple ();

    Matrix result (dist.x, dist.y, dist.z);

    return result;
    }

Matrix GetRotateMatrix (const Vect& dir, float ang)
    {
    Quaternion result = Quaternion ();

    result.SetRotate (dir, ang);

    return ToMatrix (result);
    }

Matrix ShortArc (const Vect& from, const Vect& to)
    {
    Quaternion result = Quaternion (from * to, from % to);

    result.Simple();
    result.w += 1.0f;
    result.Simple();

    return ToMatrix (result);
    }

#endif

