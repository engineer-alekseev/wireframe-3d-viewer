#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec4 dot_color;

void main()
{
    gl_FragColor = dot_color;
}
