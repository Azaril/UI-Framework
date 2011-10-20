struct PS_INPUT
{
    float4 color : TEXCOORD0;
    float2 textureCoords : TEXCOORD1;
	float2 maskCoords : TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 Color : SV_TARGET0;
};

SamplerState LinearSampler
{    
	Filter = MIN_MAG_MIP_LINEAR;    
	AddressU = Clamp;    
	AddressV = Clamp;
};

Texture2D brushTexture;
Texture2D maskTexture;

PS_OUTPUT main(in PS_INPUT In)
{
    PS_OUTPUT Out;

    Out.Color = In.color * brushTexture.Sample(LinearSampler, In.textureCoords) * maskTexture.Sample(LinearSampler, In.maskCoords);

    return Out;
}