attribute vec4 a_position;
attribute vec2 a_textCoord;
attribute vec3 a_normal;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

// Output
varying vec2 v_textCoord;


//
// vec4 if point - last coord - 1
// vec4 if vector - last coord - 0

void main(void)
{
    mat4 viewMatrix = u_viewMatrix;
    viewMatrix[3][0] = 0;
    viewMatrix[3][1] = 0;
    viewMatrix[3][2] = 0;

    mat4 mv_matrix = viewMatrix * u_modelMatrix;

    gl_Position = u_projectionMatrix * mv_matrix * a_position;
    v_textCoord = a_textCoord;
}
