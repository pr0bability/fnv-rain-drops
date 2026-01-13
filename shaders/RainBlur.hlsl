// Separable gaussian blur.

#ifdef __INTELLISENSE__
    #define PS
#endif

// Shader inputs and outputs.

struct VertexData {
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VertexOutput {
    float4 positionSS : POSITION;
    float2 uv : TEXCOORD0;
};

struct PixelInput {
    float2 uv : TEXCOORD0;
};

struct PixelOutput {
    float4 color : COLOR0;
};

#ifdef VS

float4 GeometryOffset : register(c0);
float4 TextureOffset : register(c1);

row_major float4x4 InvProjMatrix : register(c3);

VertexOutput Main(VertexData vertex) {
    VertexOutput output;
    
    output.positionSS.xy = vertex.position.xy - (float2(2, -2) * GeometryOffset.xy);
    output.positionSS.zw = vertex.position.zw;
    output.uv = vertex.uv + TextureOffset.xy;

    return output;
}

#else

sampler2D TexBuffer : register(s0);

float4 ScreenData : register(c0);

static const float2 TexelSize = ScreenData.xy;
static const float2 OffsetMask = ScreenData.zw;

static const float GaussianWeights[5] = {
    0.227027, 
    0.1945946, 
    0.1216216, 
    0.054054, 
    0.016216
};

PixelOutput Main(PixelInput input) {
    PixelOutput output;
    
    float4 sample = tex2D(TexBuffer, input.uv);
    float3 sum = sample.rgb;
    
    output.color.a = 1.0f;
    
    float totalWeight = GaussianWeights[0];
    sum *= totalWeight;
    
    [unroll]
    for (int i = -4; i <= 4; i++) {
        if (i == 0)
            continue;
            
        float2 offset = ((i * 2.0f) * OffsetMask) * TexelSize;
        
        sample = tex2D(TexBuffer, input.uv + offset);
        float3 sampleValue = sample.rgb;
        
        float weight = GaussianWeights[abs(i)];
        
        sum += sampleValue * weight;
        totalWeight += weight;
    }
    
    const float epsilon = 0.0001f;
    output.color.rgb = sum / (totalWeight + epsilon);
    
    return output;
}

#endif