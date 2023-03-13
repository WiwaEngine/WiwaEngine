#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D u_Texture;


void main()
{
	vec4 texColor = texture(u_Texture, texCoord);
    
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}