varying vec4 v_fragColor;
varying vec2 v_texCoord;

uniform vec4 outline_color;
uniform float radius;

void main()
{
    vec4 normal = vec4(0.0);
    vec4 accum = vec4(0.0);
    outline_color = vec4(1.0, 0.0, 0.0, 0.0);
    radius = 0.02f;
    normal = texture2D(CC_Texture0, v_texCoord);
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x - radius, v_texCoord.y - radius));
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x + radius, v_texCoord.y - radius));
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x - radius, v_texCoord.y + radius));
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x + radius, v_texCoord.y + radius));
    accum.rgb = outline_color * accum.a;
    accum.a = 0;
    normal = accum * (1 - normal.a) + normal * normal.a;
    gl_FragColor = v_fragColor * normal;
}