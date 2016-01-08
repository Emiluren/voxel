#version 330
in vec2 UV;
in vec3 normal;
//in vec3 worldPosition;

out vec3 color;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    vec3 Direction;
    float DiffuseIntensity;
};
uniform DirectionalLight light;
uniform sampler2D textureSampler;

//uniform vec3 eyePosition;
//uniform float specularIntensity;
//uniform float specularPower;

void main()
{
    vec3 ambientColor = light.Color * light.AmbientIntensity;
    float diffuseFactor = dot(normalize(normal), -light.Direction);

    vec3 diffuseColor = vec3(0, 0, 0);
    //vec3 specularColor = vec3(0, 0, 0);

    if (diffuseFactor > 0)
    {
        diffuseColor = light.Color * diffuseFactor * light.DiffuseIntensity;

        /*vec3 vertToEye = normalize(eyePosition - worldPosition);
        vec3 lightReflect = normalize(reflect(light.Direction, normal));
        float specularFactor = dot(vertToEye, lightReflect);
        specularFactor = pow(specularFactor, specularPower);

        if (specularFactor > 0)
            specularColor = light.Color * specularIntensity * specularFactor;*/
    }

    color = texture(textureSampler, UV).rgb *
        (ambientColor + diffuseColor /*+ specularColor*/);
}
