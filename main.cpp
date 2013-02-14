#include <GL/glew.h>
#include <GL/glfw.h>

#include "MainApp.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

int main(int argc, char *argv[])
{
	if(!glfwInit())
    {
        return -1;
    }

    //Create GL context
    glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 8, 8, 8, 8, 8, 8, GLFW_WINDOW);

    //Initialize GLEW
    GLenum err = glewInit();
    if(err != GLEW_NO_ERROR)
    {
        const unsigned char *str = glewGetErrorString(err);
        return -1;
    }

    //Instance of the main app
    MainApp app;

    app.Init();
    app.OnResize(WINDOW_WIDTH, WINDOW_HEIGHT);

	while(1) {
        if(glfwGetKey(GLFW_KEY_ESC)) break;

        //Clear the existing screen data
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Update app logic
		app.Update();

        //Flip the screen buffer
        glfwSwapBuffers();
	}

    app.ShutDown();

	return 0;
}