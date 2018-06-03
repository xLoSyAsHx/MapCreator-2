attribute vec3 a_position;
attribute vec2 a_textCoord;
attribute vec3 a_normal;
attribute vec4 a_id;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

varying vec4 v_id;

void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;

    v_id = a_id;
    gl_Position = u_projectionMatrix * mv_matrix * vec4(a_position, 1);

}
