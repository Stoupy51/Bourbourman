#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 _M, _V, _P;
uniform mat3 _iTM;

out vec3 color;
out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main ()
{
	gl_Position = _P * _V * _M * vec4 (aPos, 1.0);

	color = aColor;
	fragPos = vec3(_M * vec4(aPos, 1.0));
    normal = _iTM * aNormal;
	texCoords = aTexCoord;
}