#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "math.h"
#include "time.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/shader.hpp>

GLFWwindow* window;
using namespace glm;

int main(void) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Homework 1 - Two triangles", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);



    GLuint programID_1 = LoadShaders( "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SimpleTransform.vertexshader", "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SingleColor.fragmentshader" );
    GLuint programID_2 = LoadShaders( "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SimpleTransform.vertexshader", "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SingleColor2.fragmentshader" );

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    GLuint MatrixID_1 = glGetUniformLocation(programID_1, "MVP");
    GLuint MatrixID_2 = glGetUniformLocation(programID_2, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    float angle = 0.0f;

    static const GLfloat g_vertex_buffer_data[] = {
            -0.4f,  0.8f, 0.0f,
            -0.8f,  -0.6f, 0.0f,
            0.6f,  -0.4f, 0.0f,
            -0.2f, -0.8f, 0.0f,
            0.2f,  0.6f, 0.0f,
            0.6f,  0.2f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

     //glEnable(GL_DEPTH_TEST);
     //glDepthFunc(GL_LESS);

    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float radius = 10.0f;

    do {
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glClear(GL_COLOR_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Camera matrix

        glm::mat4 View = glm::lookAt(
                glm::vec3(camX, camX, camZ),
                glm::vec3(0, 0, 0), // and looks at the origin
                glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*) 0           //  array buffer offset
        );

        glUseProgram(programID_1);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID_1, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(programID_2);
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID_2, 1, GL_FALSE, &MVP[0][0]);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);

    glDeleteProgram(programID_1);
    glDeleteProgram(programID_2);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
/*
 *
    GLuint programID = LoadShaders( "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SimpleTransform.vertexshader", "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SingleColor.fragmentshader" );
    GLuint programID2 = LoadShaders( "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SimpleTransform.vertexshader", "/home/imroggen/OpenGL/ogl-master/tutorial03_matrices/SingleColor.fragmentshader" );

 *
 *
