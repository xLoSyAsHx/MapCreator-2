attribute vec4 a_position;
attribute vec2 a_textcoord0;


uniform mat4 MVPMatrix;


varying vec2 qt_TexCoord0;

void main(void)
{
    gl_Position = MVPMatrix * a_position;
    qt_TexCoord0 = a_textcoord0;
}
