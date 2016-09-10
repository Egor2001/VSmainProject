#ifndef MATH3D_H_INCLUDED
#define MATH3D_H_INCLUDED

#include "../stdafx.h"

struct EGLvector
    {
    float x, y, z, p;

    EGLvector ();
    EGLvector (float x_set, float y_set, float z_set);
    EGLvector (float x_set, float y_set, float z_set, float p_set);

    ~EGLvector ();

    inline  EGLvector  operator - () const;
    inline  EGLvector  operator ! () const;

    inline float        Length() const;
    inline float       Length2() const;
    inline void         Simple();
    inline EGLvector GetSimple() const;
    };

inline bool       operator == (const EGLvector&, const EGLvector&);
inline void       operator *= (      EGLvector&,            float);
inline EGLvector  operator *  (const EGLvector&,            float);
inline EGLvector  operator *  (           float, const EGLvector&);
inline void       operator /= (      EGLvector&,            float);
inline EGLvector  operator /  (const EGLvector&,            float);
inline void       operator += (      EGLvector&, const EGLvector&);
inline EGLvector  operator +  (const EGLvector&, const EGLvector&);
inline void       operator -= (      EGLvector&, const EGLvector&);
inline EGLvector  operator -  (const EGLvector&, const EGLvector&);
inline float      operator %  (const EGLvector&, const EGLvector&);
inline void       operator *= (      EGLvector&, const EGLvector&);
inline EGLvector  operator *  (const EGLvector&, const EGLvector&);

EGLvector::EGLvector () : x (0), y (0), z (0), p (1) {}

EGLvector::EGLvector (float x_set, float y_set, float z_set) :
    x (x_set), y (y_set), z (z_set), p (1) {}

EGLvector::EGLvector (float x_set, float y_set, float z_set, float p_set) :
    x (x_set), y (y_set), z (z_set), p (p_set) {}

EGLvector::~EGLvector () {}

EGLvector EGLvector::operator - () const
{
    return EGLvector (-x, -y, -z, p);
}

EGLvector EGLvector::operator ! () const
{
    float len = Length();

    if (len == 0.0f) return EGLvector ();

    return (*this)/len;
}

float EGLvector::Length() const
{
    return sqrtf(x*x + y*y + z*z) / p;
}

float EGLvector::Length2() const
{
    return (x*x + y*y + z*z)/(p*p);
}

void EGLvector::Simple()
{
    x /= p;
    y /= p;
    z /= p;
    p = 1;
}

EGLvector EGLvector::GetSimple() const
{
    EGLvector norm = (*this);

    norm.Simple ();

    return norm;
}

struct EGLmatrix
{
    float matrix [4][4];

    EGLmatrix ();

    EGLmatrix (float Set00, float Set01, float Set02, float Set03,
               float Set10, float Set11, float Set12, float Set13,
               float Set20, float Set21, float Set22, float Set23,
               float Set30, float Set31, float Set32, float Set33);

    EGLmatrix (float Set00, float Set01, float Set02,
               float Set10, float Set11, float Set12,
               float Set20, float Set21, float Set22);

    EGLmatrix (float Set03, float Set13, float Set23);

    ~EGLmatrix ();

    inline const float* operator[] (int num) const;
    inline       float* operator[] (int num);

    inline void Inv_Direct  (EGLmatrix& change, EGLmatrix& result) const;
    inline void Inv_Reverse (EGLmatrix& change, EGLmatrix& result) const;

    inline EGLmatrix operator ~ () const;
};

inline void      operator *= (      EGLmatrix&,            float);
inline EGLmatrix operator *  (const EGLmatrix&,            float);
inline EGLmatrix operator *  (           float, const EGLmatrix&);
inline void      operator /= (      EGLmatrix&,            float);
inline EGLmatrix operator /  (const EGLmatrix&,            float);
inline void      operator *= (      EGLmatrix&, const EGLmatrix&);
inline EGLmatrix operator *  (const EGLmatrix&, const EGLmatrix&);
inline void      operator /= (      EGLmatrix&, const EGLmatrix&);
inline EGLmatrix operator /  (const EGLmatrix&, const EGLmatrix&);
inline void      operator *= (      EGLvector&, const EGLmatrix&);
inline EGLvector operator *  (const EGLvector&, const EGLmatrix&);
inline void      operator /= (      EGLvector&, const EGLmatrix&);
inline EGLvector operator /  (const EGLvector&, const EGLmatrix&);
inline EGLvector operator *  (const EGLmatrix&, const EGLvector&);

EGLmatrix::EGLmatrix()
{
    memset (matrix, 0, sizeof (matrix));

    for (int i = 0; i < 4; i++)
        matrix [i][i] = 1.0;
}

EGLmatrix::EGLmatrix (float Set00, float Set01, float Set02, float Set03,
                      float Set10, float Set11, float Set12, float Set13,
                      float Set20, float Set21, float Set22, float Set23,
                      float Set30, float Set31, float Set32, float Set33)
{
    matrix [0][0] = Set00; matrix [0][1] = Set01; matrix [0][2] = Set02; matrix [0][3] = Set03;
    matrix [1][0] = Set10; matrix [1][1] = Set11; matrix [1][2] = Set12; matrix [1][3] = Set13;
    matrix [2][0] = Set20; matrix [2][1] = Set21; matrix [2][2] = Set22; matrix [2][3] = Set23;
    matrix [3][0] = Set30; matrix [3][1] = Set31; matrix [3][2] = Set32; matrix [3][3] = Set33;
}

EGLmatrix::EGLmatrix (float Set00, float Set01, float Set02,
                      float Set10, float Set11, float Set12,
                      float Set20, float Set21, float Set22)
{
    matrix [0][0] = Set00; matrix [0][1] = Set01; matrix [0][2] = Set02; matrix [0][3] = 0.0f;
    matrix [1][0] = Set10; matrix [1][1] = Set11; matrix [1][2] = Set12; matrix [1][3] = 0.0f;
    matrix [2][0] = Set20; matrix [2][1] = Set21; matrix [2][2] = Set22; matrix [2][3] = 0.0f;
    matrix [3][0] = 0.0f ; matrix [3][1] = 0.0f ; matrix [3][2] = 0.0f ; matrix [3][3] = 1.0f;
}

EGLmatrix::EGLmatrix (float Set03, float Set13, float Set23)
{
    matrix [0][0] = 1.0f; matrix [0][1] = 0.0f; matrix [0][2] = 0.0f; matrix [0][3] = Set03;
    matrix [1][0] = 0.0f; matrix [1][1] = 1.0f; matrix [1][2] = 0.0f; matrix [1][3] = Set13;
    matrix [2][0] = 0.0f; matrix [2][1] = 0.0f; matrix [2][2] = 1.0f; matrix [2][3] = Set23;
    matrix [3][0] = 0.0f; matrix [3][1] = 0.0f; matrix [3][2] = 0.0f; matrix [3][3] = 1.0f ;
}

EGLmatrix::~EGLmatrix () {}

const float* EGLmatrix::operator[] (int num) const
{
    return & (matrix[num][0]);
}

float* EGLmatrix::operator[] (int num)
{
    return & (matrix[num][0]);
}

EGLmatrix EGLmatrix::operator ~ () const
{
    EGLmatrix result = EGLmatrix ();
    EGLmatrix change = EGLmatrix ();

    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
        change[i][j] = matrix[i][j];

    Inv_Direct  (change, result);
    Inv_Reverse (change, result);

    return result;
}

void EGLmatrix::Inv_Direct (EGLmatrix& change, EGLmatrix& result) const
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

void EGLmatrix::Inv_Reverse (EGLmatrix& change, EGLmatrix& result) const
{
    float factor = 0.0f;

    for (int i = 3; i >= 0; i--)
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

struct EGLquaternion
{
    float x, y, z, w;

    EGLquaternion ();
    EGLquaternion (const EGLvector& v_set, float w_set);
    EGLquaternion (float x_set, float y_set, float z_set, float w_set);
   ~EGLquaternion ();

    EGLquaternion operator - () const;

    void SetRotate (const EGLvector& set_v, float ang);

    inline float      Norm() const;
    inline float Magnitude() const;

    inline EGLquaternion Conjugated() const;
    inline EGLquaternion   Inverted() const;

    inline void          Simple();
    inline EGLquaternion GetSimple() const;
};

inline void          operator *= (      EGLquaternion&,                float);
inline EGLquaternion operator *  (const EGLquaternion&,                float);
inline EGLquaternion operator *  (               float, const EGLquaternion&);
inline void          operator /= (      EGLquaternion&,                float);
inline EGLquaternion operator /  (const EGLquaternion&,                float);
inline void          operator += (      EGLquaternion&, const EGLquaternion&);
inline EGLquaternion operator +  (const EGLquaternion&, const EGLquaternion&);
inline void          operator -= (      EGLquaternion&, const EGLquaternion&);
inline EGLquaternion operator -  (const EGLquaternion&, const EGLquaternion&);
inline void          operator *= (      EGLquaternion&, const EGLquaternion&);
inline EGLquaternion operator *  (const EGLquaternion&, const EGLquaternion&);
inline void          operator /= (      EGLquaternion&, const EGLquaternion&);
inline EGLquaternion operator /  (const EGLquaternion&, const EGLquaternion&);
inline void          operator *= (          EGLvector&, const EGLquaternion&);
inline EGLvector     operator *  (const     EGLvector&, const EGLquaternion&);
inline void          operator /= (          EGLvector&, const EGLquaternion&);
inline EGLvector     operator /  (const     EGLvector&, const EGLquaternion&);
inline EGLvector     operator *  (const EGLquaternion&, const     EGLvector&);
inline float         operator %  (const EGLquaternion&, const EGLquaternion&);

inline EGLquaternion eglToQuaternion (const     EGLmatrix& convert);
inline     EGLmatrix eglToMatrix     (const EGLquaternion& convert);

EGLmatrix eglGetScaleMatrix     (float factor);
EGLmatrix eglGetTranslateMatrix (const EGLvector& dist);
EGLmatrix eglGetRotateMatrix    (const EGLvector& dir  , float ang);
EGLmatrix eglShortArc           (const EGLvector& from , const EGLvector& to);

EGLquaternion::EGLquaternion() :
    x (0.0f), y (0.0f), z (0.0f), w (0.0f) {}

EGLquaternion::EGLquaternion (const EGLvector& v_set, float w_set) :
    x (v_set.x / v_set.p), y (v_set.y / v_set.p), z (v_set.z / v_set.p), w (w_set) {}

EGLquaternion::EGLquaternion (float x_set, float y_set, float z_set, float w_set) :
    x (x_set), y (y_set), z (z_set), w (w_set) {}

EGLquaternion::~EGLquaternion() {}

EGLquaternion EGLquaternion::operator - () const
{
    return EGLquaternion (-x, -y, -z, -w);
}

void EGLquaternion::SetRotate (const EGLvector& set_v, float ang)
{
    EGLvector simp = (!set_v * sin (ang / 2)).GetSimple();

    x = simp.x;
    y = simp.y;
    z = simp.z;

    w = cos (ang / 2);
}

float EGLquaternion::Norm() const
{
    return x * x + y * y + z * z + w * w;
}

float EGLquaternion::Magnitude() const
{
    return sqrtf (Norm());
}

EGLquaternion EGLquaternion::Conjugated() const
{
    return EGLquaternion (-x, -y, -z, w);
}

EGLquaternion EGLquaternion::Inverted() const
{
    return EGLquaternion (Conjugated() / Norm());
}

void EGLquaternion::Simple()
{
    float magn = Magnitude();

    if (magn == 0.0f) (*this) = EGLquaternion (0, 0, 0, 1);

    x /= magn;
    y /= magn;
    z /= magn;
    w /= magn;
}

EGLquaternion EGLquaternion::GetSimple() const
{
    EGLquaternion norm = (*this);

    norm.Simple();

    return norm;
}
//============================================
bool operator == (const EGLvector& v1, const EGLvector& v2)
{
    EGLvector comp1 = v1.GetSimple (),
         comp2 = v2.GetSimple ();

    return !((comp1.x != comp2.x) && (comp1.y != comp2.y) && (comp1.z != comp2.z));
}

void operator *= (EGLvector& vec, float mul)
{
    if (mul == 0.0f)
    {
        vec = EGLvector ();

        return;
    }

    vec.p /= mul;
}

EGLvector operator * (const EGLvector& vec, float mul)
{
    EGLvector result = vec;

    result *= mul;

    return result;
}

EGLvector operator * (float mul, const EGLvector& vec)
{
    return vec * mul;
}

void operator /= (EGLvector& vec, float mul)
{
    vec.p *= mul;
}

EGLvector operator / (const EGLvector& vec, float mul)
{
    EGLvector result = vec;

    result /= mul;

    return result;
}

void operator += (EGLvector& vec1, const EGLvector& vec2)
{
                     vec1.   Simple();
    EGLvector sum2 = vec2.GetSimple();

    vec1.x += sum2.x;
    vec1.y += sum2.y;
    vec1.z += sum2.z;
}

EGLvector operator + (const EGLvector& vec1, const EGLvector& vec2)
{
    EGLvector sum1  = vec1;
    sum1 += vec2;

    return sum1;
}

void operator -= (EGLvector& vec1, const EGLvector& vec2)
{
                     vec1.   Simple();
    EGLvector sum2 = vec2.GetSimple();

    vec1.x -= sum2.x;
    vec1.y -= sum2.y;
    vec1.z -= sum2.z;
}

EGLvector operator - (const EGLvector& vec1, const EGLvector& vec2)
{
    EGLvector sum1  = vec1;
    sum1 -= vec2;

    return sum1;
}

float operator % (const EGLvector& vec1, const EGLvector& vec2)
{
    return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z) / (vec1.p * vec2.p);
}

void operator *= (EGLvector& vec1, const EGLvector& vec2)
{
    EGLvector mul1 = vec1;

    vec1.x = (mul1.y * vec2.z) - (mul1.z * vec2.y);
    vec1.y = (mul1.z * vec2.x) - (mul1.x * vec2.z);
    vec1.z = (mul1.x * vec2.y) - (mul1.y * vec2.x);
    vec1.p = (mul1.p * vec2.p);
}

EGLvector operator * (const EGLvector& vec1, const EGLvector& vec2)
{
    EGLvector mul1 = vec1;

    mul1 *= vec2;

    return mul1;
}
//-----------------------------------------------
void operator *= (EGLmatrix& mat, float mult)
{
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
        mat[i][j] *= mult;
}

EGLmatrix operator * (const EGLmatrix& mat, float mult)
{
    EGLmatrix result = mat;

    result *= mult;

    return result;
}

EGLmatrix operator * (float mult, const EGLmatrix& mat)
{
    return mat * mult;
}

void operator /= (EGLmatrix& mat, float mult)
    {
    for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
        mat[i][j] /= mult;
}

EGLmatrix operator / (const EGLmatrix& mat, float mult)
{
    EGLmatrix result = mat;

    result /= mult;

    return result;
}

void operator *= (EGLmatrix& mult1, const EGLmatrix& mult2)
{
    EGLmatrix result;

    memset (result.matrix, 0, sizeof (result.matrix));

    for (int a = 0; a < 4; a++)
    for (int b = 0; b < 4; b++)
        for (int c = 0; c < 4; c++)
            result.matrix[a][b] += mult1.matrix[a][c] * mult2.matrix[c][b];

    mult1 = result;
}

EGLmatrix operator * (const EGLmatrix& mult1, const EGLmatrix& mult2)
{
    EGLmatrix result = mult1;

    result *= mult2;

    return result;
}

void operator /= (EGLmatrix& mult1, const EGLmatrix& mult2)
{
    mult1 *= ~mult2;
}

EGLmatrix operator / (const EGLmatrix& mult1, const EGLmatrix& mult2)
{
    return mult1 * ~mult2;
}

void operator *= (EGLvector& vec, const EGLmatrix& mat)
{
    EGLvector mult = vec;

    vec.x = mat.matrix[0][0] * mult.x + mat.matrix[0][1] * mult.y +
            mat.matrix[0][2] * mult.z + mat.matrix[0][3] * mult.p;

    vec.y = mat.matrix[1][0] * mult.x + mat.matrix[1][1] * mult.y +
            mat.matrix[1][2] * mult.z + mat.matrix[1][3] * mult.p;

    vec.z = mat.matrix[2][0] * mult.x + mat.matrix[2][1] * mult.y +
            mat.matrix[2][2] * mult.z + mat.matrix[2][3] * mult.p;

    vec.p = mat.matrix[3][0] * mult.x + mat.matrix[3][1] * mult.y +
            mat.matrix[3][2] * mult.z + mat.matrix[3][3] * mult.p;
}

EGLvector operator * (const EGLvector& vec, const EGLmatrix& mat)
{
    EGLvector result = vec;

    result *= mat;

    return result;
}

void operator /= (EGLvector& vec, const EGLmatrix& mat)
{
    vec *= ~mat;
}

EGLvector operator / (const EGLvector& vec, const EGLmatrix& mat)
{
    return vec * ~mat;
}

EGLvector operator * (const EGLmatrix& mat, const EGLvector& vec)
{
    return vec * mat;
}
//----------------------------------------
void operator *= (EGLquaternion& quat, float mult)
{
    quat.x *= mult;
    quat.y *= mult;
    quat.z *= mult;
    quat.w *= mult;
}

EGLquaternion operator * (const EGLquaternion& quat, float mult)
{
    return EGLquaternion (quat.x * mult, quat.y * mult, quat.z * mult, quat.w * mult);
}

EGLquaternion operator * (float mult, const EGLquaternion& quat)
{
    return quat * mult;
}

void operator /= (EGLquaternion& quat, float mult)
{
    quat.x /= mult;
    quat.y /= mult;
    quat.z /= mult;
    quat.w /= mult;
}

EGLquaternion operator / (const EGLquaternion& quat, float mult)
{
    return EGLquaternion (quat.x / mult, quat.y / mult, quat.z / mult, quat.w / mult);
}

void operator += (EGLquaternion& add1, const EGLquaternion& add2)
{
    add1.x += add2.x;
    add1.y += add2.y;
    add1.z += add2.z;
    add1.w += add2.w;
}

EGLquaternion operator + (const EGLquaternion& add1, const EGLquaternion& add2)
{
    return EGLquaternion (add1.x + add2.x, add1.y + add2.y, add1.z + add2.z, add1.w + add2.w);
}

void operator -= (EGLquaternion& add1, const EGLquaternion& add2)
{
    add1.x -= add2.x;
    add1.y -= add2.y;
    add1.z -= add2.z;
    add1.w -= add2.w;
}

EGLquaternion operator - (const EGLquaternion& add1, const EGLquaternion& add2)
{
    return EGLquaternion (add1.x - add2.x, add1.y - add2.y, add1.z - add2.z, add1.w - add2.w);
}

void operator *= (EGLquaternion& mult1, const EGLquaternion& mult2)
{
    mult1 = EGLquaternion ( (mult1.y * mult2.z) - (mult1.z * mult2.y) + (mult2.x * mult1.w) + (mult1.x * mult2.w),
                         (mult1.z * mult2.x) - (mult1.x * mult2.z) + (mult2.y * mult1.w) + (mult1.y * mult2.w),
                         (mult1.x * mult2.y) - (mult1.y * mult2.x) + (mult2.z * mult1.w) + (mult1.z * mult2.w),
                         (mult1.w * mult2.w) - (mult1.x * mult2.x) - (mult1.y * mult2.y) - (mult1.z * mult2.z));
}

EGLquaternion operator * (const EGLquaternion& mult1, const EGLquaternion& mult2)
{
    return EGLquaternion ((mult1.y * mult2.z) - (mult1.z * mult2.y) + (mult2.x * mult1.w) + (mult1.x * mult2.w),
                       (mult1.z * mult2.x) - (mult1.x * mult2.z) + (mult2.y * mult1.w) + (mult1.y * mult2.w),
                       (mult1.x * mult2.y) - (mult1.y * mult2.x) + (mult2.z * mult1.w) + (mult1.z * mult2.w),
                       (mult1.w * mult2.w) - (mult1.x * mult2.x) - (mult1.y * mult2.y) - (mult1.z * mult2.z));
}

void operator /= (EGLquaternion& mult1, const EGLquaternion& mult2)
{
    mult1 *= mult2.Inverted ();
}

EGLquaternion operator / (const EGLquaternion& mult1, const EGLquaternion& mult2)
{
    return mult1 * mult2.Inverted ();
}

void operator *= (EGLvector& vec, const EGLquaternion& quat)
{
    EGLquaternion res = (quat * EGLquaternion (vec, 0) * quat.Inverted ());

    vec = EGLvector (res.x, res.y, res.z);
}

EGLvector operator * (const EGLvector& vec, const EGLquaternion& quat)
{
    EGLquaternion res = (quat * EGLquaternion (vec, 0) * quat.Inverted ());

    return EGLvector (res.x, res.y, res.z);
}

void operator /= (EGLvector& vec, const EGLquaternion& quat)
{
    vec *= quat.Inverted ();
}

EGLvector operator / (const EGLvector& vec, const EGLquaternion& quat)
{
    return vec * quat.Inverted ();
}

EGLvector operator * (const EGLquaternion& quat, const EGLvector& vec)
{
    return vec * quat;
}

float operator % (const EGLquaternion& mult1, const EGLquaternion& mult2)
{
    return mult1.x * mult2.x + mult1.y * mult2.y + mult1.z * mult2.z + mult1.w * mult2.w;
}
//-------------------------------------------

inline EGLquaternion eglToQuaternion (const EGLmatrix& convert)
{
    EGLquaternion result = EGLquaternion ();

    float trace = convert[0][0] + convert[1][1] + convert[2][2];

    if (trace > 0)
        result = EGLquaternion (EGLvector (convert[1][2] - convert[2][1],
                                           convert[2][0] - convert[0][2],
                                           convert[0][1] - convert[1][0]), 1.0f + trace);

    else if (convert[0][0] > convert[1][1] && convert[0][0] > convert[2][2])
        result = EGLquaternion (EGLvector (1.0f + convert[0][0] - convert[1][1] - convert[2][2],
                                                  convert[0][1] + convert[1][0],
                                                  convert[0][2] + convert[2][0]),
                                                  convert[1][2] - convert[2][1]);

    else if (convert[1][1] > convert[2][2])
        result = EGLquaternion (EGLvector (convert[1][0] + convert[0][1],
                                    1.0f + convert[1][1] - convert[0][0] - convert[2][2],
                                           convert[1][2] + convert[2][1]),
                                           convert[2][0] - convert[0][2]);

    else
        result = EGLquaternion (EGLvector (convert[2][0] + convert[0][2],
                                           convert[2][1] + convert[1][2],
                                    1.0f + convert[2][2] - convert[0][0] - convert[1][1]),
                                           convert[0][1] - convert[1][0]);

    return result;
}

inline EGLmatrix eglToMatrix (const EGLquaternion& convert)
{
    EGLmatrix result = EGLmatrix();

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

    return EGLmatrix (1.0f - (yy + zz),         xy + wz ,         xz - wy,
                              xy - wz , 1.0f - (xx + zz),         yz + wx,
                              xz + wy ,         yz - wx , 1.0f - (xx + yy));
}

EGLmatrix eglGetScaleMatrix (float factor)
{
    EGLmatrix result (factor, 0.0f  , 0.0f  ,
                      0.0f  , factor, 0.0f  ,
                      0.0f  , 0.0f  , factor);

    return result;
}

EGLmatrix eglGetTranslateMatrix (const EGLvector& dist)
{
    EGLvector norm = dist.GetSimple ();

    EGLmatrix result (dist.x, dist.y, dist.z);

    return result;
}

EGLmatrix eglGetRotateMatrix (const EGLvector& dir, float ang)
{
    EGLquaternion result = EGLquaternion ();

    result.SetRotate (dir, ang);

    return eglToMatrix (result);
}

EGLmatrix eglShortArc (const EGLvector& from, const EGLvector& to)
{
    EGLquaternion result = EGLquaternion (from * to, from % to);

    result.Simple();
    result.w += 1.0f;
    result.Simple();

    return eglToMatrix (result);
}

#endif

