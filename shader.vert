R"(
#version 150

in vec3 vert;
in vec3 clr;

out vec4 col;

uniform mat4 vMat;// model -> cam
uniform mat4 pMat;// cam -> clip

void main (void)
{
    gl_Position = pMat * vMat * vec4(vert, 1.f);
    col = vec4(clr, 1.f);
}
)"
