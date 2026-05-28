struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};
float4x4 mWorldViewProj;
VS_OUTPUT main(VS_INPUT i)
{
    VS_OUTPUT o;
    o.Position = mul(float4(i.Position, 1.0f), mWorldViewProj);
    o.TexCoord = i.TexCoord;
    return o;
}