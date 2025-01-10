#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
out vec4 colour;

struct DirectionalLight
{
    vec3 colour;
    float ambientIntesity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntesity;
    
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour);
}
