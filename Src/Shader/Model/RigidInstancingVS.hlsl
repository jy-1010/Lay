// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME
#define L_W_MAT g_localWorldMatrix.lwMatrix

// OUT
#define VS_OUTPUT VertexToPixelRigidInstancing
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

struct InstanceData
{
    float4 position;
    float4 rotation;
    float4 scale;
};

StructuredBuffer<InstanceData> g_instanceBuffer : register(t0);

// 回転行列作成
float3x3 CreateRotationMatrix(float3 rotation)
{
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);

    float cy = cos(rotation.y);
    float sy = sin(rotation.y);

    float cz = cos(rotation.z);
    float sz = sin(rotation.z);

    float3x3 rotX = float3x3(
        1, 0, 0,
        0, cx, -sx,
        0, sx, cx
    );

    float3x3 rotY = float3x3(
         cy, 0, sy,
          0, 1, 0,
        -sy, 0, cy
    );

    float3x3 rotZ = float3x3(
        cz, -sz, 0,
        sz, cz, 0,
         0, 0, 1
    );

    return mul(mul(rotX, rotY), rotZ);
}

// インスタンス変換
float3 TransformInstancePosition(
    float3 localPosition,
    InstanceData instance)
{
    // Scale
    localPosition *= instance.scale.xyz;

    // Rotation
    float3x3 rotationMatrix =
        CreateRotationMatrix(instance.rotation.xyz);

    localPosition = mul(localPosition, rotationMatrix);

    // Instance Position
    localPosition += instance.position.xyz;

    return localPosition;
}

VS_OUTPUT main(VS_INPUT VSInput,uint instanceID : SV_InstanceID)
{
VS_OUTPUT ret;

// インスタンスデータを取得
    InstanceData instance =g_instanceBuffer[instanceID];

//// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
//    float4 lLocalPosition;
//    float4 lWorldPosition;
//// float3 → float4
//    lLocalPosition.xyz = VSInput.pos;
//    lLocalPosition.w = 1.0f;
//// ローカル座標をワールド座標に変換(剛体)
//    lWorldPosition.w = 1.0f;
//    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
//    ret.worldPos = lWorldPosition.xyz; // ワールド座標をピクセルシェーダへ引き継ぐ
//// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
    
//    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
//// UV座標
//    ret.uv.x = VSInput.uv0.x;
//    ret.uv.y = VSInput.uv0.y;
//// ディフューズカラー
//    ret.diffuse = VSInput.diffuse;
//// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )
//// 出力パラメータを返す
//    return ret;
    
    // インスタンスごとの回転行列
    float3x3 rotationMatrix =
    CreateRotationMatrix(instance.rotation.xyz);

    // モデルのローカル座標をインスタンス単位で変換
    float3 instancePosition =
    TransformInstancePosition(VSInput.pos, instance);

    // インスタンスの位置をワールド座標として使う場合
    float4 worldPosition = float4(instancePosition, 1.0f);

    ret.worldPos = worldPosition.xyz;
    
    float3 worldNormal =
    normalize(mul(VSInput.norm, rotationMatrix));

    float3 worldTangent =
    normalize(mul(VSInput.tan, rotationMatrix));

    float3 worldBinormal =
    normalize(mul(VSInput.bin, rotationMatrix));
    
    ret.worldNormal = worldNormal;
    ret.worldTangent = worldTangent;
    ret.worldBinormal = worldBinormal;
    // UV
    ret.uv = VSInput.uv0;
    
    // Diffuse
    ret.diffuse = VSInput.diffuse;

    return ret;
}