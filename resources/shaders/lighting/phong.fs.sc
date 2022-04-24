#include "../common.sc"

precision mediump float;
varying vec3 normalInterp;  // Surface normal
varying vec3 vertPos;       // Vertex position
uniform int mode;   // Rendering mode
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess
// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position

void main() {
  vec3 N = normalize(normalInterp);
  vec3 L = normalize(lightPos - vertPos);

  // Lambert's cosine law
  float lambertian = max(dot(N, L), 0.0);
  float specular = 0.0;
  if(lambertian > 0.0) {
    vec3 R = reflect(-L, N);      // Reflected light vector
    vec3 V = normalize(-vertPos); // Vector to viewer
    // Compute the specular term
    float specAngle = max(dot(R, V), 0.0);
    specular = pow(specAngle, shininessVal);
  }
  gl_FragColor = vec4(
      add(
          add(
                mul(Ka, ambientColor),
                mul(
                    mul(Kd, lambertian),
                    diffuseColor
                )
            ),
                mul(
                    mul(Ks, specular),
                    specularColor
                )
        ), 1.0);

  // only ambient
  if(mode == 2) gl_FragColor = vec4(mul(Ka, ambientColor), 1.0);
  // only diffuse
  if(mode == 3) gl_FragColor = vec4(mul(mul(Kd, lambertian), diffuseColor), 1.0);
  // only specular
  if(mode == 4) gl_FragColor = vec4(mul(mul(Ks, specular), specularColor), 1.0);
}
