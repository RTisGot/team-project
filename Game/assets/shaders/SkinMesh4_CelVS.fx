struct VS_INPUT
{

float3 Position : POSITION;
float3 Normal : NORMAL0;
float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 WorldPos : TEXCOORD2;
};

VS_OUTPUT PSMain(VS_INPUT IN)
{
    VS_OUTPUT OUT;

    // MV1が内部でスキニングする前提
    OUT.Position = float4(IN.Position, 1.0);
    OUT.Tex = IN.Tex;
    OUT.Normal = IN.Normal;
    OUT.WorldPos = IN.Position;

    return OUT;
}