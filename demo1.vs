/******************************************************************************
 * This is a really simple vertex shader that simply sets the output vertex's
 * position to be the same as the input.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec2 vertexMoveCoord;
uniform float glow1;
out vec3 shaderColor;
out vec2 shaderTexCoord;

void main()
{
    gl_Position = vec4(vertexPosition, 1.0f) + vec4(vertexMoveCoord.x * glow1, vertexMoveCoord.y * glow1, 0.0f, 0.0f);
    shaderColor = vertexColor;
    shaderTexCoord = vertexTexCoord;
}
