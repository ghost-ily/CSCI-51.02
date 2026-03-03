/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
uniform float glow1;
uniform float glow2;
uniform sampler2D shaderTextureA;
uniform sampler2D shaderTextureB;
out vec4 fragmentColor;

void main()
{
    vec4 colorA = texture(shaderTextureA, shaderTexCoord) * glow1;
    vec4 colorB = texture(shaderTextureB, shaderTexCoord) * glow2;
    fragmentColor = mix(colorA, colorB, 0.5f);
    //fragmentColor = vec4(shaderColor, 1.0f) * mix(colorA, colorB, 0.5f);
}