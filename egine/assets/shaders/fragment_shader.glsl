#version 300 es
    precision mediump float;

    in vec3 vColor;
    in vec2 vTexCoord;

    uniform sampler2D breakTexture;
    
    out vec4 FragColor;

    void main() {
        vec4 texColor = texture(breakTexture,vTexCoord);
        FragColor = vec4(vColor, 1.0) * texColor;
    }