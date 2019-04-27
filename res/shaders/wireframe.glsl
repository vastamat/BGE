#if defined(VS_BUILD)
uniform mat4 in_Projection;
uniform mat4 in_View;
uniform mat4 in_Model;

layout (location = 0) in vec3 in_Position;

void main()
{
    gl_Position = in_Projection * in_View * in_Model * vec4(in_Position, 1.0);
}

// ------------------------------------------------------------------------------

#elif defined(FS_BUILD)

out vec4 FragColor;

void main()
{
  FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif