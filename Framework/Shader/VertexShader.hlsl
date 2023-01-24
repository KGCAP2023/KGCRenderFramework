
//https://forum.unity.com/threads/what-is-the-difference-between-float4-pos-sv_position-and-float4-pos.165351/

//struct VS_INPUT
//{
//    float4 inPos : POSITION;
//    float4 inTexCoord : TEXCOORD;
//};

//struct VS_OUPUT
//{
//    float4 inPos : SV_POSITION;
//    float4 inTexCoord : TEXCOORD;
//};

//VS_OUPUT main(VS_INPUT input)
//{
//    return input;
//}


cbuffer ConstantBuffer : register(b0)
{
    matrix WVP;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
// DirectX에서는 행렬
// 순서가 row-major(행우선)이고,
// HLSL에서는 column-major(열우선)이다.
// Output.Pos=mul(float4(Input.Pos,1.0f),matrixWorld); 전치 필요 
// 전치해서 넘기기가 싫다면 
// #pragma pack_matrix( row_major ) 이거 추가하면 된다.
//--------------------------------------------------------------------------------------

VS_OUTPUT main(float4 Pos : POSITION, float4 Color : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, WVP);
    output.Color = Color;
    return output;
}