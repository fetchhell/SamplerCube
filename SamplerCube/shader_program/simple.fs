#version 330

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D texture;

void main() {
    
    FragColor = texture2D(texture, vTexCoord);
}