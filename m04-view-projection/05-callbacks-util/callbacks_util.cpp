

/*
 *  The callback functions we have used in the previous two examples are in the library in the file
 *      callbacks.cpp.
 *  We will not be changing the code, so it seemed helpful to move it out of the way.
 *  Look in function main to see how to register the functions provided.
 */

#include "GLM/gtc/type_ptr.hpp"

#include <glad/gl.h>

/*
 * The callbacks section of the library contains the callback functions we have been studying
 *      in the two previous examples.
 */
#include "cs4722/callbacks.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/compile_shaders.h"


static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> object_list;

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
    artf->world_transform.scale = (glm::vec3(.5, .5, .5));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);

    object_list.push_back(artf);


    cs4722::shape* b1 = new cs4722::block();
    auto* artf1 = new cs4722::artifact_rotating();
    artf1->the_shape = (b1);
    artf1->world_transform.translate = (glm::vec3(-.5, 0, 0));
    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf1->rotation_rate = (M_PI / 3);
    artf1->world_transform.scale = (glm::vec3(.2f, .6f, .2f));
    artf1->animation_transform.rotation_center =
            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);

    object_list.push_back(artf1);


    cs4722::init_buffers(program, object_list, "bPosition", "bColor");

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


    for (auto artf : object_list) {
        artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
       

    }
}


int
main(int argc, char** argv)
{
    glfwInit();
    auto aspect_ratio = 16.0/9.0;
    auto* window = cs4722::setup_window("Window Events Using Library Callbacks", 0.5,
                                        aspect_ratio);
    gladLoadGL(glfwGetProcAddress);
    glDebugMessageCallback(cs4722::message_callback, nullptr);

    init();
    the_view->perspective_aspect = aspect_ratio;

    glfwSetWindowUserPointer(window, the_view);
    /*
     * The callback functions are registered here.
     * The functions are defined in namespace cs4722 in order to guard against the same names
     *      being used in other library components.
     */
    glfwSetKeyCallback(window, cs4722::general_key_callback);
    glfwSetCursorPosCallback(window, cs4722::move_callback);
    glfwSetWindowSizeCallback(window, cs4722::window_size_callback);

	
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
