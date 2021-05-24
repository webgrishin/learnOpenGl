#version 460 core
out vec4 FragColor;

// uniform vec4 fragColor;
in vec4 fragColor;

void main()
{
	FragColor = fragColor;
	// FragColor = vec4(fragColor.r, fragColor.g, fragColor.b, 0.5f);
	// FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	// FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
