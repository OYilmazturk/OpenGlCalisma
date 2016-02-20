#version 330

in vec4 colorOut;
out vec4 color;

uniform sampler2D ourTexture;

void main() {
	color = colorOut; 
}
