/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
uniform sampler2D shaderTextureA;
uniform sampler2D shaderTextureB;
out vec4 fragmentColor;

void main()
{
    vec4 colorA = texture(shaderTextureA, shaderTexCoord);
    vec4 colorB = texture(shaderTextureB, shaderTexCoord);
    fragmentColor = mix(colorA, colorB, 0.5f);
    //fragmentColor = vec4(shaderColor, 1.0f) * texture(shaderTextureA, shaderTexCoord);
}
