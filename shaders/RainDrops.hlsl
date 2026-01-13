// Screen rain drops.
// Heavily inspired by https://www.shadertoy.com/view/ltffzl.
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// Defines:
//   BLUR - Use blurred buffer.

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
    float2 screenPos : VPOS;
};

struct PixelOutput {
    float4 color : COLOR0;
};

#ifdef VS

float4 GeometryOffset : register(c0);
float4 TextureOffset : register(c1);

VertexOutput Main(VertexData vertex) {
    VertexOutput output;
    
    output.positionSS.xy = vertex.position.xy - (float2(2, -2) * GeometryOffset.xy);
    output.positionSS.zw = vertex.position.zw;
    output.uv = vertex.uv + TextureOffset.xy;

    return output;
}

#else

sampler2D RenderedBuffer : register(s0);

#ifdef BLUR
    sampler2D BlurredBuffer : register(s1);
#endif

float4 ScreenData : register(c0);
float4 RainData : register(c1);
float4 RainSettings[2] : register(c2);
float4 FogData : register(c4);

static const float2 InvResolution = ScreenData.xy;
static const float2 Resolution = ScreenData.zw;
static const float Time = RainData.x;
static const float StaticScale = RainData.y;
static const float Moving1Scale = RainData.z;
static const float Moving2Scale = RainData.w;
static const float Density = RainSettings[0].x;
static const float Size = RainSettings[0].y;
static const float EnableStatic = RainSettings[0].z;
static const float EnableMoving = RainSettings[0].w;
static const float EnableFog = RainSettings[1].x;
static const float EnableFogTrails = RainSettings[1].y;
static const float FogColorInfluence = RainSettings[1].z;
static const float3 FogColor = FogData.xyz;
static const float FogStrength = FogData.a;

// Interpolation used for animating the drops.
float Saw(float b, float t) {
    return smoothstep(0.0f, b, t) * smoothstep(1.0f, b, t);
}

float Random(float seed) {
    return frac(sin(seed * 12345.564) * 7658.76);
}

// Random vector.
float3 Random3D(float seed) {
    float3 seed3D = frac(seed.xxx * float3(0.1031f, 0.11369f, 0.13787f));
    seed3D += dot(seed3D, seed3D.xyz + 19.19f);
    return frac(float3((seed3D.x + seed3D.y) * seed3D.z, (seed3D.x + seed3D.z) * seed3D.y, (seed3D.y + seed3D.z) * seed3D.x));
}

// Static drop calculation.
float StaticDrops(float2 coord, float t) {
    coord *= 20.0f;
    
    float2 id = floor(coord);
    coord = frac(coord) - 0.5f;
    float3 rand = Random3D(id.x * 107.45f + id.y * 3543.654);
    
    float dropCycleLength = 0.5f + rand.z * 2.0f;
    float dropTime = frac((t + rand.z * 37.0f) / dropCycleLength);
    
    float cycleIndex = floor((t + rand.z * 37.0f) / dropCycleLength);
    float3 posRand = Random3D(id.x * 107.45f + id.y * 3543.654 + cycleIndex * 845.123f);
    
    if (posRand.z > Density) 
        return 0.0f;
    
    float2 p = (posRand.xy - 0.5f) * 0.7f;
    float d = length(coord - p);
    
    float fade = Saw(0.025f, dropTime);
    
    return smoothstep(0.2f * Size, 0.0f, d) * frac(rand.z * 10.0f) * fade;
}

// Moving drops with trails.
float2 MovingDrops(float2 coord, float t) {
    float2 uv = coord;
    
    // Animate downward.
    coord.y -= t * 0.75f;
    
    float2 aspect = float2(6.0f, 1.0f);
    float2 grid = aspect * 2.0f;
    float2 id = floor(coord * grid);
    
    // Column shift for variation.
    float colShift = Random(id.x);
    coord.y += colShift;
    
    id = floor(coord * grid);
    float3 rand = Random3D(id.x * 35.2f + id.y * 2376.1f);
    
    if (rand.z > Density)
        return float2(0.0f, 0.0f);
    
    float2 st = frac(coord * grid) - float2(0.5f, 0.0f);
    
    float x = rand.x - 0.5f;
    
    // Wiggle motion as drop falls.
    float y = uv.y * 20.0f;
    float wiggle = sin(y + sin(y));
    x += wiggle * (0.5f - abs(x)) * (rand.z - 0.5f);
    x *= 0.5f;
    
    // Animate drop position in cell.
    float ti = frac(t + rand.z);
    y = 1.0f - ((Saw(0.85f, ti) - 0.5f) * 0.6f + 0.5f);
    float2 p = float2(x, y);
    
    // Main drop.
    float d = length((st - p) * aspect.yx);
    float mainDrop = smoothstep(0.15f * Size, 0.0f, d);
    
    // Trail behind the drop.
    float r = sqrt(smoothstep(1.0f, y, 1.0f - st.y));
    float cd = abs(st.x - x);
    float trail = smoothstep(0.18f * Size * r, 0.15f * Size * r * r, cd);
    float trailFront = smoothstep(-0.02f, 0.02f, y - st.y);
    trail *= trailFront * r * r * r;
    
    return float2(mainDrop, trail);
}

// Combined drops.
// coord should be aspect-ratio corrected.
float2 Drops(float2 coord, float t) {
    float s = 0.0f;
    float2 m1 = float2(0.0f, 0.0f);
    float2 m2 = float2(0.0f, 0.0f);
    
    [branch]
    if (EnableStatic > 0)
        s = StaticDrops(coord, t) * StaticScale;
    
    [branch]
    if (EnableMoving > 0) {
        m1 = MovingDrops(coord, t) * Moving1Scale;
        m2 = MovingDrops(coord * 1.85f, t) * Moving2Scale;
    }
    
    // Combine drop visibility
    float c = s + m1.x + m2.x;
    c = smoothstep(0.3f, 1.0f, c);
    
    return float2(c, max(m1.y * Moving1Scale, m2.y * Moving2Scale) * EnableFogTrails);
}

// Fog amount, clear middle of the screen increasing to edges.
float Fog(float2 uv) {
    float2 centerDist = abs(uv - 0.5f) * 2.0f;
    float radialDist = length(centerDist);
    
    float fog = pow(radialDist, 3.0f);
    
    return saturate(fog);
}

PixelOutput Main(PixelInput input) {
    PixelOutput output;
    
    float2 coord = (input.screenPos.xy - 0.5f * Resolution.xy) * InvResolution.y;
    
    float t = Time * 0.2f;
    
    // Calculate drops.
    float2 center = Drops(coord, t);
    
    float offset = 1.0f * InvResolution.y;
    float2 horizontal = Drops(coord + float2(offset, 0.0f), t);
    float2 vertical = Drops(coord + float2(0.0f, offset), t);
    
    float2 normal = float2(horizontal.x - center.x, vertical.x - center.x);
    
    float fog = Fog(input.uv) * FogStrength;
    
    fog = max(0.0f, fog - center.y * 2.0f);

    float blur = lerp(fog, 0.0f, smoothstep(0.1f, 0.2f, center.x)) * EnableFog;
    
    float4 renderedColor = tex2D(RenderedBuffer, input.uv.xy + normal);
    
#ifdef BLUR
    float4 blurredColor = tex2D(BlurredBuffer, input.uv.xy + normal);
    
    blurredColor = lerp(blurredColor, blurredColor * float4(FogColor, 1.0f), FogColorInfluence);
#else
    float4 blurredColor = lerp(renderedColor, renderedColor * float4(FogColor, 1.0f), FogColorInfluence);
#endif

    output.color.rgba = lerp(renderedColor.rgba, blurredColor.rgba, blur);
    
    return output;
}

#endif