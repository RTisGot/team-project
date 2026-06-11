Texture2D DiffuseMap : register(t0);
SamplerState DiffuseSampler : register(s0);

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 WorldPos : TEXCOORD2;
};

float4 PSMain(PS_INPUT IN) : SV_Target
{
    float4 albedo = DiffuseMap.Sample(DiffuseSampler, IN.Tex);

    float3 N = normalize(IN.Normal);
    float3 L = normalize(float3(0.35, 0.85, 0.25));

    float ndl = saturate(dot(N, L));

    // 2’i‰ei‚Ü‚¸ˆÀ’è—Dæj
    float shade = ndl > 0.5 ? 1.0 : 0.4;

    float3 color = albedo.rgb * shade;

    return float4(color, albedo.a);
}