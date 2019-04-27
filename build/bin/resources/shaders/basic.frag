#version 430

in vec3 color;
out vec4 fragColour;

void main() {
   fragColour = vec4(color, 1.0);
}