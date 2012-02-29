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
uniform sampler2D BackgroundSampler;
uniform sampler2D ForegroundSampler;

void main()
{
    vec2 tc = vTexCoord; 
    vec4 src = texture2D(ForegroundSampler, tc);
    tc.y = 1.0 - tc.y;
    vec4 dest = texture2D(BackgroundSampler, tc);

    src = clamp(src, vec4(0.0), vec4(1.0));
    dest = clamp(dest, vec4(0.0), vec4(1.0));
    float a = src.a; // clamp(1.0 - src.a, 0.0, 1.0);

    gl_FragColor.rgb = src.rgb * a + dest.rgb * (1.0 - a);
    gl_FragColor.a = 1.0;
}

