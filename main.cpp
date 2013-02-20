#include <GL/glew.h>
#include <GL/glfw.h>

#include <memory>

#include "MainApp.h"

//TEMP
#include "SoundGenerator.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

MainApp app;
int mouse_x, mouse_y;

void GLFWCALL resize(int w, int h)
{
    app.OnResize(w,h);
}

void GLFWCALL mousepos(int x, int y)
{
    mouse_x = x;
    mouse_y = y;
    app.OnMouseMove(x, y);
}

void GLFWCALL mousebutton(int button, int action)
{
    switch(action)
    {
    case GLFW_PRESS:
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            app.OnMouseDown(mouse_x, mouse_y, MainApp::MOUSE_LEFT);
        }

        if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            app.OnMouseDown(mouse_x, mouse_y, MainApp::MOUSE_RIGHT);
        }

        if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            app.OnMouseDown(mouse_x, mouse_y, MainApp::MOUSE_MIDDLE);
        }
        
        break;
    case GLFW_RELEASE:
        if(button == GLFW_MOUSE_BUTTON_LEFT)
        {
            app.OnMouseUp(mouse_x, mouse_y, MainApp::MOUSE_LEFT);
        }

        if(button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            app.OnMouseUp(mouse_x, mouse_y, MainApp::MOUSE_RIGHT);
        }

        if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            app.OnMouseUp(mouse_x, mouse_y, MainApp::MOUSE_MIDDLE);
        }

        break;
    }
}

int main(int argc, char *argv[])
{
	if(!glfwInit())
    {
        return -1;
    }

    int test = 0x00000001;
    char test2 = 0;

    memcpy(&test2, &test, 1);

    //Create GL context
    glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 8, 8, 8, 8, 8, 8, GLFW_WINDOW);

    //Initialize GLEW
    GLenum err = glewInit();
    if(err != GLEW_NO_ERROR)
    {
        const unsigned char *str = glewGetErrorString(err);
        return -1;
    }

    //Callbacks
    glfwSetWindowSizeCallback(resize);
    glfwSetMouseButtonCallback(mousebutton);
    glfwSetMousePosCallback(mousepos);

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