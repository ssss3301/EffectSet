#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;
varying vec2 v_secondaryTexCoord;

uniform sampler2D mainTex;
uniform sampler2D secondaryTex;
uniform bool useFlowLight;
uniform bool useGray;

void main()
{
    vec4 mainColor =  texture2D(mainTex, v_texCoord);
    if(useGray)
    {
	gl_FragColor.xyz = vec3(0.2126*mainColor.r + 0.7152*mainColor.g + 0.0722*mainColor.b);
	gl_FragColor.w = mainColor.w;
    }
    else
    {
	    if(useFlowLight)
	    {
		vec4 secondaryColor = vec4(0.0, 0.0, 0.0, 0.0);
		if(v_secondaryTexCoord.x <= 1.0f && v_secondaryTexCoord.x >= 0.0f)
			secondaryColor =  texture2D(secondaryTex, v_secondaryTexCoord);
		if(mainColor.a <= 0.1f)
			gl_FragColor = mainColor + secondaryColor * mainColor.a;
		else
			gl_FragColor = mainColor * (1 - secondaryColor.a) + secondaryColor * secondaryColor.a;
	    }
	    else
	    {
		gl_FragColor = mainColor;
	    }
    }
}