uniform sampler2D u_texture;
uniform vec4 u_lightPosition;
uniform float u_lightPower;

// Input
varying vec2 v_textCoord;
varying vec3 v_normal;
varying vec4 v_position;

void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 diffMatColor = texture2D(u_texture, v_textCoord);
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);

    // eyeVect = v_position (point position)  - vec4(0.0, 0.0, 0.0, 0.0) (viewer position)
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition);
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));

    // Пятно блика
    float specularFactor = 60.0;
    // Свечение материала
    float ambientFactor = 0.1;
    // Расстояние от наблюдателя до просматриваемой точки
    float len = length(v_position.xyz - eyePosition.xyz);

    // Диффъюзный, рассеянный свет
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect));// / (1.0 + 0.25 * pow(len, 2));
    resultColor += diffColor;

    //
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;

    // Блики
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2));
    resultColor +=specularColor;

    gl_FragColor = resultColor;
}
