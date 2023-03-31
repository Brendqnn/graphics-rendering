#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "vbo.h"
#include "vao.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800


static const char *VERTEX_SHADER =
    "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "   gl_Position = projection * view * model * vec4(position, 1.0);\n"
    "}\n";

static const char *FRAGMENT_SHADER =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "   fragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
    "}\n";


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Spinning Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set the viewport size
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	GLuint shader = shader_load(VERTEX_SHADER, FRAGMENT_SHADER);

    GLuint vbo;
	setup_vbo(vbo);

    GLuint vao;
	setup_vao(vao);

    // Set up the model, view, and projection matrices
	mat4 model;
	mat4 view;
	mat4 projection;
	glm_mat4_identity(model);
	glm_mat4_identity(view);
	glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});
	glm_perspective(glm_rad(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f, projection);


    // Get the location of the model, view, and projection uniform variables in the shader program
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projectionLoc = glGetUniformLocation(shader, "projection");

    // Set up the rendering loop
    double previousTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate the time delta
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        // Rotate the model matrix
        glm_rotate(model, deltaTime * 10.0f, (vec3){0.0f, 1.0f, 0.0f});

        // Set the model, view, and projection matrices in the shader program
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

        // Draw the triangle
        glUseProgram(shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader);
    glfwTerminate();
    return EXIT_SUCCESS;
}


	 















