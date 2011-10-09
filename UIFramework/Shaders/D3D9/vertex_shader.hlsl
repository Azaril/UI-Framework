struct VS_INPUT
{
    float2 position : TEXCOORD0;
    float4 color : TEXCOORD1;
    float2 textureCoords : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 position : POSITION;
    float4 color : TEXCOORD0;
    float2 textureCoords : TEXCOORD1;
};

float4x4 transform;

VS_OUTPUT main(in VS_INPUT In)
{
    VS_OUTPUT Out;

    Out.position.xy = In.position;
    Out.position.zw = 1.0;

    Out.position = mul(transform, Out.position);

    Out.textureCoords = In.textureCoords;

    Out.color = In.color;

    return Out;
}