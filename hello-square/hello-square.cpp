#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

using namespace std;


// Function returns a unique ID for a shader
// type : GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
static unsigned int CompileShader( unsigned int type, const string& source)
{
    // Obtain a unique ID for the shader
        unsigned int id = glCreateShader(type);

    // Obtain char of the source code
        const char* src = &source[0];   // or source.c_str()

    // Specify source of the shader
        // shader : ID of the shader
        // count  : how many source codes
        
        // string : remember this is a pointer pointer, to the char source code
        // length : length of the string. 
        //        : nullptr for null-terminated. (i.e. You know how strings have \0 in the end)
        glShaderSource(id, 1, &src, nullptr);


    // Compile shader, passing the id
        glCompileShader(id);

    // Error handling : Checking to see if compilation was ok
    // Retrieving the result of the compiling process

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        // If unsuccessful, result will return GL_FALSE (GL_FALSE is = 0)
        if ( result == GL_FALSE )
        {
            //// Getting the error message
                // Getting the length of the message
                int length;
                glGetShaderiv(id,GL_INFO_LOG_LENGTH, &length);

                // Here's a bit tricky because length is determined on the fly, it's not constant
                // There's a work around this by using alloca(), where you can define on the stack
                char* message = (char*)alloca(length * sizeof(char));

                // Getting the log
                glGetShaderInfoLog(id, length, &length, message);

                // Print the message
                cout << "Failed to compile " << ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << " shader" << endl;
                cout << message << endl;

                // Delete shader so as not to proceed because of the error
                glDeleteShader(id);

                // Exit
                return 0; // it is 0 because function returns u-int

        }



    // Return the unique ID
        return id;
}


// Function to compile vertex and fragment shaders
static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader)
{
    // Create program which returns a unique ID
        unsigned int program = glCreateProgram();

    // Create the two shader objects, vertex and fragment
        
        // Vertex shader
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);

        // Fragment shader
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Shaders have been created
    // We now attach these shaders into our program
    // The function essentially takes in the ID (program is unsigned int after all)

        // Linking the vertex shader
        glAttachShader(program, vs);

        // Linking the fragment shader
        glAttachShader(program, fs);


    // Similar to C++ when you have multiple files, you have to do linking
        glLinkProgram(program);
    
    // This function checks if the program can be executed
        glValidateProgram(program);
    
    // Similar to C++, you'll get object files after you compile
    // We delete these for openGL
        glDeleteShader(vs);
        glDeleteShader(fs);

    // Returns the program's ID
        return program;
}



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
        cout << "openGL version: " << glGetString(GL_VERSION) << endl;


    //// Defining vertex buffer
        unsigned int buffer;    // actual memory of the buffer
        glGenBuffers(1, &buffer);   // 1 : how many buffers,
                                    // &buffer : buffer is assigned an ID
        
        // Selecting/binding the buffer
            glBindBuffer(GL_ARRAY_BUFFER, buffer);

            // GL_ARRAY_BUFFER : what sort of object is in the buffer
            // buffer : ID of the buffer

        // Key-in data

            // Float array that stores the data
            float positions[] = {
                -0.5f, -0.5f,   // index 0
                 0.5f, -0.5f,   // index 1
                 0.5f,  0.5f,   // index 2
                -0.5f,  0.5f    // index 3
            };

            glBufferData( GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), positions, GL_STATIC_DRAW );


        // Enable vertex attribute
            glEnableVertexAttribArray(0);

        // Vertex attribute
            glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 8, 0 );


    //// Defining index buffer

        unsigned int ibo;       // index buffer object
        glGenBuffers(1, &ibo);

            // NOTE: This has to be unsigned
            // Int array that stores the indices (going counter-clockwise in triangle)
            unsigned int indices[] = {
                0, 1, 2,        // right triangle
                2, 3, 0         // left triangle    
            };

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);









    //// Writing our shader source code
        // NOTE: Traditionally you write like this:
        //
        // string vertexShader =
        // "#version 330 core\n"
        // "\n"
        // "layout(location = 0) in vec4 position;\n"
        // "\n"
        // "void main(){\n"
        // "gl_Position = position;\n"
        // "}\n"

        // But using C++11's multiline string definitions, you can write it like this:
        const std::string vertexShader = R"glsl(
        #version 330 core

        layout(location = 0) in vec4 position;

        void main(){
            gl_Position = position;
        }
        )glsl";

        // layout(location = 0) in vec4 position;
            // What this is saying is, at index 0 of the vertex attribute is where position is stored
            // vec4 is how the position is stored (vec4, the vector has 4 floats)
                // NOTE: that our position is actually vec2
                    // We use vec4 because eventually the gl_Position in void main() has to be in vec4

        // Same thing for fragment shader
        const std::string fragmentShader = R"glsl(
        #version 330 core

        layout(location = 0) out vec4 color;

        void main(){
            color = vec4(1.0, 0.0, 0.0, 1.0);
        }
        )glsl";

        // color = vec4(1.0, 0.0, 0.0, 1.0); outputs a red color
            // range of 0 - 255 is changed to 0 - 1 here
            // vec4 (R,G,B,A)


    // Creating our shader
        unsigned int shader = CreateShader(vertexShader, fragmentShader);

    // Binding our shader
        glUseProgram(shader);


    //// Drawing a triangle with modern openGL
    // Loop until user closes the window
    while ( !glfwWindowShouldClose(window) )
    { 
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle (the currently bound buffer)
        // glDrawArrays(GL_TRIANGLES, 0, 6);       // 6 : we're drawing 6 vertices
        glDrawElements( GL_TRIANGLES, 
                        6, 
                        GL_UNSIGNED_INT, 
                        nullptr);  // because we have already a "bound" index buffer (ibo), we don't have to put anything here

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // Delete shader
    glDeleteShader(shader);
    
    glfwTerminate();
	return 0;
}