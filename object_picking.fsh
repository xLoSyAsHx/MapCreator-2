/*
uniform int gObjectIndex_1;
uniform int gObjectIndex_2;
uniform int gObjectIndex_3;
uniform int gObjectIndex_4;
*/

varying vec4 v_id;

void main()
{
    //float param1 = float(v_id) / 255.0;

    gl_FragColor = v_id;
    /*
    gObjectIndex_4 += 1;

    if (gObjectIndex_4 > 10000)
    {
        gObjectIndex_4 = 1;
        gObjectIndex_3 += 1;
    }

    if (gObjectIndex_3 > 10000)
    {
        gObjectIndex_3 = 1;
        gObjectIndex_2 += 1;
    }

    if (gObjectIndex_2 > 10000)
    {
        gObjectIndex_2 = 1;
        gObjectIndex_1 += 1;
    }
    */
}
