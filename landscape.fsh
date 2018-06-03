uniform int gObjectIndex_1;
uniform int gObjectIndex_2;
uniform int gObjectIndex_3;
uniform int gObjectIndex_4;

void main()
{
    gl_FragColor = vec4(gObjectIndex_1, gObjectIndex_2, gObjectIndex_3, gObjectIndex_4);

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
}
