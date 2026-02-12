#version 300 es
    precision mediump float;

    struct Light{
        vec3 position;
        vec3 color;
    };
    uniform Light uLight;
    uniform sampler2D baseColorTexture;
    
    in vec2 vTexCoord;
    in vec3 vNormal;
    in vec3 vFragPos;
    out vec4 FragColor;

    void main() {
        vec3 norm = normalize(vNormal);
        vec3 lightDir = normalize(uLight.position - vFragPos);
        float diff = max(dot(norm,lightDir),0.0);
        vec3 diffuse = diff * uLight.color;

        vec4 texColor = texture(baseColorTexture,vTexCoord);
        FragColor = texColor * vec4(diffuse,1.0);
    }