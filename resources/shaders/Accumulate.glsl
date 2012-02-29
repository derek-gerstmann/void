-- VS

attribute vec4 Position;
attribute vec2 TexCoord;
varying vec2 vTexCoord;
uniform float ScrollOffset;
uniform float Depth;

void main()
{
    vTexCoord = TexCoord;
    gl_Position = Position;
    gl_Position.z = Depth;
}

-- FS

varying vec2 vTexCoord;
uniform float Weight = 1.0;
uniform sampler2D AccumColorSampler;
uniform sampler2D AccumWeightSampler;
uniform sampler2D ColorSampler;
uniform sampler2D WeightSampler;
uniform float WindowWidth;
uniform float WindowHeight;
uniform float Seed;

int RandomHash1i(int seed)
{
    int i=(seed^12345391)*2654435769;
    i^=(i<<6)^(i>>26);
    i*=2654435769;
    i+=(i<<5)^(i>>12);
    return i;
}

#define FLT_UINT_MAX 	(4294967295.0)

float RandomHash1f(int seed, float b)
{
    return float(b * RandomHash1i(seed)) / FLT_UINT_MAX;
}

void main()
{
    vec2 ntc = vTexCoord; 
    vec2 tc = vTexCoord; 
    tc.y = 1.0 - tc.y;

	vec2 window_size = vec2(WindowWidth, WindowHeight);
	int seed = int(gl_FragCoord.y * window_size.x * 1024 + gl_FragCoord.x * 1024 * Seed);
    vec4 accum_color = texture2D(AccumColorSampler, tc);
    float accum_weight = Weight; // texture2D(AccumWeightSampler, tc).r;

	ntc.x += RandomHash1f(++seed, 2.0/window_size.x);
	ntc.y += RandomHash1f(++seed, 2.0/window_size.y);
    vec4 sample_color = texture2D(ColorSampler, tc);
    float sample_weight = 1.0; // texture2D(WeightSampler, tc).r;
    
	gl_FragColor = accum_color + sample_color;
	gl_FragDepth = accum_weight + sample_weight;
}

