
/*
 * To support the camera and projection we introduce the 'view' class.
 * This class is mainly a collection of attributes.
 * In this example we just concern ourselves with the camera and its attributes:
 *      camera_up
 *      camera_forward
 *      camera_left
 *      camera_position
 *   The first three of these attributes are vectors specifying the orientation
 *      of the camera
 *   The last attribute is a position.  These are all stored as vec3's
 *
 *   We have to be careful in changing the camera orientation since the three vectors
 *      have specified relationships:  all three are unit vectors (length 1);
 *      and, they are each perpendicular to the other two.
 */


#include "GLM/gtc/type_ptr.hpp"

#include <glad/gl.h>

#include <cs4722/artifact.h>
#include "cs4722/view.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/compile_shaders.h"
#include "cs4722/callbacks.h"

static cs4722::view *the_view;
static GLuint program;
static std::vector<cs4722::artifact*> artifact_list;

GLuint transform_loc;



void init()
{

    the_view = new cs4722::view();

//    the_view->camera_position = glm::vec3(0, 0, -1.2);
//    the_view->camera_up = glm::vec3(0, -1, 0);
    the_view->camera_up = glm::vec3(1, -1, 0);
//    the_view->camera_position = glm::vec3(0, 0, 1);
//    the_view->camera_forward = glm::vec3(0, 18, -1);


    program = cs4722::compile_shaders("vertex_shader.glsl","fragment_shader.glsl");
    glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");

    glEnable(GL_DEPTH_TEST);

	
    // create a shape
    //cs4722::shape *b =  new cs4722::cylinder(.75, 20);
    cs4722::shape* b = new cs4722::sphere(15,50);
    // cs4722::shape* b = new cs4722::block();
    auto* artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(.5, -.5, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.5f, .5f, .5));
    
    artifact_list.push_back(artf);

	
    cs4722::shape* b1 = new cs4722::block();
    auto* artf1 = new cs4722::artifact_rotating();
    artf1->the_shape = (b1);
    artf1->world_transform.translate = (glm::vec3(-.5, 0, 0));
    artf1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf1->rotation_rate = (M_PI / 3);
    artf1->animation_transform.rotation_center =
            artf1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    artf1->world_transform.scale = (glm::vec3(.2f, .6f, .2f));

    artifact_list.push_back(artf1);


    cs4722::init_buffers(program, artifact_list, "bPosition", "bColor");
}

//----------------------------------------------------------------------------
//
// display
//


void
display()
{
    /*
     * The view transform is a combination or rotations and translations that bring the camera
     *      to the origin and orient it in the standard way: positive x goes to the right;
     *      positive z goes away from us; positive y goes up.
     *
     *  glm provides the 'lookAt' function to compute a view transformation.
     *  The first parameter is the position of the camera
     *  The second parameter is a point that is directly in front of the camera
     *      An easy way to compute such a point is to simply add the forward vector to the position
     *  The third parameter is a vector pointing in the direction of the camera up
     *
     *  The view transform is computed outside the artifact loop because the view applies
     *      to all of the artifacts in the same way.
     */
    glm::mat4x4 view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;
    for (auto artf : artifact_list) {
        artf->animate(time, delta_time);
        /*
         * the animation transform times the world transform gives what is commonly called
         *      the model transform.
         *      This is the transform that moves the artifact into its position in the scene
         *      and gives it the proper orientation and size.
         */
        auto model_matrix = artf->animation_transform.matrix() * artf->world_transform.matrix();
        /*
         * The view transform is applied after the model transform, so the
         *      product is view times model.
         */
        auto transform = view_transform * model_matrix;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawArrays(GL_TRIANGLES,  artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


int
main(int argc, char** argv)
{
    glfwInit();
    auto *window = cs4722::setup_window("Animation With Camera", 0.9);
    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();
    init();
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
