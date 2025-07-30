#include "renderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

#include "constants.h"

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

    unsigned int EBO;
    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3
    };

    // Instantiate Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO and set vertex attribute pointers
    glBindVertexArray(quadVAO);

    // Bind VBO and copy vertex data to it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind EBO and copy index data to it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define vertex attribute structure
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
    shader.SetVec4f("color", color);

    // Calculate the model matrix
    glm::mat4 model = glm::mat4(1.0f);

    // Translate
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // Rotate and scale relative to center
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));
    

    // Set the model matrix uniform
    shader.SetMat4("model", model);

    // Bind the VAO and draw the quad
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    // Unbind the VAO
    glBindVertexArray(0);
}