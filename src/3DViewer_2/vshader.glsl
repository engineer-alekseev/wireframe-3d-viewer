#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform float point_size;

attribute vec4 a_position;
void main()
{
    gl_Position = mvp_matrix * a_position;
    gl_PointSize = point_size;
}

