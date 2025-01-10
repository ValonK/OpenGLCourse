#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 colour;

struct DirectionalLight
{
    vec3 colour;
    float ambientIntesity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material
{
    float specularIntesity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main()
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntesity;
    
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColour = vec4(0, 0, 0, 0);
    if(diffuseFactor > 0.f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if(specularFactor > 0.f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(directionalLight.colour * material.specularIntesity * specularFactor, 1.f);
        }
    }

    colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);
}
