
/**
 * This program creates a cube map and uses it to paint a texture onto a cube (or other shape).
 *
 * There are two major differences from using a simple texture:
 *      *  Six images are loaded to create the texture
 *      *  The texture coordinates for a fragment form a vector, three components, this
 *          will be computed from the object coordinates of each fragment.
 *
 *  The object coordinates of a fragment are the coordinates the fragment would have in the coordinates
 *  describing the untransformed shape.
 *
 *  The function init_cube_texture_from_dds gets the six images for the texture from a single file.
 *  The function init_cube_texture_from_file() gets the six images for the texture from
 *      six different files.
 *
 */

#include <GLM/gtc/type_ptr.hpp>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "red_book/vermilion.h"
#include "STB/stb_image.h"

#include "cs4722/callbacks.h"
#include "cs4722/view.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/compile_shaders.h"


static cs4722::view* the_view;
static GLuint program;
static std::vector<cs4722::artifact*> artifact_list;


GLuint transform_loc, sampler_loc;

void init()
{
    the_view = new cs4722::view();
    the_view->camera_position = (glm::vec3(0, 0, 2));


    program = cs4722::compile_shaders("vertex_shader01.glsl","fragment_shader01.glsl");
    glUseProgram(program);

    transform_loc = glGetUniformLocation(program, "transform");
    sampler_loc = glGetUniformLocation(program, "sampler");


    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);


     auto *a_shape = new cs4722::block();
//    auto* a_shape = new cs4722::cylinder();
    auto *artf = new cs4722::artifact_rotating();
    artf->the_shape = (a_shape);
    artf->animation_transform.rotation_axis = glm::vec3(1,1,0);
    artf->rotation_rate = (M_PI / 8);
    artf->world_transform.scale = (glm::vec3(.4f, .4f, .4f));
    artf->world_transform.scale = (glm::vec3(1,1,1));
    artifact_list.push_back(artf);

    cs4722::init_buffers(program, artifact_list, "bPosition", "",
                         "", "","");


  
}


/*
 * This function is supplied with the textbook.
 * A DDS format file contains one or more images as well as
 * information that can be used to create textures of various kinds
 * The file used here will create a cube texture.
 */
void init_cube_texture_from_dds()
{
    // The texture created by vglLoadTexture will be bound to this texture unit.
    glActiveTexture(GL_TEXTURE1);
    // this variable will hold the image data
    vglImageData image;
    // this function creates a texture .
    //  The last parameter is the location to store the image data and information about the image.
    auto tex = vglLoadTexture("../media/TantolundenCube.dds", 0, &image);
    GLenum e = glGetError();
    // free the storage used to hold the image data.  It's not needed once the texture
    //   data has been transferred to the texture.
    vglUnloadImage(&image);
}

/**
 * This function loads six separate image files to create a cube texture.
 */
void init_cube_texture_from_file()
{

    // create a texture
    GLuint cube_texture;
    glGenTextures(1, &cube_texture);
    //  Bind to texture unit 1
    //  This is the old way of binding, but is the way it must be done for cube textures
    //      at least for OpenGL implemented on my systems
    glActiveTexture(GL_TEXTURE1);
    // Bind the texture to a binding point.
    // This is similar to how vertex buffer data was managed.
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_texture);

    // paths to each of the images that will be used in the cube texture
    // The files are organized and named so that they can be used by a texture loading
    //      utility introduced in the next example.
    auto  paths = std::vector<const char*>( {
         "../media/oriented-cube-reversed/posx.png",
         "../media/oriented-cube-reversed/negx.png",
         "../media/oriented-cube-reversed/posy.png",
         "../media/oriented-cube-reversed/negy.png",
         "../media/oriented-cube-reversed/posz.png",
         "../media/oriented-cube-reversed/negz.png",
    });

    // alternative images
    auto  paths2 = std::vector<const char*>({
     "../media/fjaderholmarna/posx.png",
     "../media/fjaderholmarna/negx.png",
     "../media/fjaderholmarna/posy.png",
     "../media/fjaderholmarna/negy.png",
     "../media/fjaderholmarna/posz.png",
     "../media/fjaderholmarna/negz.png",
        });

    const auto internal_format = GL_RGBA8;

    /*
     * Each image will be loaded into the corresponding face of the cube texture.
     * Each face is identified by a symbolic constant.
     * Conveniently the constants are sequential integers, so we can step through the constants
     *      by starting with the first constant and adding repeatedly adding 1 to get the other
     *      faces.
     * The fist constant is for the positive X face, X+.
     * The other faces, in order, are: X-, Y+, Y-, Z+, and Z-.
     * Notice that the image paths are specified in the same order.
     */
	auto face = GL_TEXTURE_CUBE_MAP_POSITIVE_X ;

	for(auto const path : paths2)
	{
        int texture_width, texture_height, n;
        auto *texture_data = stbi_load(path,
               &texture_width, &texture_height, &n, 0);
        const auto external_format = (n == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(face,
            0, // level
            internal_format,
            texture_width, texture_height,   // taken from the image
            0, // border
            external_format,
            GL_UNSIGNED_BYTE,  // component type of the external image
            texture_data
        );
        // the image data is no longer needed in the driver, it is now in the texture
        delete texture_data;
        // increment the face for the next iteration.
        face++;
	}

	// set parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // the cube texture is 3D, so there are three coordinates
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}



void display()
{
    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;
    glUniform1i(sampler_loc, 1);

    for (auto* artf: artifact_list) {
        artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
    	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
                     artf->the_shape->buffer_size);
        
    }
}





int
main(int argc, char** argv)
{
    glfwInit();


    auto* window = cs4722::setup_window("Cube Map", .9);

    gladLoadGL(glfwGetProcAddress);
    cs4722::setup_debug_callbacks();

    init();

    /*
     * Uncomment one of the following two lines to choose which function to use to load  the cube texture.
     */
    init_cube_texture_from_file();
//     init_cube_texture_from_dds();

    glfwSetWindowUserPointer(window, the_view);
    cs4722::setup_user_callbacks(window);

	
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
