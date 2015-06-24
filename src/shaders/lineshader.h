/*
 * lineshader.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once



namespace LineShader {

const char *vertexCode = R"V0G0N(
attribute vec2 v;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
void main() {
  gl_Position = mvp_matrix * vec4(v.x, v.y, 1, 1);
}
)V0G0N";


const char *fragmentCode = R"V0G0N(
uniform  vec4 uColor;
void main() {
  gl_FragColor = vec4(uColor.x, uColor.y, uColor.z, 1);
}
)V0G0N";


}


