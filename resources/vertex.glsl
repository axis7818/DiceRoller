#version 330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 fragNor;
out vec3 WPos;
out vec2 texCoord;

void main() {
   mat4 MV = V * M;
   gl_Position = P * MV * vertPos;
   fragNor = (MV * vec4(vertNor, 0.0)).xyz;
   WPos = vec3(MV * vertPos);
   texCoord = vertTex;
}
