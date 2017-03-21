#version 330 core

in vec3 fragNor;
in vec3 WPos;
in vec2 texCoord;

layout(location = 0) out vec4 color;

void main() {
   vec3 normal = normalize(fragNor);
   color = vec4(normal, 1);
}
