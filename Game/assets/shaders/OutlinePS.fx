float4 OutlineParam : register(c0);
float4 main() : SV_TARGET
{
    return float4(OutlineParam.y, OutlineParam.z, OutlineParam.w, 1.0f);
}