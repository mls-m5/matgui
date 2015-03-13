/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


namespace PlainShader {

const char *vertexCode = R"V0G0N(
attribute vec4 vPosition;
uniform vec4 uColor;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
varying vec4 fColor;

void main() {
	gl_Position = mvp_matrix * vPosition;
	fColor = uColor / (1. + vPosition.y / 4.); //A little shading
}
)V0G0N";


const char *fragmentCode = R"V0G0N(
varying vec4 fColor;

void main() {
	gl_FragColor = fColor;
	//gl_FragColor = vec4(0,0,1,0);
}
)V0G0N";


}



