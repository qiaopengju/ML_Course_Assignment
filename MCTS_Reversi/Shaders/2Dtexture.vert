#version 330 core //source code of vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 vertexColor;   // 向片段着色器输出颜色
out vec2 TexCoord;

uniform mat4 model;

void main(){
    gl_Position = model * vec4(aPos, 1.0);
    
    TexCoord = aTexCoord;
}