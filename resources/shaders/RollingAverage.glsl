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

vec4 RollingAverage(
	vec4 value, vec4 newEntry, 
	float n, float weighting ) 
{
    return (value * n + newEntry * weighting)/(n + weighting);
}

void main()
{
    vec2 tc = vTexCoord; 
    vec4 accum_color = texture2D(AccumColorSampler, tc);
    float accum_weight = Weight; // texture2D(AccumWeightSampler, tc).r;
    vec4 sample_color = vec4(1.0); // texture2D(ColorSampler, tc);
    float sample_weight = 1.0; // texture2D(WeightSampler, tc).r;
    
    vec4 value = accum_color * (1.0 / accum_weight); // RollingAverage(accum_color, sample_color, accum_weight, sample_weight);
	gl_FragColor = value;
}

