#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

using namespace std;

int main(int argc,char** argv)
{

    // Initialize the glfw library
    if (!glfwInit())
        return -1;

    // Create a window
    GLFWwindow* window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initiate GLEW
    // NOTE: GLEW can only be initiated after a valid rendering context
    
    if ( glewInit() != GLEW_OK )    
    {
        cout << "Error" << endl;
        return -1;

    }

    // To check openGL version
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;


    // Loop until user closes the window
    while ( !glfwWindowShouldClose(window) )
    {   
        
        glClear(GL_COLOR_BUFFER_BIT);

        // Create a triangle
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f, 0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


	return 0;
}