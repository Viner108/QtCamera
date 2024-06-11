#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D image;
uniform float width; // ширина изображения
uniform float height; // высота изображения

void main() {
    vec2 uv = TexCoord;
    float gradient = uv.x; // использование x-координаты как градиент от 0 до 1

    float kernelX[9] = float[9](-1, 0, 1, -2, 0, 2, -1, 0, 1);
    float kernelY[9] = float[9]( 1, 2, 1,  0, 0, 0, -1, -2, -1);

    float gx = 0.0;
    float gy = 0.0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            vec4 color = texture(image, uv + vec2(i, j) * vec2(1.0 / width, 1.0 / height));
            float gray = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
            gx += gray * kernelX[(i + 1) * 3 + (j + 1)];
            gy += gray * kernelY[(i + 1) * 3 + (j + 1)];
        }
    }

    float edge = sqrt(gx * gx + gy * gy);

    vec4 edgeColor = vec4(vec3(edge), 1.0);
    vec4 originalColor = texture(image, uv);

    FragColor = mix(originalColor, edgeColor, gradient); // смешиваем исходное изображение и фильтр Собеля с использованием градиента
}
