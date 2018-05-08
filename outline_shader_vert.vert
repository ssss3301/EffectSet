attribute vec4 a_color;
attribute vec4 a_position;
attribute vec2 a_texCoord;

#ifdef GL_ES
varying lowp vec4 v_fragColor;
varying mediump vec2 v_texCoord;
#else
varying vec4 v_fragColor;
varying vec2 v_texCoord;
#endif

void main()
{
    gl_Position = CC_PMatrix * a_position;
    v_fragColor = a_color;
    v_texCoord = a_texCoord;
}