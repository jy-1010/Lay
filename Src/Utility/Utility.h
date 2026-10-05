#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "../Common/Vector.h"
#include "../Common/Quaternion.h"
#include "../Manager/KeyConfig.h"
#include "../Object/Common/Transform.h"

class Utility
{

public:

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	static constexpr float ONE_TRACK_DEG = 360.0f; //1周分の角度

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XZ = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XY = { 1.0f, 1.0f, 1.0f };
	static constexpr VECTOR AXIS_YZ = { 0.0f, 1.0f, 1.0f };
	static constexpr VECTOR AXIS_XYZ = { 1.0f, 1.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//値を反転させる溜めのスケール
	static constexpr float REVERSE_SCALE = -1.0f;

	//アルファ最大値
	static constexpr int ALPHA_MAX = 255;

	static constexpr float EPSILON = 1e-6f;    //浮動小数誤差対策の極小値

	// 四捨五入
	static int Round(float v);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static IntVector2 Lerp(const IntVector2& start, const IntVector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// ベジェ曲線
	static IntVector2 Bezier(const IntVector2& p1, const IntVector2& p2, const IntVector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	
	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const IntVector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const IntVector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const IntVector2& v1, const IntVector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius, 
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// 正規化
	static VECTOR Normalize(const IntVector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	//待機時間
	static bool IsTimeOver(float& totalTime, const float& waitTime);

	/// <summary>
	/// とある点からとある点までの移動ベクトルを返す
	/// </summary>
	/// <param name="_start">狙う側</param>
	/// <param name="_goal">向かう先</param>
	/// <param name="_speed">設定速度(未設定だと、方向ベクトルのみを返す)</param>
	/// <returns>向かう先までの移動ベクトル</returns>
	static const VECTOR GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed = 1.0f);

	/// <summary>
	/// 対象に向かう回転軸
	/// </summary>
	/// <param name="_pos">自身の座標</param>
	/// <param name="_targetPos">対象の座標</param>
	/// <param name="_needAxis">必要な回転軸</param>
	/// <returns>回転軸</returns>
	static const VECTOR GetRotAxisToTarget(const VECTOR _pos, const VECTOR _targetPos, const VECTOR _needAxis = AXIS_XYZ);

	/// <summary>
	/// 対象に向く
	/// </summary>
	/// <param name="_trans">回転させるモデルの情報</param>
	/// <param name="_toGoalAxis">対象までの回転軸</param>
	/// <param name="_time">補正完了までの全体時間(指定がないなら補正なし)</param>
	/// <param name="_relativePos">モデル自体の相対座標</param>
	static void LookAtTarget(Transform& _trans, const VECTOR _toTargetAxis, const float _time = 0.0f, const VECTOR _relativePos = VECTOR_ZERO);

	/// <summary>
	/// 特定の範囲内に座標が含まれてるか調べる
	/// </summary>
	/// <param name="pos">調べたい座標</param>
	/// <param name="leftTop">範囲左上</param>
	/// <param name="rightBotm">範囲右下</param>
	/// <returns>判定結果を返す</returns>
	static bool IsPointInRect(const IntVector2& _pos, const IntVector2& _leftTop, const IntVector2& _rightBotm);

	/// <summary>
	/// 特定の円の範囲内に座標が含まれてるか調べる
	/// </summary>
	/// <param name="_pos"></param>
	/// <param name="_circlePos"></param>
	/// <param name="_radius"></param>
	/// <returns></returns>
	static bool IsPointInRectCircle(const IntVector2& _pos, const IntVector2& _circlePos, const float _radius);

	//符号を返す
	static int GetSign(float f);
	static int GetSign(int f);

	//エクスプローラーからファイルを開く
	static std::string OpenFileDialog();

	//数値の反転
	static VECTOR ReverseValue(const VECTOR _vec);
	static  int ReverseValue(const int _i);
	static float ReverseValue(const float _f);
	static IntVector3 ReverseValue(const IntVector3 _iv);

	/// <summary>
	/// 範囲外の値を指定した範囲に収める
	/// </summary>
	/// <param name="index"></param>収めたい値
	/// <param name="max"></param>範囲の最大値
	/// <returns></returns>0からN-1の範囲に補正した値
	static int WrapIndex(int index, int max);

	/// <summary>
	/// 点線を描画する
	/// </summary>
	/// <param name="sPos">始点</param>
	/// <param name="ePos">終点</param>
	/// <param name="color">色</param>
	/// <param name="len">間隔</param>
	static void DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len = 50.0f);

	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="_time">経過時間</param>
	/// <param name="_totalTime">アニメーション総時間</param>
	/// <param name="_start">開始位置</param>
	/// <param name="_end">終了位置</param>
	/// <returns>変化量</returns>
	static float EaseInQuad(float _time,const float _totalTime, const float _start, const float _end);


	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="_time">経過時間</param>
	/// <param name="_totalTime">アニメーション総時間</param>
	/// <param name="_start">開始位置</param>
	/// <param name="_end">終了位置</param>
	/// <returns>変化量</returns>
	static float EaseOutQuad(float _time, const float _totalTime, const float _start, const float _end);

	/// <summary>
	/// イーズインアウトバック
	/// </summary>
	/// <param name="_time">経過時間</param>
	/// <param name="_totalTime">アニメーション総時間</param>
	/// <param name="_start">開始位置</param>
	/// <param name="_end">終了位置</param>
	/// <returns>変化量</returns>
	static float EaseInOutBack(float _time, const float _totalTime, const float _start, const float _end);

	static constexpr float DEFAULT_SHAKE_SPEED = 5.0f;		//揺れの振幅
	static constexpr float DEFAULT_SHAKE_AMPLITUDE = 10.0f;	//揺れの速さ
	/// <summary>
	///	揺らす計算した値を返す
	/// </summary>
	/// <param name="_value">揺らしたい値</param>
	/// <param name="_time">時間</param>
	/// <param name="_speed">速度</param>
	/// <param name="_amplitude">振幅</param>
	/// <returns></returns>揺らす計算した値
	static float GetShake(const float _value, float _time, const float _speed = DEFAULT_SHAKE_SPEED, const float _amplitude = DEFAULT_SHAKE_AMPLITUDE);

	/// <summary>
	/// 指定した桁数の数字を返す
	/// </summary>
	/// <param name="_value">桁値を求める値</param>
	/// <param name="_digit">桁数</param>
	/// <returns>桁値</returns>
	static int GetDigit(const int _value, const int _digit);

	/// <summary>
	/// 桁数を返す
	/// </summary>
	/// <param name="_value">桁数を調べる値</param>
	/// <returns>桁数</returns>
	static int GetDigitCount(const int _value);

	/// <summary>
	/// 値を上下に動かす処理
	/// </summary>
	/// <param name="_value">現在の値</param>
	/// <param name="_step">増減量</param>
	/// <param name="_max">最大値</param>
	/// <param name="_min">最小値</param>
	/// <param name="_dir">増加方向（参照）1なら増加中、-1なら減少中</param>
	/// <returns>処理後の値</returns>
	static float PingPongUpdate(const float _value, const float _step, const float _max, const float _min, int& _dir);

	/// <summary>
	/// 点pから線分abへの最近点を求める
	/// </summary>
	/// <param name="p"></param>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns>最近点の座標</returns>
	static VECTOR ClosestPointOnSegment(VECTOR p, VECTOR a, VECTOR b);

	/// <summary>
	/// XZ平面に円を描画する
	/// </summary>
	/// <param name="center">円の中心座標</param>
	/// <param name="radius">円の半径</param>
	/// <param name="vertexNum">円の頂点数</param>
	/// <param name="fillFlag">塗りつぶすかどうか</param>
	static void DrawCircle3DXZ(VECTOR center, float radius, int vertexNum,int color, bool fillFlag);

	/// <summary>
	/// COLOR_FからFLOAT4に変換
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	static FLOAT4 COLOR_F2FLOAT4(const COLOR_F& color);

	/// <summary>
	/// モデルのフレームの最小最大座標を取得
	/// </summary>
	/// <param name="modelId">求めるモデルのハンドルID</param>
	/// <param name="minPos">最小座標(参照型)</param>
	/// <param name="maxPos">最大座標(参照型)</param>
	/// <param name="outFlameNum">計算に含めないフレーム番号</param>
	static void GetModelFlameBox(int modelId, VECTOR& minPos, VECTOR& maxPos,std::vector<int>outFlameNum ={});

	/// <summary>
	/// モデルのメッシュの最小最大座標を取得
	/// </summary>
	/// <param name="modelId">モデルのハンドルID</param>
	/// <param name="minPos">最小座標(参照型)</param>
	/// <param name="maxPos">最大座標(参照型)</param>
	static void GetModelMeshLocalBox(int modelId, VECTOR& minPos, VECTOR& maxPos);

	/// <summary>
	/// 3点から法線方向を求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	static VECTOR CalcNormal(const VECTOR& a, const VECTOR& b, const VECTOR& c);

	/// <summary>
	/// 4天の中心を求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <param name="d"></param>
	/// <returns></returns>
	static VECTOR CalcCenter(const VECTOR& a, const VECTOR& b, const VECTOR& c ,const VECTOR& d);

	/// <summary>
	/// 球面マッピングのUV座標を計算する
	/// </summary>
	/// <param name="normal">中心からの正規化された座標</param>
	/// <returns>uv値</returns>
	static FloatVector2 CalcSphericalUV(const VECTOR& normal);

	/// <summary>
	/// 点を中心を基準に回転させる
	/// </summary>
	/// <param name="point">回転させる点の座標</param>
	/// <param name="center">回転の中心座標</param>
	/// <param name="rot">回転角度（度数法）</param>
	/// <returns>回転後の座標</returns>
	static VECTOR ApplyRotation(const VECTOR& point, const VECTOR& center, const VECTOR& rot);

	/// <summary>
	/// X軸を中心に回転させる
	/// </summary>
	/// <param name="localPos">回転させる中心からの座標</param>
	/// <param name="rad">回転角度（ラジアン）</param>
	/// <returns>回転後の座標</returns>
	static VECTOR RotateX(VECTOR localPos, float rad);
	/// <summary>
	/// Y軸を中心に回転させる
	/// </summary>
	/// <param name="localPos">回転させる中心からの座標</param>
	/// <param name="rad">回転角度（ラジアン）</param>
	/// <returns>回転後の座標</returns>
	static VECTOR RotateY(VECTOR localPos, float rad);
	/// <summary>
	/// Z軸を中心に回転させる
	/// </summary>
	/// <param name="localPos">回転させる中心からの座標</param>
	/// <param name="rad">回転角度（ラジアン）</param>
	/// <returns>回転後の座標</returns>
	static VECTOR RotateZ(VECTOR localPos, float rad);

	/// <summary>
	/// vector配列の中のnullptrを削除する
	/// </summary>
	/// <typeparam name="T">型名</typeparam>
	/// <param name="allay">配列の参照型</param>
	/// <returns>削除した要素数</returns>
	template<typename T>
	static int EraseVectorAllay(std::vector<T>& allay);
};
template<typename T>
inline int Utility::EraseVectorAllay(std::vector<T>& allay)
{
	int ret = static_cast<int>(std::erase_if(allay, [](auto& a) {return a == nullptr;}));
	return ret;
}