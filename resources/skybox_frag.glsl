#version 330 core

in vec3 fragNor;
in vec2 texCoord;

uniform sampler2D Texture;

layout(location = 0) out vec4 color;

void main() {
   vec3 normal = normalize(fragNor);
   // color = vec4(normal, 1);
   // color = vec4(1, 0, 0, 1);
   color = texture(Texture, texCoord);
}
