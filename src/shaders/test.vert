#version 410\n
in vec3 vertexPosition;
void main() {
   gl_Position = vec4(vertexPosition, 1.0);
}