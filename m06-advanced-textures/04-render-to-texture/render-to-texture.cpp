

#include "sharing.h"

#include "cs4722/window.h"
#include "cs4722/callbacks.h"

/*
 * This example is more complex since there are two different shader programs to support
 * two different rendering setups:
 *  * Rendering a scene made of parts
 *  * Rendering the 'view in view'
 *
 *  The scene made of parts is actually going to be rendered twice on each iteration
 *  of the event loop
 *
 *   * Render to a texture contained in a frame buffer to paint on the view-in-view
 *   * Render to the usual default render buffer in the visible window
 *
 *    The view-in-view, just a rectangle, will always be rendered directly to the
 *    window frame buffer.
 *
 *
 *  Each set of shader programs has its own file
 *  * setup_parts.cpp for the parts making the main scene
 *  * setup_view_in_view for the view-in-view rectangle
 *
 *  Each file contains a display function for rendering using the corresponding shaders
 *  and an init function for setting things up.
 *  The parts file also has functions to change the rendering target to a framebuffer or to
 *  a window.
 *  The sharing.h include file is used by this file to access those six functions as needed.
 *
 */


int
main(int argc, char** argv)
{
    glfwInit();

    auto *window = cs4722::setup_window("View in View", .9);


    gladLoadGL(glfwGetProcAddress);
    glDebugMessageCallback(cs4722::message_callback, nullptr);


    auto *view = new cs4722::view();
//    view->enable_logging = false;

    view->set_flup(glm::vec3(0.257654,0.361615,-0.896018),
                   glm::vec3(-0.961056,0,-0.276356),
                   glm::vec3(-0.0999345,0.932327,0.347532),
                   glm::vec3(0,0,-0.3));

    view->perspective_fovy = (.9 * M_PI);

    // initialize the two sub-scenes
    scene_setup(view);
    view_in_view_setup(view);

    glfwSetWindowUserPointer(window, view);

    glfwSetKeyCallback(window, cs4722::general_key_callback);
    glfwSetCursorPosCallback(window, cs4722::move_callback);
    glfwSetWindowSizeCallback(window, cs4722::window_size_callback);
	
    while (!glfwWindowShouldClose(window))
    {
        // set up the frame buffer for rendering to a texture
        scene_setup_for_fb();
        // clear that buffer
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::olive_drab.as_float());
        glClear(GL_DEPTH_BUFFER_BIT);
        // reverse the camera
        view->camera_forward = -view->camera_forward;
        view->camera_left = -view->camera_left;
        // call the display function for parts to render the scene into the
        //    texture contained in the framebuffer
        scene_display();
        // turn the camera back
        view->camera_forward = -view->camera_forward;
        view->camera_left = -view->camera_left;

        //  shift the sub-scene rendering to the window
        scene_setup_for_window(window);
        // clear the window frame buffer
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float());
        glClear(GL_DEPTH_BUFFER_BIT);
        // render the parts subscene into the window frame buffer
        scene_display();
        // render the view-in-view to the window frame buffer
        //    The texture created in the earlier rendering is used to cover the rectangle
        view_in_view_display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
