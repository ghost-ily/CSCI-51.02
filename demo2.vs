/******************************************************************************
 * This is a vertex shader that assumes that each vertex has a position and
 * color. The color of the vertex is further manipulated via a uniform, and
 * this color is passed to the fragment shader as an output.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
uniform float glow;
uniform float scale;
uniform float x_offset;
uniform float y_offset;
uniform vec3 rgb_offset;
out vec3 shaderColor;

void main()
{
    gl_Position = vec4((vertexPosition.x * x_offset + x_offset) * scale, (vertexPosition.y * y_offset + y_offset) * scale, vertexPosition.z, 1.0f);
    shaderColor = vertexColor * rgb_offset * glow;
}
