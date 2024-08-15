#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform vec4 line_color;


void main()
{
    gl_FragColor = line_color;
}


