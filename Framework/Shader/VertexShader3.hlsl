
cbuffer ConstantBuffer : register(b0)
{
    matrix wvpMatrix;
    matrix worldMatrix;
}

#define MAX_BONES 500
cbuffer ConstantBuffer2 : register(b1)
{
    float4x4 gBoneTransforms[MAX_BONES];
}

struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float4 boneIndices : BLENDINDICES;
    float4 boneWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    //¿¾³¯ 
    //float4x4 BoneTransform = gBoneTransforms[input.boneIndices.x] * input.boneWeights.x;
    //BoneTransform += gBoneTransforms[input.boneIndices.y] * input.boneWeights.y;
    //BoneTransform += gBoneTransforms[input.boneIndices.z] * input.boneWeights.z;
    //BoneTransform += gBoneTransforms[input.boneIndices.w] * input.boneWeights.w;
    
    //float4 PosL = mul(BoneTransform, float4(input.inPos, 1.0f));
    //output.outPosition = mul(PosL, wvpMatrix);
    
    float finalWeight = 1 - (input.boneWeights[0] + input.boneWeights[1] + input.boneWeights[2]);
	
    float4x4 boneTransform = gBoneTransforms[(int) input.boneIndices[0]] * input.boneWeights[0];
    boneTransform += gBoneTransforms[(int) input.boneIndices[1]] * input.boneWeights[1];
    boneTransform += gBoneTransforms[(int) input.boneIndices[2]] * input.boneWeights[2];
    boneTransform += gBoneTransforms[(int) input.boneIndices[3]] * finalWeight;
    
    output.outPosition = mul(float4(input.inPos, 1.0f),boneTransform);
    output.outPosition = mul(output.outPosition, wvpMatrix);

    output.outTexCoord = input.inTexCoord;
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), worldMatrix));
    output.outWorldPos = mul(float4(input.inPos, 1.0f), worldMatrix);
    return output;
}