#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 WVP;
uniform mat4 world;

out vec2 UV;
out vec3 normal;
//out vec3 worldPos;

void main()
{
    gl_Position = WVP * vec4(position, 1.0);
    UV = vertexUV;
    normal = (world * vec4(vertexNormal, 0.0)).xyz;
    //worldPos = (world * vec4(position, 1.0)).xyz;
}
