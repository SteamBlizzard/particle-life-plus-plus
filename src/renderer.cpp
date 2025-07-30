#include "renderer.h"

#include <glm/glm.hpp>

Renderer::Renderer(Shader &shader) : shader(shader)
{
    initRenderData();
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::initRenderData()
{
    unsigned int VBO;
    float vertices[] = {
        // x, y
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f
    };

    // Instantiate Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    // Bind VBO and copy vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Bind VAO and set vertex attribute pointers
    glBindVertexArray(quadVAO);
    // (position 0, 2 floats per vertex, no normalization, stride of 2 floats, offset 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    // Enable vertex attribute
    glEnableVertexAttribArray(0);

    // Unbind the VAO (rebind when rendering)
    glBindVertexArray(0);
}

void Renderer::render(
    const glm::vec2 position,
    const glm::vec2 size,
    float rotation,
    const glm::vec4 color)
{
    // Use the shader program
    shader.Use();

    // Set the color uniform
    shader.SetVec4f("color", color, true);

    // Calculate the model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // Set the model matrix uniform
    shader.SetMat4("model", model, true);

    // Bind the VAO and draw the quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Unbind the VAO
    glBindVertexArray(0);
}