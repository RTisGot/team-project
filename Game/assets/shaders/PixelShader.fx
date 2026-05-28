

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};
Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);
float4 main(VS_OUTPUT i) : SV_TARGET
{
    return DiffuseTexture.Sample(DiffuseSampler, i.TexCoord);
}