// -- FS

varying vec2 vTexCoord;
uniform sampler2D Sampler;

void main()
{
    vec2 tc = vTexCoord; 
    gl_FragColor = texture2D(Sampler, tc);
}
