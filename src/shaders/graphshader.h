/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


namespace GraphShader {

const char *vertexCode = R"V0G0N(
attribute float vX, vY;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
void main() {
  gl_Position = mvp_matrix * vec4(vX, vY, 1, 1);
}
)V0G0N";


const char *fragmentCode = R"V0G0N(
void main() {
  gl_FragColor = vec4(1,1,1,1);
}
)V0G0N";


}
