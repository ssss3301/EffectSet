attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec2 a_secondaryTexCoord;

#ifdef GL_ES
varying mediump vec2 v_texCoord;
varying mediump vec2 v_secondaryTexCoord;
#else
varying vec2 v_texCoord;
varying vec2 v_secondaryTexCoord;
#endif

void main()
{
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord = a_texCoord;
    v_secondaryTexCoord = a_secondaryTexCoord;
}
