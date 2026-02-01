#version 460 core

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    mat3 TBN;
} fs_in;

out vec4 FragColor; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 diffuseColor  = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 specularColor = texture(texture_specular1, fs_in.TexCoords).rgb;

    // normal from normal map (in tangent space)
    vec3 tangentNormal = texture(texture_normal1, fs_in.TexCoords).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;

    vec3 norm     = normalize(fs_in.TBN * tangentNormal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 viewDir  = normalize(viewPos - fs_in.FragPos);

    // ambient
    vec3 ambient = 0.15 * diffuseColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor;

    // specular (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * specularColor * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0); // ? correct output
}
