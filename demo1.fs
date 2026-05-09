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

uniform vec3 lightPosition;

uniform vec3 lightColor;

uniform float specularity;

out vec4 fragmentColor;

uniform sampler2D normalMap;

void main()
{
    vec3 lightPosition = lightPosition;
    vec3 lightColor = lightColor;

    vec3 l = normalize(lightPosition - worldSpacePosition);
    vec3 n = normalize(worldSpaceNormal);
    vec3 n_rgb = n * 0.5 + 0.5;

    n_rgb = texture(normalMap, shaderTexCoord).rgb;
    n = normalize(n_rgb * 2.0 - 1.0);

    float Cd = dot(n, l);
    Cd = max(Cd, 0.0f);

    float Ca = 0.3f;

    vec3 e = normalize(eyePosition - worldSpacePosition);
    vec3 r = reflect(-l, n);

    float shininess = specularity;
    float Cs = pow(max(dot(e, r), 0.0f), shininess);

    vec4 textureColor = texture(shaderTexture, shaderTexCoord);

    vec3 finalColor = (Ca + Cd) * shaderColor * textureColor.rgb;
    finalColor += Cs * lightColor;

    fragmentColor = vec4(finalColor, textureColor.a);
}
