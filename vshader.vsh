attribute vec4 a_position;
attribute vec2 a_textCoord;
attribute vec3 a_normal;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

// Output
varying vec2 v_textCoord;
varying vec3 v_normal;
varying vec4 v_position;


//
// vec4 if point - last coord - 1
// vec4 if vector - last coord - 0

void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;

    gl_Position = u_projectionMatrix * mv_matrix * a_position;

    v_textCoord = a_textCoord;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * a_position;
}
