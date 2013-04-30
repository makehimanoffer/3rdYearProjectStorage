
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate position of vertex against world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store input color for pixel shader to use.
    output.color = input.color;
    
    return output;
}