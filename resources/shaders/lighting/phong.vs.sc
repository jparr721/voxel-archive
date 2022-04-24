#include "../common.sc"

attribute vec3 position;
attribute vec3 normal;
uniform mat4 projection, modelview, normalMat;
varying vec3 normalInterp;
varying vec3 vertPos;

void main(){
  vec4 vertPos4 = mul(modelview, vec4(position, 1.0));
  vertPos = div(vec3(vertPos4), vertPos4.w);
  normalInterp = vec3(mul(normalMat, vec4(normal, 0.0)));
  gl_Position = mul(projection, vertPos4);
}