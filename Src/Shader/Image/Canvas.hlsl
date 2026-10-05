#include "../Common/Pixel/PixelShader2DHeader.hlsli"

Texture2D Main : register(t1);
SamplerState mainSampler : register(s1); //サンプラー

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float2 suv = PSInput.suv;
    float4 col = Main.Sample(mainSampler, uv);
    float4 backCol = tex.Sample(texSampler, uv);
    float4 retCol = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if(col.r >0.9f&&col.g > 0.9 && col.b > 0.9)
    {
        //塗る部分が白色だったら
        retCol = backCol;
        retCol.a = 0.5f;
    }
    else
    {
        retCol = col;
    }
    return retCol;
    //return float4(color, 1.0f);
}