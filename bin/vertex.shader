#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform vec2 offset;
out vec4 colorOut;
uniform mat4 perspectiveMatrix;
uniform float zFar;

void main(){
	vec4 cameraPos = position + vec4(offset.x, offset.y, 0.0, 0.0);
	gl_Position = perspectiveMatrix * cameraPos;

	colorOut = color;
}
