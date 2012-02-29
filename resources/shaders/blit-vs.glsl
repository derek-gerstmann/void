// -- VS

attribute vec4 Position;
attribute vec2 TexCoord;
varying vec2 vTexCoord;
// uniform float Depth;

void main()
{
    vTexCoord = TexCoord;
	gl_Position = Position;
//    gl_Position.z = Depth;
}

