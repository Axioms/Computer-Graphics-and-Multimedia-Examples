
/*
 * Window events are events caused by windows.
 *  These events report changes to the associated window.
 *  For example, if the user drags the edges of a window to change the window size, an event would be generated.
 *  By reacting to a window changing size, we can scale the rendering of the scene to the new window.
 *
 *  This example also shows how to take an application to full screen and back.
 *      The F11 key will be used to toggle between full screen and back.
 *
 *  Finally, the escape key will be 'wired' to terminate the application.
 *
 *  Note: the window for this example starts out very small in order to make it very clear what
 *      changing the window size is doing.
 */

#include "GLM/gtc/type_ptr.hpp"


#include <glad/gl.h>


#include "cs4722/callbacks.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/compile_shaders.h"


static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> artf_list;

static GLuint transform_loc;


void init()
{

    the_view = new cs4722::view();
    the_view->camera_position = glm::vec3(0, 0, 3);

    program = cs4722::compile_shaders("vertex_shader.glsl","fragment_shader.glsl");
    glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");

    glEnable(GL_DEPTH_TEST);


    
    cs4722::shape* b = new cs4722::sphere(15, 50);
    auto* artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(.5, -.5, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.5f, .5f, .5f));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);

    artf_list.push_back(artf);


    cs4722::shape* b1 = new cs4722::block();
    auto* artf1 = new cs4722::artifact_rotating();
    artf1->the_shape = (b1);
    artf1->world_transform.translate = (glm::vec3(-.5, 0, 0));
    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf1->rotation_rate = (M_PI / 3);
    artf1->world_transform.scale = (glm::vec3(.2f, .6f, .2f));
    artf1->animation_transform.rotation_center =
            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);

    artf_list.push_back(artf1);


    cs4722::init_buffers(program, artf_list, "bPosition", "bColor");

}


//----------------------------------------------------------------------------
//
// display
//

auto last_time = 0.0;

void
display()
{
    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto artf : artf_list) {

        artf->animate(time, delta_time);

        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

        
        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
       

    }
}


float lr_scale = .1f;
float fb_scale = .1f;
float ud_scale = .1f;

float lr_pan_scale = .01f;
float ud_pan_scale = lr_pan_scale;


/*
 * These variables are used to hold the window height and width and position to use when
 *  exiting full screen.
 */
static int old_width, old_height, old_xpos, old_ypos;

void general_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    // the_scene->the_view->key_callback(window, key, scancode, action, mods);
    // std::cout << "key " << key << std::endl;
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
    {
        // only process key presses and repeats
    }
    else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
    {
        view->camera_position += lr_scale * view->camera_left;
    }
    else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
    {
        view->camera_position -= lr_scale * view->camera_left;
    }
    else if (key == GLFW_KEY_W)
    {
        view->camera_position += fb_scale * view->camera_forward;
    }
    else if (key == GLFW_KEY_S)
    {
        view->camera_position -= fb_scale * view->camera_forward;
    }
    else if (key == GLFW_KEY_UP)
    {
        view->camera_position += ud_scale * view->camera_up;
    }
    else if (key == GLFW_KEY_DOWN)
    {
        view->camera_position -= ud_scale * view->camera_up;
    }
    else if (key == GLFW_KEY_ESCAPE)
    {
        /*
         * If the escape key is pressed, then the following function will set the GLFW
         * flag variable 'windowShouldClose' to true.
         * Then the event loop, in function main, and the program will terminate.
         * This is the proper way to terminate the program since it gives GLFW an opportunity to
         *      clean up the resources it is using.
         */
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        /*
         * If the F11 key is pressed, but not a repeat generated by a continued press,
         * then toggle between full screen and windowed.
         */

        /*
         * If the monitor attribute of the window is set, not NULL, then
         *  the window is currently in full screen according to GLFW conventions.
         *
         */
        if (!glfwGetWindowMonitor(window)) {
            /*
             * This code is executed if the monitor attribute of the window is NULL.
             * That means that the window is currently not full screen.
             * So, execute code to take it to full screen.
             */

            /*
             * Save the window position and size.
             * The variables used to save these values are defined just above the callback function header.
             * A pointer to each variable is passed (&old_xpos is the address in memory of the variable old_xpos).
             * In this way the function can change the values of those variables.
             */
            glfwGetWindowPos(window, &old_xpos, &old_ypos);
            glfwGetWindowSize(window, &old_width, &old_height);
            /*
             * Get an object representing the primary monitor.
             *
             * While it would be nicer to figure out which monitor is being used on a multiple monitor setup,
             *      that doesn't add much to the example.
             */
            auto monitor = glfwGetPrimaryMonitor();
            /*
             * The mode of a monitor in GLFW is an object that contains attributes of the monitor, such as
             * its resolution.
             */
            const auto* mode = glfwGetVideoMode(monitor);
            /*
             * Now switch to full-screen.
             * The second parameter is a monitor object, not NULL, so that indicates full-screen.
             * It isn't necessary to use the resolution of the monitor for width and height.
             *      However, using other values will force a change of resolution
             */
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else
        {
            /*
             * This code is reached if F11 was pressed while the application is in full-screen mode.
             * Using nullptr as the second argument signals that the application should go into windowed mode.
             * The size and position of the window should be the ones stored when going into full-screen,
             *      so the old window will be restored.
             */
            glfwSetWindowMonitor(window, nullptr, old_xpos, old_ypos, old_width, old_height, 0);

        }
    }
    else
    {

    }
}

/*
 * This is a callback registered in function main.
 * This function is called when the size of the window changes.
 */
void window_size_callback(GLFWwindow* window, int width, int height)
{
    /*
     * grab the view since we will be changing the perspective aspect.
     */
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    /*
     * Find out the width and height of the framebuffer for the window.
     * This is evidently the best way to find out how big the window is.
     *      The framebuffer is sized to match the window size.
     * Notice that the addresses of the width and height variables are passed to the
     *      glfwGetFramebufferSize function.
     */
    int widthfb, heightfb;
    glfwGetFramebufferSize(window, &widthfb, &heightfb);
    /*
     * The viewport is the amount of the window to use to display the rendered scene.
     * The parameters used here to set the viewport cause the viewport to cover the whole window.
     */
    glViewport(0, 0, widthfb, heightfb);
    /*
     * Change the perspective aspect.
     * Otherwise, the scene will be distorted if the aspect ratio is changed.
     */
    view->perspective_aspect =
        (static_cast<float>(widthfb) / static_cast<float>(heightfb));

}

// used in mouse motion
static double last_x = 0.0;
static double last_y = 0.0;

void move_callback(GLFWwindow* window, double xpos, double ypos)
{
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
     auto dx = xpos - view->last_x;
     auto dy = ypos - view->last_y;
     view->last_x = xpos;
     view->last_y = ypos;


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        // std::cout << "(" << xpos << "," << ypos << ")" << std::endl;
        // auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
        //     static_cast<float>(-dx * lr_pan_scale), camera_up_));
        auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
            static_cast<float>(-dx * lr_pan_scale), glm::vec3(0, 1, 0)));
        view->camera_left = rot_lr * view->camera_left;
        view->camera_forward = rot_lr * view->camera_forward;
        view->camera_up = rot_lr * view->camera_up;

        auto const rot_ud = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
            static_cast<float>(dy * ud_pan_scale), view->camera_left));
        view->camera_up = rot_ud * view->camera_up;
        view->camera_forward = rot_ud * view->camera_forward;
    }
}


int
main(int argc, char** argv)
{
    glfwInit();

    auto aspect_ratio = 1.0;
    auto* window = cs4722::setup_window("Window Events", 0.1, aspect_ratio);

    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();

    init();
    the_view->perspective_aspect = aspect_ratio;

    glfwSetWindowUserPointer(window, the_view);
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

	
    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float());
        glClear(GL_DEPTH_BUFFER_BIT);
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
