#if defined(VS_BUILD)
uniform mat4 in_Projection;
uniform mat4 in_View;
uniform mat4 in_Model;

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;

out vec2 ex_TexCoord;

void main()
{
    gl_Position = in_Projection * in_View * in_Model * vec4(in_Position, 1.0);
    ex_TexCoord = in_TexCoord;
}

// ------------------------------------------------------------------------------

#elif defined(FS_BUILD)
uniform sampler2D in_Texture;

in vec2 ex_TexCoord;

out vec4 FragColor;

void main()
{
  vec4 tex = texture(in_Texture, ex_TexCoord);
  FragColor = tex;
}

#endif
