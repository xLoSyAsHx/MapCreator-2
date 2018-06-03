uniform sampler2D u_texture;
uniform vec4 u_lightPosition;
uniform float u_lightPower;
uniform int u_hasTexture;
uniform int u_isLandscape;

uniform vec3 u_diffuse;
uniform vec3 u_ambient;
uniform vec3 u_specular;
uniform float u_shininess;



// Spot light for LandscapeSculptTool
uniform int u_isLandscapeSculptToolEnable;
uniform vec2 u_spotLightPosition;

// Input
varying vec2 v_textCoord;
varying vec3 v_normal;
varying vec4 v_position;
varying vec2 v_notTransformedPosition;

//varying vec2 v_spotLightPosition;

void main(void)
{
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




    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    //vec4 diffMatColor = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 diffMatColor;

    if (u_isLandscape == 0)
    {
        if (u_hasTexture == 1)
        {

            diffMatColor = texture2D(u_texture, v_textCoord);

            // Диффъюзный, рассеянный свет
            vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect));// / (1.0 + 0.25 * pow(len, 2));
            resultColor += diffColor;

            //
            vec4 ambientColor = ambientFactor * diffMatColor;
            resultColor += ambientColor;

            // Блики
            vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2));
            resultColor +=specularColor;
        }
        else if (u_hasTexture == 0)
        {
            // Диффъюзный, рассеянный свет
            resultColor += vec4(u_diffuse, 1) * u_lightPower * max(0.0, dot(v_normal, -lightVect));

            // Ambient
            resultColor += vec4(u_ambient, 1);

            // Блики
            resultColor += vec4(u_specular, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2));

            // Свечение
            resultColor += (u_shininess / 100) * vec4(u_ambient, 1);

        }
    }
    else
    {
        if (u_hasTexture == 1)
        {

            diffMatColor = texture2D(u_texture, v_textCoord);

            // Диффъюзный, рассеянный свет
            vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect));// / (1.0 + 0.25 * pow(len, 2));
            resultColor += diffColor;

            //
            vec4 ambientColor = ambientFactor * diffMatColor;
            resultColor += ambientColor;

            // Блики
            vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2));
            resultColor +=specularColor;
        }
        else if (u_hasTexture == 0)
        {
            // Диффъюзный, рассеянный свет
            resultColor += vec4(u_diffuse, 1) * u_lightPower * max(0.0, dot(v_normal, -lightVect));

            // Ambient
            resultColor += vec4(u_ambient, 1);

            // Блики
            resultColor += vec4(u_specular, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor);// / (1.0 + 0.25 * pow(len, 2));

            // Свечение
            resultColor += (u_shininess / 100) * vec4(u_ambient, 1);

        }

        if (u_isLandscapeSculptToolEnable == 1)
        {
            float distance = distance(u_spotLightPosition, v_notTransformedPosition);
            if (distance > 4.0 && distance < 5.0)
            {
                resultColor += vec4(0.4, 0, 0, 0);
            }
        }
    }

    gl_FragColor = resultColor;
}
