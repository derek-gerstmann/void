// -- FS

varying vec2 vTexCoord;
uniform sampler2D Sampler;

void main()
{
    vec2 tc = vTexCoord;
//    tc = tc * vec2(0.5, 0.5) + vec2(0.5, 0.5); 
    gl_FragColor = texture2D(Sampler, tc);
}
