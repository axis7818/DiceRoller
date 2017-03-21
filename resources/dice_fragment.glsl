#version 330 core
in vec3 fragNor;
in vec3 WPos;

layout(location = 0) out vec4 color;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float shine;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
   vec3 normal = normalize(fragNor);

   // diffuse
   vec3 Dcolor = MatDif * max(dot(normalize(lightPos - WPos), normal), 0);

   // specular
   vec3 viewDir = normalize(-WPos);
   vec3 halfDir = normalize(lightPos - WPos) + viewDir;
   float specAngle = max(dot(normalize(halfDir), normal), 0.0);
   vec3 Scolor = MatSpec * pow(specAngle, shine);

   // ambient
   vec3 Acolor = MatAmb;

   // compose
   color = vec4((Dcolor + Scolor + Acolor) * lightColor, 1.0);
}
