
//struct PS_INPUT
//{
//    float4 inPos : SV_POSITION;
//    float4 inTexCoord : TEXCOORD;
//};

//float4 main(PS_INPUT input) : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};


float4 main(VS_OUTPUT input) : SV_Target
{
   return input.Color;


}