#version 330 // source code of fragment shader
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1; //采样器1

void main(){
    vec4 color = texture(texture1, TexCoord);
    if (color.a < 0.1) discard;
    FragColor = color;
}