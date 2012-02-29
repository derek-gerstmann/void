#define EXPOSURE -10.0
#define LUMINANCE 1.0
#define SIZE 256.0

-- VS

attribute vec4 Position;
attribute vec2 TexCoord;
varying vec2 vTexCoord;
uniform float Depth;

void main()
{
    vTexCoord = TexCoord;
    gl_Position = Position;
    gl_Position.z = Depth;
}

-- FS

varying vec2 vTexCoord;
uniform sampler2D Frame;
uniform float Luminance = LUMINANCE;
uniform float Exposure = EXPOSURE;
uniform float Size = SIZE;

float LinearToSRGB(float c)
{
#define EXP (1.0 / 2.4)
#define A (0.055)
#define B (1.0 + A)

	float v = c;
	if (v < 0.0031308)
		v = 12.92 * v;
	else
		v = B * pow ( v , EXP ) - A;

	return v;
}

void main(void)
{
    vec4 color = texture2D(Frame, gl_TexCoord[0].st);

    // From RGB HDR to RGB 0..1
 	color.rgb = color.a * ( color.rgb / Luminance );
//	color.rgb = color.a * ( 1.0 - exp2( Exposure * color.rgb / Luminance ));    

    // Dithering
    bool evenx = fract(vTexCoord.s * Size) < 0.5;
    bool eveny = fract(vTexCoord.t * Size) < 0.5;

#define MUL (1.0 / (2.0*255.0*5.0))

    if (evenx && eveny)
    	color.rgb -= vec3(MUL, MUL, MUL);
    else if (evenx && !eveny)
    	color.rgb -= vec3(4.0 * MUL, 4.0 * MUL, 4.0 * MUL);
    else if (!evenx && eveny)
    	color.rgb -= vec3(3.0 * MUL, 3.0 * MUL, 3.0 * MUL);
    else
    	color.rgb -= vec3(2.0 * MUL, 2.0 * MUL, 2.0 * MUL);

    // From linear RGB to sRGB
    color.r = LinearToSRGB(color.r);
    color.g = LinearToSRGB(color.g);
    color.b = LinearToSRGB(color.b);
    
    gl_FragColor = color;
}