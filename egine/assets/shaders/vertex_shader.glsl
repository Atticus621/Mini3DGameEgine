#version 300 es
    precision mediump float;
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 texCoord;
    layout (location = 3) in vec3 normal;

    out vec2 vTexCoord;
    out vec3 vNormal;
    out vec3 vFragPos;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main() {
        vFragPos = vec3(uModel * vec4(position,1.0));
        vNormal = mat3(transpose(inverse(uModel))) *normal;
        vTexCoord=texCoord;
        gl_Position = uProjection * uView * uModel * vec4(position,1.0);
    }