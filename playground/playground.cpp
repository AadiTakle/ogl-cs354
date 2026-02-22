// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>

int main(void)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	int width = 1024, height = 768;
	window = glfwCreateWindow(1024, 768, "My Playground!", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor
	glfwPollEvents(); // ensure internal state is updated
	glfwSetCursorPos(window, width / 2, height / 2); // seed cursor at center

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
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint CubeVertexArrayID;
	glGenVertexArrays(1, &CubeVertexArrayID);
	glBindVertexArray(CubeVertexArrayID);

	GLuint TriangleVertexArrayID;
	glGenVertexArrays(1, &TriangleVertexArrayID);
	glBindVertexArray(TriangleVertexArrayID);

	// Create and compile our GLSL program from the shaders
	// shaders stored in exe directory, not in project for some reason...
	GLuint cubeProgramID = LoadShaders("CubeVertexShader.vertexshader", "CubeFragmentShader.fragmentshader");
	GLuint triangleProgramID = LoadShaders("TriangleVertexShader.vertexshader", "TriangleFragmentShader.fragmentshader");


	// Hard coded cube goes crazy
	// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_cube_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	GLuint cubevertexbuffer;
	glGenBuffers(1, &cubevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertex_buffer_data), g_cube_vertex_buffer_data, GL_STATIC_DRAW);

	static const GLfloat g_triangle_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint trianglevertexbuffer;
	glGenBuffers(1, &trianglevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trianglevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_vertex_buffer_data), g_triangle_vertex_buffer_data, GL_STATIC_DRAW);

	// Vertex colors
	// One color for each vertex. They were generated randomly.
	GLfloat g_cube_color_buffer_data[108];
	// Seed random number generator
	std::srand(static_cast <unsigned int> (std::time(0)));
	// Fill the color buffer with random colors
	for (int i = 0; i < 108; i++) {
		g_cube_color_buffer_data[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	GLuint cubecolorbuffer;
	glGenBuffers(1, &cubecolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_color_buffer_data), g_cube_color_buffer_data, GL_STATIC_DRAW);

	static const GLfloat g_triangle_color_buffer_data[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	GLuint trianglecolorbuffer;
	glGenBuffers(1, &trianglecolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trianglecolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_color_buffer_data), g_triangle_color_buffer_data, GL_STATIC_DRAW);

	// Make MVP uniform in shader
	GLuint CubeMatrixID = glGetUniformLocation(cubeProgramID, "MVP");
	GLuint TriangleMatrixID = glGetUniformLocation(triangleProgramID, "MVP");

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(cubeProgramID);

		// process OS events first so cursor position is up-to-date
		glfwPollEvents();

		// Compute the MVP matrix from keyboard and mouse input
		glm::mat4 CubeModel = glm::mat4(1.0f);
		// Cube is centered at model origin, so object center is (0, 0, 0) in model space, convert to world space with model matrix
		glm::vec4 objectCenter = CubeModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		computeMatricsFromObjectRotation(objectCenter.xyz);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		// Model matrix: an identity matrix (model will be at the origin)
		// Our ModelViewProjection: multiplication of our 3 matrices
		glm::mat4 cubeMVP = ProjectionMatrix * ViewMatrix * CubeModel; // Remember, matrix multiplication is the other way around
		// Model matrix: an identity matrix (model will be at the origin)
		mat4 TriangleModel = mat4(1.0f);
		//Translate triangle right 5 units
		TriangleModel = translate(TriangleModel, vec3(4.0f, 0.0f, -1.0f));
		// Rotate triangle 90 degrees around Z axis
		// TriangleModel = rotate(TriangleModel, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		// Scale triangle to double size
		//TriangleModel = scale(TriangleModel, vec3(2.0f, 2.0f, 2.0f));
		// Our ModelViewProjection: multiplication of our 3 matrices
		mat4 triangleMVP = ProjectionMatrix * ViewMatrix * TriangleModel; // Remember, matrix multiplication is the other way around

		// Give our MVP to shader uniform
		glUniformMatrix4fv(CubeMatrixID, 1, GL_FALSE, &cubeMVP[0][0]);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cubevertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the cube !
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Use our shader
		glUseProgram(triangleProgramID);

		// Give our MVP to shader uniform
		glUniformMatrix4fv(TriangleMatrixID, 1, GL_FALSE, &triangleMVP[0][0]);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, trianglevertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0: vertex positions
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, trianglecolorbuffer);
		glVertexAttribPointer(
			1,                                // attribute 1: vertex colors
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &cubevertexbuffer);
	glDeleteVertexArrays(1, &CubeVertexArrayID);
	glDeleteBuffers(1, &trianglevertexbuffer);
	glDeleteVertexArrays(1, &TriangleVertexArrayID);
	glDeleteProgram(cubeProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

