
/*
 * A view object contains attributes to describe a projection in addition to the
 *      camera attributes we saw in the previous example.
 *  The projection transform attributes are those needed for the infinitePerspective() function
 *      described in the notes:
 *      * Field of view in the y direction
 *      * Aspect ratio, width divided by height
 *      * Distance to the near face of the view frustum
 *
 *  The program starts with default values for the projection and the camera.
 *      The aspect ratio of the window is set to 1.0 when created (square)
 *      Objects are not clearly visible
 *
 *  Move the camera to z = 25 to see the objects
 *
 *  Move the camera to z = 100 to see the objects smaller
 *
 *  Change the window aspect to 16/9
 *      Also set camera z back to 25
 *      Notice the distortion
 *
 *  Keep the previous settings but set the project aspect to 16/9
 *
 *  Keep those previous settings but change the field of view to 10 degrees
 *      Can't see much
 *
 *  Keep going and set z = 100 on the camera position
 *      The objects are much larger, sort of like using a telephoto lens
 *
 */

#include "GLM/glm.hpp"
#include "GLM/ext.hpp"


#include <algorithm>


#include <glad/gl.h>
#include <GLFW/glfw3.h>


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

    /*
     * Default view
     */
    the_view->camera_position = glm::vec3(0, 0, 0);
    the_view->camera_forward = glm::vec3(0,0,-1);
    the_view->camera_up = glm::vec3(0,1,0);
    the_view->camera_left = glm::vec3(-1,0,0);

    /*
     * Default projection
     */
    the_view->perspective_aspect = 1.0;
    the_view->perspective_fovy = M_PI / 2;
    the_view->perspective_near = .1;


    program = cs4722::compile_shaders("vertex_shader.glsl","fragment_shader.glsl");

    glUseProgram(program);

    glEnable(GL_DEPTH_TEST);

    transform_loc = glGetUniformLocation(program, "transform");


    // create a shape
    //cs4722::shape *b =  new cs4722::cylinder(.75, 20);
    cs4722::shape* b = new cs4722::sphere(15, 50);
    // cs4722::shape* b = new cs4722::block();
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
    /*
     * Compute the projection transformation using the view attributes.
     */
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    /*
     * The view-projection transformation is the product.
     * Notice that the view transform is applied first, then the projection transform.
     */
    auto vp_transform = projection_transform * view_transform;

    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;

    for (auto artf : artf_list) {
        artf->animate(time, delta_time);

        /*
         * The final transform for this artifact is the view-projection transform applied after
         * the model transform.
         */
        auto transform = vp_transform * artf->animation_transform.matrix() *
                         artf->world_transform.matrix();
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}

int main(int argc, char** argv)
{
    glfwInit();


    /*
     * We can set the window aspect ratio when creating it.
     *  -  start with 1.0
     *  -  then try 2.0 and notice the distortion
     *  - change the perspective aspect in the view to match the aspect ratio
     *  - changing window size does not have any effect on the sizing of the scene
     */
    auto aspect_ratio = 1.0;
    auto *window = cs4722::setup_window("Projection with View", 0.9, aspect_ratio);

    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();

    init();
//    the_view->perspective_aspect = aspect_ratio;

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
