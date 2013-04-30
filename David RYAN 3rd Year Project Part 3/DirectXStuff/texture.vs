



// GLOBALS //

cbuffer MxBuffer
{
    matrix worldMx;
    matrix viewMx;
    matrix projectionMx;
};


// TYPEDEFS //

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};



// Vertex Shader

PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate position of vertex against world, view, and projection matrices.
    output.position = mul(input.position, worldMx);
    output.position = mul(output.position, viewMx);
    output.position = mul(output.position, projectionMx);


    // Store texture coordinates for pixel shader.
    output.tex = input.tex;
    
    return output;
}
