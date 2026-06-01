struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float4 Diffuse : COLOR0;
    float2 TexCoord : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
};
float4x4 mWorldViewProj;
// c0.x = OutlineWidth
float4 OutlineParam : register(c0);
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    float outlineWidth = OutlineParam.x;
    // ñ@ê¸ï˚å¸Ç…âüÇµèoÇµ
    float3 expandedPos = input.Position + normalize(input.Normal) * outlineWidth;
    output.Position = mul(float4(expandedPos, 1.0f), mWorldViewProj);
    return output;
}