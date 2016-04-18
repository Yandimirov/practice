version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec4 Color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec4 ourColor;
void main()
{
    gl_Position = projection * view * model * vec4(position + offset, 1.0f);
    ourColor = Color;
}
