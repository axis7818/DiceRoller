#version 330 core
in vec3 fragNor;
in vec3 WPos;
in vec2 texCoord;

uniform sampler2D Texture;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float opacity;
uniform vec3 MatAmb;
uniform float roughnessValue;
uniform float F0;
uniform float K;
uniform int clipMode; // 0 -> normal, 1 -> clip black, 2 -> clip white

layout(location = 0) out vec4 color;

void main() {

   // interpolating normals will change the length, renormalize
   vec3 normal = normalize(fragNor);
   vec3 lightDirection = lightPos - WPos;
   float lightDistance = length(lightDirection);

   // do the lighting calculation for each fragment
   float NdotL = max(dot(normal, lightDirection), 0.0);

   float specular = 0.0;
   if (NdotL > 0.0) {
      vec3 eyeDir = normalize(-WPos);

      // calculate intermediary values
      vec3 halfVector = normalize(lightDirection + eyeDir);
      float NdotH = max(dot(normal, halfVector), 0.0);
      float NdotV = max(dot(normal, eyeDir), 0.0);
      float VdotH = max(dot(eyeDir, halfVector), 0.0);
      float mSquared = roughnessValue * roughnessValue;

      // geometric attenuation
      float NH2 = 2.0 * NdotH;
      float g1 = (NH2 * NdotV) / VdotH;
      float g2 = (NH2 * NdotL) / VdotH;
      float geoAtt = min(1.0, min(g1, g2));

      // roughness beckmann distribution function
      float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
      float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
      float roughness = r1 * exp(r2);

      // fresnel Schlick approximation
      float fresnel = pow(1.0 - VdotH, 5.0);
      fresnel *= (1.0 - F0);
      fresnel += F0;

      specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.1415926);
   }

   // texture color
   vec4 texColor = texture(Texture, texCoord);
   if (clipMode == 1 && texColor.r < 0.1 && texColor.g < 0.1 &&
    texColor.b < 0.3) {
      discard;
   } else if (clipMode == 2 && texColor.r > 0.9 && texColor.g > 0.9 &&
    texColor.b > 0.9) {
      discard;
   } else if (clipMode == 2) {
      texColor.x = 1.0 - texColor.x;
      texColor.y = 1.0 - texColor.y;
      texColor.z = 1.0 - texColor.z;
   }

   // material color
   vec4 finalValue = vec4(MatAmb + (lightColor / lightDistance) * NdotL * (K + specular * (1.0 - K)), 1);

   // final color
   color = vec4(texColor.r*finalValue.r, texColor.g*finalValue.g,
      texColor.b*finalValue.b, opacity);
}
