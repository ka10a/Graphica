// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.4f, 0.8f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "/home/imroggen/OpenGL/ogl-master/tutorial04_colored_cube/TransformVertexShader.vertexshader", "/home/imroggen/OpenGL/ogl-master/tutorial04_colored_cube/ColorFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(5,5,5), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data2[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		//1
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		//2
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 //3
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		//4
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		//5
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		//6
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 //7
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 //8
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 //9
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		//10
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		//11
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
		 //12
	};
	static const GLfloat g_vertex_buffer_data[]{
            0.0f,0.0f,0.0f,
            0.0f,3.0f,3.0f,
            0.0f,0.0f,3.0f,
            0.0f,0.0f,0.0f,
            0.0f,3.0f,3.0f,
            0.0f,3.0f,0.0f,
            0.0f,0.0f,0.0f,
            8.0f,0.0f,3.0f,
            8.0f,0.0f,0.0f,
            0.0f,0.0f,0.0f,
            8.0f,0.0f,3.0f,
            0.0f,0.0f,3.0f,
            0.0f,0.0f,0.0f,
            8.0f,3.0f,0.0f,
            0.0f,3.0f,0.0f,
            0.0f,0.0f,0.0f,
            8.0f,3.0f,0.0f,
            8.0f,0.0f,0.0f,
            0.0f,0.0f,3.0f,
            8.0f,3.0f,3.0f,
            0.0f,3.0f,3.0f,
            0.0f,0.0f,3.0f,
            8.0f,3.0f,3.0f,
            8.0f,0.0f,3.0f,
            0.0f,3.0f,0.0f,
            8.0f,3.0f,3.0f,
            8.0f,3.0f,0.0f,
            0.0f,3.0f,0.0f,
            8.0f,3.0f,3.0f,
            0.0f,3.0f,3.0f,
            8.0f,0.0f,0.0f,
            8.0f,3.0f,3.0f,
            8.0f,0.0f,3.0f,
            8.0f,0.0f,0.0f,
            8.0f,3.0f,3.0f,
            8.0f,3.0f,0.0f,
            7.0f,0.0f,3.0f,
            7.0f,3.0f,6.0f,
            7.0f,0.0f,6.0f,
            7.0f,0.0f,3.0f,
            7.0f,3.0f,6.0f,
            7.0f,3.0f,3.0f,
            7.0f,0.0f,3.0f,
            11.0f,0.0f,6.0f,
            11.0f,0.0f,3.0f,
            7.0f,0.0f,3.0f,
            11.0f,0.0f,6.0f,
            7.0f,0.0f,6.0f,
            7.0f,0.0f,3.0f,
            11.0f,3.0f,3.0f,
            7.0f,3.0f,3.0f,
            7.0f,0.0f,3.0f,
            11.0f,3.0f,3.0f,
            11.0f,0.0f,3.0f,
            7.0f,0.0f,6.0f,
            11.0f,3.0f,6.0f,
            7.0f,3.0f,6.0f,
            7.0f,0.0f,6.0f,
            11.0f,3.0f,6.0f,
            11.0f,0.0f,6.0f,
            7.0f,3.0f,3.0f,
            11.0f,3.0f,6.0f,
            11.0f,3.0f,3.0f,
            7.0f,3.0f,3.0f,
            11.0f,3.0f,6.0f,
            7.0f,3.0f,6.0f,
            11.0f,0.0f,3.0f,
            11.0f,3.0f,6.0f,
            11.0f,0.0f,6.0f,
            11.0f,0.0f,3.0f,
            11.0f,3.0f,6.0f,
            11.0f,3.0f,3.0f,
            7.0f,0.0f,6.0f,
            7.0f,1.0f,7.0f,
            7.0f,0.0f,7.0f,
            7.0f,0.0f,6.0f,
            7.0f,1.0f,7.0f,
            7.0f,1.0f,6.0f,
            7.0f,0.0f,6.0f,
            8.0f,0.0f,7.0f,
            8.0f,0.0f,6.0f,
            7.0f,0.0f,6.0f,
            8.0f,0.0f,7.0f,
            7.0f,0.0f,7.0f,
            7.0f,0.0f,6.0f,
            8.0f,1.0f,6.0f,
            7.0f,1.0f,6.0f,
            7.0f,0.0f,6.0f,
            8.0f,1.0f,6.0f,
            8.0f,0.0f,6.0f,
            7.0f,0.0f,7.0f,
            8.0f,1.0f,7.0f,
            7.0f,1.0f,7.0f,
            7.0f,0.0f,7.0f,
            8.0f,1.0f,7.0f,
            8.0f,0.0f,7.0f,
            7.0f,1.0f,6.0f,
            8.0f,1.0f,7.0f,
            8.0f,1.0f,6.0f,
            7.0f,1.0f,6.0f,
            8.0f,1.0f,7.0f,
            7.0f,1.0f,7.0f,
            8.0f,0.0f,6.0f,
            8.0f,1.0f,7.0f,
            8.0f,0.0f,7.0f,
            8.0f,0.0f,6.0f,
            8.0f,1.0f,7.0f,
            8.0f,1.0f,6.0f,
            7.0f,2.0f,6.0f,
            7.0f,3.0f,7.0f,
            7.0f,2.0f,7.0f,
            7.0f,2.0f,6.0f,
            7.0f,3.0f,7.0f,
            7.0f,3.0f,6.0f,
            7.0f,2.0f,6.0f,
            8.0f,2.0f,7.0f,
            8.0f,2.0f,6.0f,
            7.0f,2.0f,6.0f,
            8.0f,2.0f,7.0f,
            7.0f,2.0f,7.0f,
            7.0f,2.0f,6.0f,
            8.0f,3.0f,6.0f,
            7.0f,3.0f,6.0f,
            7.0f,2.0f,6.0f,
            8.0f,3.0f,6.0f,
            8.0f,2.0f,6.0f,
            7.0f,2.0f,7.0f,
            8.0f,3.0f,7.0f,
            7.0f,3.0f,7.0f,
            7.0f,2.0f,7.0f,
            8.0f,3.0f,7.0f,
            8.0f,2.0f,7.0f,
            7.0f,3.0f,6.0f,
            8.0f,3.0f,7.0f,
            8.0f,3.0f,6.0f,
            7.0f,3.0f,6.0f,
            8.0f,3.0f,7.0f,
            7.0f,3.0f,7.0f,
            8.0f,2.0f,6.0f,
            8.0f,3.0f,7.0f,
            8.0f,2.0f,7.0f,
            8.0f,2.0f,6.0f,
            8.0f,3.0f,7.0f,
            8.0f,3.0f,6.0f,
            -1.0f,1.0f,2.0f,
            -1.0f,2.0f,3.0f,
            -1.0f,1.0f,3.0f,
            -1.0f,1.0f,2.0f,
            -1.0f,2.0f,3.0f,
            -1.0f,2.0f,2.0f,
            -1.0f,1.0f,2.0f,
            0.0f,1.0f,3.0f,
            0.0f,1.0f,2.0f,
            -1.0f,1.0f,2.0f,
            0.0f,1.0f,3.0f,
            -1.0f,1.0f,3.0f,
            -1.0f,1.0f,2.0f,
            0.0f,2.0f,2.0f,
            -1.0f,2.0f,2.0f,
            -1.0f,1.0f,2.0f,
            0.0f,2.0f,2.0f,
            0.0f,1.0f,2.0f,
            -1.0f,1.0f,3.0f,
            0.0f,2.0f,3.0f,
            -1.0f,2.0f,3.0f,
            -1.0f,1.0f,3.0f,
            0.0f,2.0f,3.0f,
            0.0f,1.0f,3.0f,
            -1.0f,2.0f,2.0f,
            0.0f,2.0f,3.0f,
            0.0f,2.0f,2.0f,
            -1.0f,2.0f,2.0f,
            0.0f,2.0f,3.0f,
            -1.0f,2.0f,3.0f,
            0.0f,1.0f,2.0f,
            0.0f,2.0f,3.0f,
            0.0f,1.0f,3.0f,
            0.0f,1.0f,2.0f,
            0.0f,2.0f,3.0f,
            0.0f,2.0f,2.0f,
            7.0f,0.0f,-1.0f,
            7.0f,1.0f,0.0f,
            7.0f,0.0f,0.0f,
            7.0f,0.0f,-1.0f,
            7.0f,1.0f,0.0f,
            7.0f,1.0f,-1.0f,
            7.0f,0.0f,-1.0f,
            8.0f,0.0f,0.0f,
            8.0f,0.0f,-1.0f,
            7.0f,0.0f,-1.0f,
            8.0f,0.0f,0.0f,
            7.0f,0.0f,0.0f,
            7.0f,0.0f,-1.0f,
            8.0f,1.0f,-1.0f,
            7.0f,1.0f,-1.0f,
            7.0f,0.0f,-1.0f,
            8.0f,1.0f,-1.0f,
            8.0f,0.0f,-1.0f,
            7.0f,0.0f,0.0f,
            8.0f,1.0f,0.0f,
            7.0f,1.0f,0.0f,
            7.0f,0.0f,0.0f,
            8.0f,1.0f,0.0f,
            8.0f,0.0f,0.0f,
            7.0f,1.0f,-1.0f,
            8.0f,1.0f,0.0f,
            8.0f,1.0f,-1.0f,
            7.0f,1.0f,-1.0f,
            8.0f,1.0f,0.0f,
            7.0f,1.0f,0.0f,
            8.0f,0.0f,-1.0f,
            8.0f,1.0f,0.0f,
            8.0f,0.0f,0.0f,
            8.0f,0.0f,-1.0f,
            8.0f,1.0f,0.0f,
            8.0f,1.0f,-1.0f,
            7.0f,2.0f,-1.0f,
            7.0f,3.0f,0.0f,
            7.0f,2.0f,0.0f,
            7.0f,2.0f,-1.0f,
            7.0f,3.0f,0.0f,
            7.0f,3.0f,-1.0f,
            7.0f,2.0f,-1.0f,
            8.0f,2.0f,0.0f,
            8.0f,2.0f,-1.0f,
            7.0f,2.0f,-1.0f,
            8.0f,2.0f,0.0f,
            7.0f,2.0f,0.0f,
            7.0f,2.0f,-1.0f,
            8.0f,3.0f,-1.0f,
            7.0f,3.0f,-1.0f,
            7.0f,2.0f,-1.0f,
            8.0f,3.0f,-1.0f,
            8.0f,2.0f,-1.0f,
            7.0f,2.0f,0.0f,
            8.0f,3.0f,0.0f,
            7.0f,3.0f,0.0f,
            7.0f,2.0f,0.0f,
            8.0f,3.0f,0.0f,
            8.0f,2.0f,0.0f,
            7.0f,3.0f,-1.0f,
            8.0f,3.0f,0.0f,
            8.0f,3.0f,-1.0f,
            7.0f,3.0f,-1.0f,
            8.0f,3.0f,0.0f,
            7.0f,3.0f,0.0f,
            8.0f,2.0f,-1.0f,
            8.0f,3.0f,0.0f,
            8.0f,2.0f,0.0f,
            8.0f,2.0f,-1.0f,
            8.0f,3.0f,0.0f,
            8.0f,3.0f,-1.0f,
            0.0f,2.0f,-1.0f,
            0.0f,3.0f,0.0f,
            0.0f,2.0f,0.0f,
            0.0f,2.0f,-1.0f,
            0.0f,3.0f,0.0f,
            0.0f,3.0f,-1.0f,
            0.0f,2.0f,-1.0f,
            1.0f,2.0f,0.0f,
            1.0f,2.0f,-1.0f,
            0.0f,2.0f,-1.0f,
            1.0f,2.0f,0.0f,
            0.0f,2.0f,0.0f,
            0.0f,2.0f,-1.0f,
            1.0f,3.0f,-1.0f,
            0.0f,3.0f,-1.0f,
            0.0f,2.0f,-1.0f,
            1.0f,3.0f,-1.0f,
            1.0f,2.0f,-1.0f,
            0.0f,2.0f,0.0f,
            1.0f,3.0f,0.0f,
            0.0f,3.0f,0.0f,
            0.0f,2.0f,0.0f,
            1.0f,3.0f,0.0f,
            1.0f,2.0f,0.0f,
            0.0f,3.0f,-1.0f,
            1.0f,3.0f,0.0f,
            1.0f,3.0f,-1.0f,
            0.0f,3.0f,-1.0f,
            1.0f,3.0f,0.0f,
            0.0f,3.0f,0.0f,
            1.0f,2.0f,-1.0f,
            1.0f,3.0f,0.0f,
            1.0f,2.0f,0.0f,
            1.0f,2.0f,-1.0f,
            1.0f,3.0f,0.0f,
            1.0f,3.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,0.0f,
            0.0f,0.0f,-1.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            1.0f,0.0f,0.0f,
            1.0f,0.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            1.0f,0.0f,0.0f,
            0.0f,0.0f,0.0f,
            0.0f,0.0f,-1.0f,
            1.0f,1.0f,-1.0f,
            0.0f,1.0f,-1.0f,
            0.0f,0.0f,-1.0f,
            1.0f,1.0f,-1.0f,
            1.0f,0.0f,-1.0f,
            0.0f,0.0f,0.0f,
            1.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,0.0f,
            1.0f,1.0f,0.0f,
            1.0f,0.0f,0.0f,
            0.0f,1.0f,-1.0f,
            1.0f,1.0f,0.0f,
            1.0f,1.0f,-1.0f,
            0.0f,1.0f,-1.0f,
            1.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            1.0f,0.0f,-1.0f,
            1.0f,1.0f,0.0f,
            1.0f,0.0f,0.0f,
            1.0f,0.0f,-1.0f,
            1.0f,1.0f,0.0f,
            1.0f,1.0f,-1.0f
	};


	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.84,0,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,
            1,0.9,0.05,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,

            0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,0.32, 0.26, 0.1,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    float radius = 30;
	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float camX = sin(glfwGetTime()) * radius;
        float camY = cos(glfwGetTime()) * radius;


        glm::mat4 View       = glm::lookAt(
                glm::vec3(camX,  -camY, 20), // Camera is at (4,3,-3), in World Space
                glm::vec3(4,-2*1.5,3), // and looks at the origin
                glm::vec3(0.5,0.5,1)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        glm::mat4 MVP = Projection * View * Model;
		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*9*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

