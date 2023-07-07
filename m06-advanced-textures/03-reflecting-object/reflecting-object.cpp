

/*
 * Reflecting objects use the background cube texture as their texture source.
 * However, the sample is chosen by reflecting the camera to fragment vector in the surface.
 * In this way the surface acts as a mirror.
 *
 * This example does not reflect the scene objects.  How to do this is discussed in the notes.
 * We haven't implemented that in OpenGL, but there is a WebGL example you can examine.  See the notes.
 */


#include <GLM/gtc/matrix_inverse.hpp>
 #include <GLM/gtc/type_ptr.hpp>



#include <glad/gl.h>


#include "cs4722/view.h"
#include "cs4722/artifact.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/texture_utilities.h"
#include "cs4722/compile_shaders.h"


#include "cs4722/window.h"
#include "cs4722/callbacks.h"


static cs4722::view *the_view;
static GLint program;
static std::vector<cs4722::artifact*> artifact_list;

static GLint m_transform_loc;
static GLint vp_transform_loc, n_transform_loc;
static GLint sampler2_loc, samplerC_loc;
static GLint surface_effect_loc;
static GLint camera_position_loc;

static GLuint vao;


void init()
{

    the_view = new cs4722::view();
    the_view->enable_logging = 0;
    the_view->set_flup(glm::vec3(0.202971,-0.227978,-0.952277),
                       glm::vec3(-0.978031,0,-0.20846),
                       glm::vec3(0.0475242,0.973667,-0.222969),
                       glm::vec3(-0.146291,0.195185,1.3214));



//    printf("ready to compile\n");
    program = cs4722::compile_shaders("vertex_shader03x.glsl",
                                      "fragment_shader03x.glsl");
    if(program <= 0) {
        printf("program is %d\n", program);
        exit(23);
    }
    glUseProgram(program);

    m_transform_loc = glGetUniformLocation(program, "m_transform");
    vp_transform_loc = glGetUniformLocation(program, "vp_transform");
    n_transform_loc = glGetUniformLocation(program, "n_transform");
    sampler2_loc = glGetUniformLocation(program, "sampler2");
    samplerC_loc = glGetUniformLocation(program, "samplerC");
    surface_effect_loc = glGetUniformLocation(program, "surface_effect");
    camera_position_loc = glGetUniformLocation(program, "camera_position");

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);


    cs4722::init_texture_from_file("../media/tulips-bed-2048x2048.png", 2);
    cs4722::init_texture_computed(1, 8);
    cs4722::init_cube_texture_from_path("../media/fjaderholmarna", 10, "png");
//    cs4722::init_cube_texture_from_path("../media/oriented-cube", 10, "png");

    cs4722::shape* b = new cs4722::sphere(15, 50);
    auto* artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(.5, -.5, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.4f, .6f, .4f));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    artf->texture_unit = 1;
    artf->surface_effect = 0;
    artifact_list.push_back(artf);


    b = new cs4722::block();
    artf = new cs4722::artifact_rotating();
    artf->the_shape = (b);
    artf->world_transform.translate = (glm::vec3(-.5, 0, 0));
    artf->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
    artf->rotation_rate = (M_PI / 3);
    artf->world_transform.scale = (glm::vec3(.2f, .6f, .2f));
    artf->animation_transform.rotation_center =
            artf->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    artf->texture_unit = 2;
    artf->surface_effect = 0;

    artifact_list.push_back(artf);

    auto skybox_scale = 40000.0f;

    auto* a_shape = new cs4722::block();
    // auto* a_shape = new cs4722::sphere();
    // auto* a_shape = new cs4722::cylinder();
    // auto* a_shape = new cs4722::torus();

    auto artfb = new cs4722::artifact();
    artfb->the_shape = a_shape;
    artfb->world_transform.scale = glm::vec3(skybox_scale);
    artfb->texture_unit = 10;
    artfb->surface_effect = 1;

    artifact_list.push_back(artfb);


    /*
     * Reflecting objects
     * These are noted with surface_effect 2 for the fragment shader
     * They use the same cube texture unit as the skybox, just select the sample with
     *      different parameters.
     */

    b = new cs4722::sphere(15, 50);
    auto* obj = new cs4722::artifact_rotating();
    obj->the_shape = (b);
    obj->world_transform.translate = (glm::vec3(.5, 5, 0));
    obj->animation_transform.rotation_axis = (glm::vec3(1, 1, 0));
//    obj->rotation_rate = 2*M_PI / 10;
    obj->world_transform.scale = (glm::vec3(1.0f, .6f, 1.0f));
    obj->animation_transform.rotation_center =
            obj->world_transform.matrix() * glm::vec4(0, 0, 0, 1);
    obj->surface_effect = 2;
    obj->texture_unit = 10;
    artifact_list.push_back(obj);


    cs4722::shape* b1 = new cs4722::block();
//    b1 = new cs4722::torus();
//    b1 = new cs4722::sphere();
   auto* obj1 = new cs4722::artifact_rotating();
    obj1->the_shape = (b1);
    obj1->world_transform.translate = (glm::vec3(-.5, 0, 3));
    obj1->animation_transform.rotation_axis = (glm::vec3(1, 0, 1));
//    obj1->rotation_rate = 2 * M_PI / 15;
    obj1->world_transform.scale = (glm::vec3(1.0f, .6f, 1.0f));
    obj1->animation_transform.rotation_center =
            obj1->world_transform.matrix() * glm::vec4(0, .3f, 0, 1);
    obj1->surface_effect = 2;
    obj1->texture_unit = 10;
    artifact_list.push_back(obj1);







    /*
     * We need the normal vectors to compute reflection.
     * Normal vectors will be discussed in detail in the next module, lighting and shading.
     */

    vao = cs4722::init_buffers(program, artifact_list, "bPosition", "",
                               "bTextureCoord", "bNormal", "");

}


//----------------------------------------------------------------------------
//
// display
//


void display()
{

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;
    glUniformMatrix4fv(vp_transform_loc, 1, GL_FALSE, glm::value_ptr(vp_transform));
    /*
     * The camera position is needed to compute the reflection
     */
    glUniform3fv(camera_position_loc, 1, glm::value_ptr(the_view->camera_position));

    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto artf : artifact_list) {
        artf->animate(time, delta_time);
        glm::mat4 model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        /*
         * Normal vectors need to be transformed to world coordinates to compute the reflection.
         * The matrix that does that transformation is the inverse of the transpose of the model transform.
         */
        glm::mat4 n_transform = glm::inverseTranspose(model_transform);
        // send model and normal transformations to the vertex shader
        glUniformMatrix4fv(m_transform_loc, 1, GL_FALSE, glm::value_ptr(model_transform));
        glUniformMatrix4fv(n_transform_loc, 1, GL_FALSE, glm::value_ptr(n_transform));

        // it is probably safer to check which way the texture unit value is to be used
        //      regular texture or cube texture.
        if(artf->surface_effect == 0) {
            glUniform1i(sampler2_loc, artf->texture_unit);
        } else {
            glUniform1i(samplerC_loc, artf->texture_unit);
        }

        glUniform1i(surface_effect_loc, artf->surface_effect);

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


int
main(int argc, char** argv)
{
    glfwInit();

    auto *window = cs4722::setup_window("Skybox", .9);



    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();

    init();

    glfwSetWindowUserPointer(window, the_view);
//    cs4722::setup_user_callbacks(window);

//    the_view->enable_logging = false;
//    the_view->enable_logging = true;

    printf("view logging %d\n", the_view->enable_logging);

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
//        printf("view logging %d\n", the_view->enable_logging);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
