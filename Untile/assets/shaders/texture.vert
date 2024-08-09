#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec4 aColor;
layout (location = 1) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 transform = mat4(1.f);

uniform mat4 model = mat4(1.f);
uniform mat4 view = mat4(1.f);
uniform mat4 projection = mat4(1.f);

void main()
{
	// gl_Position = transform * vec4(aPos, 1.0);
	gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
	// ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
