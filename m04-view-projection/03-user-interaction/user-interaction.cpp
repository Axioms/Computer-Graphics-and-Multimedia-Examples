
/*
 * This program introduces user interaction using callback functions.
 * 
 * We implement linear motion using the keyboard and camera orientation using the mouse.
 *      The left mouse button must be held down while moving the mouse in order for the motion
 *      to have an effect.
 *
 *  It is important to remember that what we are calling the event loop, in function main, repeatedly calls
 *      the display function to render the scene over and over.
 *  The function glfwPollEvents is also called in this loop.  That function triggers the callbacks, if
 *      events have happened.
 *      So, the camera state is changed in response to user interaction and then the scene is rendered again
 *      using the new values for the camera.
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

    the_view->camera_position = glm::vec3(0, 0, 25);

    program = cs4722::compile_shaders("vertex_shader.glsl","fragment_shader.glsl");
    glUseProgram(program);


    transform_loc = glGetUniformLocation(program, "transform");

    glEnable(GL_DEPTH_TEST);


    cs4722::shape* b = new cs4722::sphere(15, 50);
    auto* artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(5, -5, 5));
    artf->world_transform.scale = (glm::vec3(4, 4, 4));
    artf->rotation_rate = (M_PI / 3);
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);

    artf_list.push_back(artf);


    cs4722::shape* b1 = new cs4722::block();
    auto* artf1 = new cs4722::artifact_rotating();
    artf1->the_shape = (b1);
    artf1->world_transform.translate = (glm::vec3(-5, 5, -5));
    artf1->world_transform.scale = (glm::vec3(2, 6, 2));
    artf1->rotation_rate = (M_PI / 3);
    artf1->animation_transform.rotation_center =
            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));

    artf_list.push_back(artf1);

//    cs4722::shape* b = new cs4722::sphere(15, 50);
//    auto* artf = new cs4722::artifact_rotating();
//    artf->the_shape = (b);
//    artf->world_transform.translate = (glm::vec3(.5, -.5, 0));
//    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
//    artf->rotation_rate = (M_PI / 3);
//    artf->world_transform.scale = (glm::vec3(.5f, .5f, .5f));
//    artf->animation_transform.rotation_center =
//            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
//
//    artf_list.push_back(artf);
//
//
//    cs4722::shape* b1 = new cs4722::block();
//    auto* artf1 = new cs4722::artifact_rotating();
//    artf1->the_shape = (b1);
//    artf1->world_transform.translate = (glm::vec3(-.5, 0, 0));
//    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
//    artf1->rotation_rate = (M_PI / 3);
//    artf1->world_transform.scale = (glm::vec3(.2f, .6f, .2f));
//    artf1->animation_transform.rotation_center =
//            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
//
//    artf_list.push_back(artf1);


    cs4722::init_buffers(program, artf_list, "bPosition", "bColor");

}


//----------------------------------------------------------------------------
//
// display
//


void
display()
{

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    static auto last_time = 0.0;
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



/*
 *
 * This section contains constants that govern how fast the camera moves and rotates.
 * Camera speed would be, in a scene using first-person perspective, the speed of the character.
 * The speed of camera rotation would be mouse sensitivity.
 *
 */
/*
 * How far to move the camera in the specified direction on each click of the appropriate key.
 */
float movement_speed = 1.f;
float lr_scale = movement_speed;
float fb_scale = movement_speed;
float ud_scale = movement_speed;

/*
 * How far to rotate the camera, in radians, for each unit of motion of the mouse.
 */
float lr_pan_scale = .01f;
float ud_pan_scale = lr_pan_scale;


/*
 * The following function will be called by the system whenever there is a key pressed or released.
 *  For this to happen, the function must be 'registered' with GLFW.  This happens in the main function.
 *      There is a comment at that statement.
 *
 *  The programmer using callbacks implements a function.
 *      The implementation can make use of the parameter values passed to the function.
 *      The programmer will not write code that calls this function, at least in usual circumstances.
 *
 *  See the GLFW documentation for an explanation of all the parameters.
 *  Here are the ones that we use:
 *      The first parameter is the window the had the focus when the key was pressed.
 *      The second parameter indicates which key was pressed.  This is not necessarily a Unicode code for a character
 *      The fourth parameter indicates what kind of action caused the function to be called
 *
 *  We are not really interested in the window displaying the scene but in the view object that contains the
 *      camera attributes.
 *      GLFW has a feature that allows storing a pointer to an object in a window.
 *      In the main function the view object is stored in the window object in that way
 *      In this function, the view object is fetched so that the camera attributes can be modified
 *
 *  We are doing this, rather than accessing the view object directly, because this code is destined to be
 *      moved to a different file and placed in the library.
 *      The view object would not be directly accessible in that case.
 *
 *  The main actions indicated by the fourth parameter are pressing a key, releasing a key,
 *      and a key repeating.
 *
 *  The 'key' parameter is a code that should be compared to symbolic constants defined in GLFW
 *      See this page for the full list of symbols: https://www.glfw.org/docs/3.3/group__keys.html
 *      UP and DOWN are the arrow keys
 *
 */
void general_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
     * Get the view object stored in the window.
     * The declared type of the value is a generic pointer so this must be cast to the proper type.
     */
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    // std::cout << "key " << key << std::endl;  // debugging
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
    {
        /*
         * As my younger son would say, this is a bit hackish.
         * As the first test in a long series of tests, if this one passes, this function does
         * nothing at all.
         * This test passes if the event is neither a key press or a repeated key.
         * The effect is that only key presses and repeated keys can result in real results.
         */
    }
    else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
    {
        /*
         * Key A is for moving left, so add a multiple of the camera_left vector to the position.
         */
        view->camera_position += lr_scale * view->camera_left;
    }
    else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
    {
        /*
         * Key D is for moving right, so subtract a multiple of the camera_left vector to the position.
         */
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
    else if (key == GLFW_KEY_UP )
    {
        view->camera_position += ud_scale * view->camera_up;
    }
    else if (key == GLFW_KEY_DOWN)
    {
        view->camera_position -= ud_scale * view->camera_up;
    }
    else
    {

    }
}


/*
 * This callback function is called when the mouse moves.
 *
 *  The first parameter is the window over which the mouse moved
 *  The second and third parameters give the position of the mouse in the window.
 *      We are not interested in the position of the mouse but only in how much the mouse has moved
 *          since the last callback.
 *          Code to compute this 'delta' is in the function code.
 */
void move_callback(GLFWwindow* window, double xpos, double ypos)
{
//    /*
//     * We have to remember the last position of the mouse.
//     * A 'static' local variable is initialized when the program starts and then retains its value
//     *      between calls.
//     * The disadvantage is that this code is tied to one window at a time.
//     *      We'll fix
//     */
//    static double last_x = 0.0;
//    static double last_y = 0.0;

    /*
     * Get the view object from the window.
     */
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    // the_scene->the_view->cursor_move_callback(window, xpos, ypos);
    /*
     * The view object stores the last known position of the mouse.
     * So, we just subtract from the current position to get the distance the mouse has moved.
     */
    auto dx = xpos - view->last_x;
    auto dy = ypos - view->last_y;
    /*
     * Update the last known position for the next mouse event.
     */
    view->last_x = xpos;
    view->last_y = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        /*
         * Panning the camera is implemented by rotating the camera left and right.
         *  This does not change the position of the camera but changes its orientation.
         *
         *  An important question is which axis to rotate about?
         *  There are two good choices: the up vector of the camera and the positive y-axis.
         *  The code below ues the y-axis. However, you can change that by uncommenting the first
         *      definition of rot_lr.
         *
         *  rot_lr is a rotation matrix.
         *  The function glm::rotate returns a mat4 which must be converted to a mat3 in order to
         *      transform the camera vectors.
         *  The amount of rotation is a multiple of the amount the mouse has moved in the x direction.
         */
        // std::cout << "(" << xpos << "," << ypos << ")" << std::endl;
        // auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
        //     static_cast<float>(-dx * lr_pan_scale), camera_up_));
        auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
            static_cast<float>(-dx * lr_pan_scale), glm::vec3(0, 1, 0)));
        view->camera_left = rot_lr * view->camera_left;
        view->camera_forward = rot_lr * view->camera_forward;
        view->camera_up = rot_lr * view->camera_up;

        /*
         * Panning up and down is similar but using the camera left vector.
         * Note that the camera left vector will not change under this rotation, so it is not transformed.
         */
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

    auto aspect_ratio = 16.0/9.0;
    auto* window = cs4722::setup_window("User Interaction", 0.9, aspect_ratio);

    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();

    init();
    the_view->perspective_aspect = aspect_ratio;

    /*
     * Here is where we store the view object in the window.
     */
    glfwSetWindowUserPointer(window, the_view);

    /*
     * These two function calls 'register' the callbacks.
     * This means that the callback functions will be called by the system when the appropriate events
     *      happen.
     *
     *  Note that the functions names are given without ().
     *  This means that the functions are not being called, it is the functions themselves that are being
     *      passed to the registration function.
     */
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
	
    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float());
        glClear(GL_DEPTH_BUFFER_BIT);

        display();
        glfwSwapBuffers(window);
        /*
         * The glfwPollEvents function will make sure that all callback functions that have been registered
         * will be called.
         *
         * Note that event handling does not overlap rendering.
         * This saves potential problems relating to race conditions.
         */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
