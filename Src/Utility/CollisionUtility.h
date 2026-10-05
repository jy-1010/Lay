#pragma once
#include <DxLib.h>

class CollisionUtility
{
public:
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="pos1">1つ目の球の中心座標</param>
	/// <param name="radius1">１つ目の球の半径</param>
	/// <param name="pos2">2つ目の球の中心座標</param>
	/// <param name="radius2">２つ目の球の半径</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Sphere(VECTOR pos1, float radius1, VECTOR pos2, float radius2);

	/// <summary>
	/// 球とモデルの当たり判定
	/// </summary>
	/// <param name="pos">球の中心座標</param>
	/// <param name="radius">球の半径</param>
	/// <param name="modelId">モデルのハンドル</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Model(VECTOR pos, float radius, int modelId);

	/// <summary>
	/// 円周と円の当たり判定
	/// </summary>
	/// <param name="pos1">円周の中心座標</param>
	/// <param name="radius1">円周の半径</param>
	/// <param name="pos2">円の中心座標</param>
	/// <param name="radius2">円の半径</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCircumference2Circle(VECTOR pos1, float radius1, VECTOR pos2, float radius2);

	/// <summary>
	/// 線と三角形の当たり判定(AI産のため理論は不明)
	/// </summary>
	/// <param name="tPos1">三角形の座標1</param>
	/// <param name="tPos2">三角形の座標2</param>
	/// <param name="tPos3">三角形の座標3</param>
	/// <param name="lPos1">線の座標1</param>
	/// <param name="lPos2">線の座標2</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColTriangle2Line(VECTOR tPos1, VECTOR tPos2, VECTOR tPos3, VECTOR lPos1, VECTOR lPos2, VECTOR& hitPos);

	/// <summary>
	/// 線とカプセルの当たり判定(AI産のため理論は不明)
	/// </summary>
	/// <param name="cPos1">カプセルの座標１</param>
	/// <param name="cPos2">カプセルの座標２</param>
	/// <param name="cRadius">カプセルの半径</param>
	/// <param name="lPos1">線の座標１</param>
	/// <param name="lPos2">線の座標２</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCapsule2Line(VECTOR cPos1, VECTOR cPos2, float cRadius, VECTOR lPos1, VECTOR lPos2, VECTOR& hitPos);

	/// <summary>
	/// カプセルと球の当たり判定(Ai)
	/// </summary>
	/// <param name="cPos1">カプセル座標１</param>
	/// <param name="cPos2">カプセル座標２</param>
	/// <param name="cRadius">カプセルの半径</param>
	/// <param name="sPos">球の座標</param>
	/// <param name="sRadius">球の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCapsule2Sphere(VECTOR cPos1, VECTOR cPos2, float cRadius, VECTOR sPos, float sRadius, VECTOR& hitPos);

	/// <summary>
	/// カプセルとカプセルの当たり判定
	/// </summary>
	/// <param name="c1Pos1">カプセル１の座標１</param>
	/// <param name="c1Pos2">カプセル１の座標２</param>
	/// <param name="c1Radius">カプセル１の半径</param>
	/// <param name="c2Pos1">カプセル２の座標１</param>
	/// <param name="c2Pos2">カプセル２の座標２</param>
	/// <param name="c2Radius">カプセル２の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCapsule2Capsule(VECTOR c1Pos1, VECTOR c1Pos2, float c1Radius, VECTOR c2Pos1, VECTOR c2Pos2, float c2Radius, VECTOR& hitPos);

	/// <summary>
	/// 球と三角形の当たり判定
	/// </summary>
	/// <param name="sPos">球の座標</param>
	/// <param name="radius">球の半径</param>
	/// <param name="tPos1">三角形の座標１</param>
	/// <param name="tPos2">三角形の座標２</param>
	/// <param name="tPos3">三角形の座標３</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Triangle(VECTOR sPos, float radius, VECTOR tPos1, VECTOR tPos2, VECTOR tPos3, VECTOR& hitPos);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="s1Pos">球１の座標</param>
	/// <param name="radius1">球１の半径</param>
	/// <param name="s2Pos">球２の座標</param>
	/// <param name="radius2">球２の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Sphere(VECTOR s1Pos, float radius1, VECTOR s2Pos, float radius2, VECTOR& hitPos);

	/// <summary>
	/// 円柱と円柱の当たり判定
	/// </summary>
	/// <param name="c1Pos">円柱１の座標</param>
	/// <param name="radius1">円柱１の半径</param>
	/// <param name="c2Pos">円柱２の座標</param>
	/// <param name="radius2">円柱２の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCylinder2Cylinder(VECTOR c1Pos, float radius1, VECTOR c2Pos, float radius2, VECTOR& hitPos);

	/// <summary>
	/// 円周と円の当たり判定
	/// </summary>
	/// <param name="pos1">円周の中心座標</param>
	/// <param name="radius1">円周の半径</param>
	/// <param name="pos2">円の中心座標</param>
	/// <param name="radius2">円の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCircumference2Circle(VECTOR pos1, float radius1, VECTOR pos2, float radius2, VECTOR& hitPos);

	/// <summary>
	/// 四角形と点の当たり判定
	/// </summary>
	/// <param name="leftTop">四角形の左上座標</param>
	/// <param name="rightBottom">四角形の右下座標</param>
	/// <param name="pointPos">ポイント座標</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColRect2Point(IntVector2 leftTop, IntVector2 rightBottom, IntVector2 pointPos);
};

