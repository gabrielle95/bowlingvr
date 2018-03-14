#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform int max_shadow_matrices;
uniform mat4 shadowMatrices[max_shadow_matrices];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
	int n = 0;
	while(n < max_shadow_matrices)
	{
		for(int face = 0; face < 6; ++face)
		{
			gl_Layer = face; // built-in variable that specifies to which face we render.
			for(int i = 0; i < 3; ++i) // for each triangle's vertices
			{
				FragPos = gl_in[i].gl_Position;
				gl_Position = shadowMatrices[face] * FragPos;
				EmitVertex();
			}    
			EndPrimitive();
		}
		n = n + 6;
	}
} 