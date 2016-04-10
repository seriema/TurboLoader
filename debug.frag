#version 150 core

smooth in vec4 col;

out vec4 fragColor;



void main (void)
{
    fragColor = col;
}