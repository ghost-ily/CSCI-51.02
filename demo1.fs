/******************************************************************************
 * This is a really simple fragment shader that simply sets the output fragment
 * color to yellow.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#version 330 core

in vec3 shaderColor;
in vec2 shaderTexCoord;
in vec3 worldSpacePosition;
in vec3 worldSpaceNormal;

uniform sampler2D shaderTexture;

uniform vec3 eyePosition;

out vec4 fragmentColor;

void main()
{
    vec3 lightPosition = vec3(2.0f, 2.0f, 0.0f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

    vec3 l = normalize(lightPosition - worldSpacePosition);
    vec3 n = normalize(worldSpaceNormal);

    float Cd = dot(n, l);
    Cd = max(Cd, 0.0f);

    float Ca = 0.3f;

    vec3 e = normalize(eyePosition - worldSpacePosition);
    vec3 r = reflect(-l, n);

    float shininess = 32.0f;
    float Cs = pow(max(dot(e, r), 0.0f), shininess);

    vec4 textureColor = texture(shaderTexture, shaderTexCoord);

    vec3 finalColor = (Ca + Cd) * shaderColor * textureColor.rgb;
    finalColor += Cs * lightColor;

    fragmentColor = vec4(finalColor, textureColor.a);
}
