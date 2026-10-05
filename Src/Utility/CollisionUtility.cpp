#include "Utility.h"
#include "CollisionUtility.h"

bool CollisionUtility::IsColSphere2Sphere(VECTOR pos1, float radius1, VECTOR pos2, float radius2)
{
    return Utility::Distance(pos1, pos2) <= radius1 + radius2;
}

bool CollisionUtility::IsColSphere2Model(VECTOR pos, float radius, int modelId)
{
    auto col = MV1CollCheck_Sphere(modelId, -1, pos, radius);
    return col.HitNum > 0;
}

bool CollisionUtility::IsColCircumference2Circle(VECTOR pos1, float radius1, VECTOR pos2, float radius2)
{
    float dis = static_cast<float>(Utility::Distance(pos1, pos2));
    return abs(dis - radius1) < radius2;
}

bool CollisionUtility::IsColTriangle2Line(VECTOR tPos1, VECTOR tPos2, VECTOR tPos3, VECTOR lPos1, VECTOR lPos2, VECTOR& hitPos)
{

    VECTOR dir = VSub(lPos1, lPos2);    //線の方向ベクトル
    VECTOR edge1 = VSub(tPos3, tPos1);  //三角形の辺ベクトル1
    VECTOR edge2 = VSub(tPos2, tPos1);  //三角形の辺ベクトル2

    VECTOR h = VCross(dir, edge2);  //平面と線の位置関係を求める補助ベクトル
    float a = VDot(edge1, h);   //三角形と線分の考査判定に必要な係数

    if (fabs(a) < Utility::EPSILON)
    {
        // aが0に近ければ、線分は平面とほぼ平行
        return false;
    }

    float f = 1.0f / a; //逆数を求めて計算を高速化(何度も使うから)

    VECTOR s = VSub(lPos1, tPos1);  //線分視点から三角形の頂点へのベクトル

    float u = f * VDot(s, h);   //三角形の重心座標
    if (u < 0.0f || u > 1.0f)
    {
        //三角形の外側
        return false;
    }

    VECTOR q = VCross(s, edge1);    //三角形内での位置関係補助ベクトル

    float v = f * VDot(dir, q); //三角形のもう一方向の重心座標
    if (v < 0.0f || u + v > 1.0f)
    {
        //三角形の外側
        return false;
    }

    float t = f * VDot(edge2, q);   //線分上で交点までの距離
    if (t < 0.0f || t > 1.0f)
    {
        //線分の範囲外
        return false;
    }

    hitPos = VAdd(lPos1, VScale(dir, t)); //当たった座標の計算
    return true;

}

bool CollisionUtility::IsColCapsule2Line(VECTOR cPos1, VECTOR cPos2, float cRadius, VECTOR lPos1, VECTOR lPos2, VECTOR& hitPos)
{
    // 線分Aの方向ベクトル（P1→P2）
    VECTOR u = VSub(lPos2, lPos1);

    // カプセル中心線の方向ベクトル（Q1→Q2）
    VECTOR v = VSub(cPos2, cPos1);

    // 両線分の始点間ベクトル（P1→Q1）
    VECTOR w = VSub(lPos1, cPos1);

    // a = u・u → 線分Aの長さの二乗
    float a = VDot(u, u);

    // b = u・v → AとBの方向の平行度（角度の関係）
    float b = VDot(u, v);

    // c = v・v → カプセル中心線の長さの二乗
    float c = VDot(v, v);

    // d = u・w → A方向におけるBの始点の位置関係
    float d = VDot(u, w);

    // e = v・w → B方向におけるAの始点の位置関係
    float e = VDot(v, w);

    // D = a*c - b*b → 連立方程式の判別式（平行かどうかの指標）
    float D = a * c - b * b;

    // s, t → 各線分上の最近点パラメータ (0～1)
    float s = (b * e - c * d);
    float t = (a * e - b * d);

    if (D != 0)
    {
        // 通常ケース（非平行）
        s /= D;
        t /= D;
    }
    else
    {
        // 平行もしくはほぼ平行
        s = 0;
        t = e / c;
    }

    // s, t を [0,1] の範囲に制限（線分内に収める）
    if (s < 0)s = 0;
    if (s > 1)s = 1;
    if (t < 0)t = 0;
    if (t > 1)t = 1;

    // 各線分上の最近接点座標
    VECTOR pA = VAdd(lPos1, VScale(u, s));  // 線分A上の最近点
    VECTOR pB = VAdd(cPos1, VScale(v, t));  // カプセル中心線上の最近点

    // 2点間の距離ベクトル
    VECTOR diff = VSub(pA, pB);

    // 最近距離
    double dist = Utility::Magnitude(diff);

    // 衝突判定：最近距離がカプセル半径以下なら当たり
    if (dist > cRadius)
    {
        return false;
    }
    // 接触点計算：中心線から法線方向へ半径分移動した位置
    if (dist > 1e-9)
    {
        hitPos = VAdd(pB, VScale(VNorm(diff), cRadius));
    }
    else
    {
        hitPos = pA; // 完全に重なっている場合
    }
    return true;
}

bool CollisionUtility::IsColCapsule2Sphere(VECTOR cPos1, VECTOR cPos2, float cRadius, VECTOR sPos, float sRadius, VECTOR& hitPos)
{
    VECTOR ab = VSub(cPos2, cPos1); //カプセルの軸方向ベクトル
    VECTOR ac = VSub(sPos, cPos1);  //球からカプセルの方向ベクトル
    float t = VDot(ac, ab) / VDot(ab, ab);  //線分上の最近接位置係数
    t = std::fmax(0.0f, std::fmin(1.0f, t));
    VECTOR closest = VAdd(cPos1, VScale(ab, t));

    VECTOR diff = VSub(sPos, closest);
    float dist = Utility::MagnitudeF(diff);

    float rSum = sRadius + cRadius;

    if (dist <= rSum)
    {
        VECTOR dir = dist > 0 ? VNorm(diff) : VECTOR(0, 1, 0);  //方向
        hitPos = VSub(sPos, VScale(dir, sRadius));              //球の表面上の接触点
        return true;
    }
    return false;
}

bool CollisionUtility::IsColCapsule2Capsule(VECTOR c1Pos1, VECTOR c1Pos2, float c1Radius, VECTOR c2Pos1, VECTOR c2Pos2, float c2Radius, VECTOR& hitPos)
{
    constexpr float EPS = 1e-6f;

    VECTOR d1 = VSub(c1Pos2, c1Pos1);
    VECTOR d2 = VSub(c2Pos2, c2Pos1);
    VECTOR r = VSub(c1Pos1, c2Pos1);

    float a = VDot(d1, d1);
    float e = VDot(d2, d2);
    float b = VDot(d1, d2);
    float c = VDot(d1, r);
    float f = VDot(d2, r);

    float s = 0.0f;
    float t = 0.0f;

    // 両方とも点
    if (a <= EPS && e <= EPS)
    {
        hitPos = VScale(VAdd(c1Pos1, c2Pos1), 0.5f);
        return VSize(VSub(c1Pos1, c2Pos1)) <= (c1Radius + c2Radius);
    }

    // カプセル1が点
    if (a <= EPS)
    {
        s = 0.0f;
        t = std::clamp(f / e, 0.0f, 1.0f);
    }
    // カプセル2が点
    else if (e <= EPS)
    {
        t = 0.0f;
        s = std::clamp(-c / a, 0.0f, 1.0f);
    }
    else
    {
        float denom = a * e - b * b;

        if (fabsf(denom) > EPS)
        {
            s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
        }

        t = (b * s + f) / e;

        if (t < 0.0f)
        {
            t = 0.0f;
            s = std::clamp(-c / a, 0.0f, 1.0f);
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
            s = std::clamp((b - c) / a, 0.0f, 1.0f);
        }
    }

    // 最近接点
    VECTOR p1 = VAdd(c1Pos1, VScale(d1, s));
    VECTOR p2 = VAdd(c2Pos1, VScale(d2, t));

    // 衝突位置(中点)
    hitPos = VScale(VAdd(p1, p2), 0.5f);

    float distSq = VSquareSize(VSub(p1, p2));
    float radius = c1Radius + c2Radius;

    return distSq <= radius * radius;
}

bool CollisionUtility::IsColSphere2Triangle(VECTOR sPos, float radius, VECTOR tPos1, VECTOR tPos2, VECTOR tPos3, VECTOR& hitPos)
{
    VECTOR n = VCross(VSub(tPos2, tPos1), VSub(tPos3, tPos1));
    float len = Utility::MagnitudeF(n);
    if (len < Utility::EPSILON)
    {
        return false;
    }
    n = VScale(n, 1.0f / len);

    float dist = VDot(n, VSub(sPos, tPos1));
    if (fabs(dist) > radius)
    {
        //平面から遠い
        return false;
    }

    VECTOR p = VSub(sPos, VScale(n, dist));

    VECTOR c0 = VCross(VSub(tPos2, tPos1), VSub(p, tPos1));
    VECTOR c1 = VCross(VSub(tPos3, tPos2), VSub(p, tPos2));
    VECTOR c2 = VCross(VSub(tPos1, tPos3), VSub(p, tPos3));
    if (VDot(c0, n) >= 0 && VDot(c1, n) >= 0 && VDot(c2, n) >= 0)
    {
        hitPos = p;
        return true;
    }

    // 各辺との最近点を確認
    VECTOR cp1 = Utility::ClosestPointOnSegment(sPos, tPos1, tPos2);
    VECTOR cp2 = Utility::ClosestPointOnSegment(sPos, tPos2, tPos3);
    VECTOR cp3 = Utility::ClosestPointOnSegment(sPos, tPos3, tPos1);

    float d1 = Utility::MagnitudeF(VSub(sPos, cp1));
    float d2 = Utility::MagnitudeF(VSub(sPos, cp2));
    float d3 = Utility::MagnitudeF(VSub(sPos, cp3));

    hitPos = cp1;
    float minD = d1;
    if (d2 < minD)
    {
        hitPos = cp2; minD = d2;
    }
    if (d3 < minD)
    {
        hitPos = cp3; minD = d3;
    }

    return minD <= radius;
}

bool CollisionUtility::IsColSphere2Sphere(VECTOR s1Pos, float radius1, VECTOR s2Pos, float radius2, VECTOR& hitPos)
{
    VECTOR dir = VNorm(VSub(s1Pos, s2Pos));
    float distance = static_cast<float>(Utility::Distance(s1Pos, s2Pos));
    if (distance > radius1 + radius2)
    {
        return false;
    }
    hitPos = VAdd(s1Pos, VScale(dir, radius1 - (radius1 + radius2 - distance) / 2));
    return true;
}

bool CollisionUtility::IsColCylinder2Cylinder(VECTOR c1Pos, float radius1, VECTOR c2Pos, float radius2, VECTOR& hitPos)
{
    VECTOR dir = VNorm(VSub(c1Pos, c2Pos));
    c1Pos.y = 0.0f;
    c2Pos.y = 0.0f;
    float distance = static_cast<float>(Utility::Distance(c1Pos, c2Pos));
    if (distance > radius1 + radius2)
    {
        return false;
    }
    hitPos = VAdd(c1Pos, VScale(dir, radius1 - (radius1 + radius2 - distance) / 2));
    return true;
}

bool CollisionUtility::IsColCircumference2Circle(VECTOR pos1, float radius1, VECTOR pos2, float radius2, VECTOR& hitPos)
{
    VECTOR dir = VNorm(VSub(pos2, pos1));
    pos1.y = 0.0f;
    pos2.y = 0.0f;
    float dis = static_cast<float>(Utility::Distance(pos1, pos2));
    bool ret = abs(dis - radius1) < radius2;
    if (!ret)
    {
        return ret;
    }
    hitPos = VAdd(pos1, VScale(dir, radius1));
    return ret;
}

bool CollisionUtility::IsColRect2Point(IntVector2 leftTop, IntVector2 rightBottom, IntVector2 pointPos)
{
    if (leftTop.x > pointPos.x || rightBottom.x < pointPos.x || leftTop.y > pointPos.y || rightBottom.y < pointPos.y)
    {
        return false;
    }
    return true;
}
