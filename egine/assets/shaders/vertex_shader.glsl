#version 300 es
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    layout (location = 2) in vec2 texCoord;


    out vec2 vTexCoord;

    uniform mat4 uModel;
    uniform mat4 uView;
    uniform mat4 uProjection;

    void main() {

        vTexCoord=texCoord;
        gl_Position = uProjection * uView * uModel * vec4(position,1.0);
    }