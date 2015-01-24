/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


namespace TextureShader {

const char *vertexCode = R"V0G0N(
attribute vec4 vPosition;
uniform vec4 uColor;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
varying vec4 fColor;

void main() {
	gl_Position = mvp_matrix * vPosition;
	//fColor = uColor;
	fColor = vec4(vPosition.x, vPosition.y, 0, 0);
	//fColor = vec4(1,1,1,1);
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[0] = vPosition;
}
)V0G0N";


const char *fragmentCode = R"V0G0N(
varying vec4 fColor;
uniform sampler2D texture1;

void main() {
	float s = texture2D(texture1, fColor.xy).a;
	gl_FragColor = texture2D(texture1, fColor.xy) * s;
}
)V0G0N";


}
