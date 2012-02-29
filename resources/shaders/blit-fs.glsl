// -- FS

varying vec2 vTexCoord;
uniform sampler2D Sampler;

void main()
{
    vec2 tc = vTexCoord; 
    tc.y = 1.0 - tc.y;
    gl_FragColor = texture2D(Sampler, tc);
}
