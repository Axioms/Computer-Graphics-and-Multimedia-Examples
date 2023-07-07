
/**
 * Starting with the no-lighting project, there are numerous changes needed to support shading.
 *      * A light object is defined
 *      * The light's ambient, diffuse, and specular components must be sent to the fragment shader
 *      * The specular, ambient, and diffuse colors of the surface material of each part must be
 *          sent to the fragment shader
 *      * The surface's shininess and specular strength must be sent to the fragment shader
 *      * Because shading is computed in the view frame (in this implementation)  the model-view
 *          transform must be sent to the vertex shader
 *
 *   Changes are needed in the shaders.  See those for comments
 *
 *
 *   The original version of this had a memory leak.
 *   Indeed, most of the examples up to this point have a memory leak.  If a program uses .as_float() inside
 *      the display loop, then that will leak memory until the program crashes.
 *      This just didn't become noticeable until this example when .as_float() is used six times in the display loop.
 *   The code in this example uses one approach to solve the issue.
 *   See the code and comment around line 203 and following for a discussion.
 */


#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_inverse.hpp>

#include <iostream>


#include <glad/gl.h>

#include <GLFW/glfw3.h>



#include "cs4722/artifact.h"
#include "cs4722/buffer_utilities.h"
#include "cs4722/window.h"
#include "cs4722/callbacks.h"
#include "cs4722/light.h"
#include "cs4722/compile_shaders.h"

static cs4722::view *the_view;
static GLuint program;
static GLuint vao;

// many uniform variable locations to deal with
static GLint ambient_light_loc;  // three components of light
static GLint specular_light_loc;
static GLint diffuse_light_loc;
static GLint ambient_color_loc;  // three components of surface material
static GLint specular_color_loc;
static GLint diffuse_color_loc;
static GLint light_position_loc; // dposition of the light
static GLint camera_position_loc;
static GLint specular_shininess_loc;     // exponent for sharpening highlights
static GLint specular_strength_loc;      // extra factor to adjust shininess
static GLint m_transform_loc;
static GLint vp_transform_loc;
static GLint normal_transform_loc;


static std::vector<cs4722::artifact*> artifact_list;

static cs4722::light a_light;

void init()
{
    the_view = new cs4722::view();
    the_view->enable_logging = false;
    a_light.ambient_light = cs4722::x11::gray25;
    a_light.is_directional = false;
    a_light.light_direction_position = glm::vec4(0,0,0,1);

	program = cs4722::compile_shaders("vertex_shader05.glsl",
                                   "fragment_shader05.glsl");
	glUseProgram(program);



    // fragment shader uniformas
    ambient_light_loc = glGetUniformLocation(program, "ambient_light");
    specular_light_loc = glGetUniformLocation(program, "specular_light");
    diffuse_light_loc = glGetUniformLocation(program, "diffuse_light");
    ambient_color_loc = glGetUniformLocation(program, "ambient_color");
    specular_color_loc = glGetUniformLocation(program, "specular_color");
    diffuse_color_loc = glGetUniformLocation(program, "diffuse_color");
    light_position_loc = glGetUniformLocation(program, "light_position");
    camera_position_loc = glGetUniformLocation(program, "camera_position");
    specular_shininess_loc = glGetUniformLocation(program, "specular_shininess");
    specular_strength_loc = glGetUniformLocation(program, "specular_strength");
    // vertex shader uniforms
    m_transform_loc = glGetUniformLocation(program, "m_transform");
    vp_transform_loc = glGetUniformLocation(program, "vp_transform");
    normal_transform_loc = glGetUniformLocation(program, "normal_transform");


    glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);


	// create a list of shapes that will be shared by the artifacts we have

	const auto color_set = std::vector <cs4722::color>({
			cs4722::x11::white, cs4722::x11::grey50, cs4722::x11::grey75,
		});
	
	auto* shape_list = new std::vector<cs4722::shape*>();
	shape_list->push_back(new cs4722::sphere());
	shape_list->push_back(new cs4722::block());
	shape_list->push_back(new cs4722::torus());
	shape_list->push_back(new cs4722::cylinder());
	auto numshp = shape_list->size();



	auto number = 4;
	auto d = 20.0f / (2 * number + 1);
	auto radius = d / 4;
	auto base = -number * d / 2 + radius;

	for (auto x = 0; x < number; ++x)
	{
		for (auto y = 0; y < number; ++y)
		{
			for (auto z = 0; z < number; ++z)
			{
				auto* artf = new cs4722::artifact_rotating();
				artf->the_shape = (shape_list->at((x + y + z) % numshp));
				artf->world_transform.translate = (glm::vec3(base + x * d, base + y * d, base + z * d));
				artf->world_transform.scale = (glm::vec3(radius, radius, radius));
                artf->animation_transform.rotation_axis = (glm::vec3(x + 1, y + 1, z + 1));
                artf->animation_transform.rotation_center =
                        artf->world_transform.matrix() * glm::vec4(0,3,0,1);
                artf->rotation_rate = ((x + y + z) % 12 * M_PI / 24);
                /**
                 * The surface material is set up.
                 * The same color is used for the ambient color and the diffuse color.
                 * The specular color is set to white
                 */
				artf->surface_material.ambient_color = (cs4722::color(
					x * 255 / (number-1), y * 255 / (number - 1), 
					z * 255 / (number - 1), 255));
				artf->surface_material.specular_color = cs4722::x11::white;
				artf->surface_material.diffuse_color = artf->surface_material.ambient_color;
				artf->surface_material.specular_strength = .75;
				artf->surface_material.shininess = 30.0;
				artifact_list.push_back(artf);
			}
		}
	}
	

    vao = cs4722::init_buffers(program, artifact_list, "bPosition","","","bNormal");
}



void display()
{

    glBindVertexArray(vao);
    glUseProgram(program);

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect,
                                                         the_view->perspective_near);

    auto vp_transform = projection_transform * view_transform;

    glUniform4fv(light_position_loc, 1, glm::value_ptr(a_light.light_direction_position));
    glUniform4fv(camera_position_loc, 1, glm::value_ptr(the_view->camera_position));


    static auto last_time = 0.0;
    auto time = glfwGetTime();
	auto delta_time = time - last_time;

	for (auto artf: artifact_list) {

		artf->animate(time, delta_time);
        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();

        //
        /**
         *      The model-view-projection transform we have been sending to the vertex shader
         *          is here replaced by two separate parts: the model transform and
         *          the view-projection.
         *          These will be combined in the vertex shader.
         *          But, the model transform is needed there to convert to world coordinates
         *          so it's easier to set the transforms up this way
         */
        glUniformMatrix4fv(m_transform_loc, 1, GL_FALSE, glm::value_ptr(model_transform));
        glUniformMatrix4fv(vp_transform_loc, 1, GL_FALSE, glm::value_ptr(vp_transform));
        glUniformMatrix4fv(normal_transform_loc, 1, GL_FALSE,
                           glm::value_ptr(glm::inverseTranspose(model_transform)));


        /**
         * The as_float method in class color allocates an array and returns a pointer to that array.
         * Heretofore the code has simply discarded that pointer after using the data and, so, the
         *      storage is never reclaimed.
         * One approach, not illustrated in these examples, is to assign the array pointer to a variable,
         *      use the variable in a call to glUniform4fv and the call delete[] to deallocated the storage.
         * In this example we illustrate an approach that lets us keep pretty much the same code at the expense
         *      of allocating more storage on a temporary basis.
         *
         *  The unique_ptr class creates objects that manage pointers.
         *  Create a new object or array and create a unique_ptr to hold that pointer.
         *  When the unique_ptr object does away, it will first deallocated the contained data.
         *
         *  The function as_float_up() returns a unique_ptr.
         *  This is not a pointer to something but the object itself.
         *  That means that the object returned is allocated on the stack space of the display function.
         *  As soon as the display function is finished, the unique_ptr will be deallocated which will
         *      trigger deletion of the array it contains.
         *
         *   This works since we can return an object, which means the value obtained is a copy of the original
         *      object and is allocated on the stack.
         *  Trying to return an array from a function just returns a pointer to the first element of the array.
         *      That only works properly if the array is allocated on the heap with 'new'.
         *
         *
         *  Note that the get() method is applied to the return value in order to get the actual data
         *      encapsulated in the unique_ptr.
         *
         *
         */
        glUniform4fv(ambient_light_loc, 1, a_light.ambient_light.as_float_up().get());
        glUniform4fv(diffuse_light_loc, 1, a_light.diffuse_light.as_float_up().get());
        glUniform4fv(specular_light_loc, 1, a_light.specular_light.as_float_up().get());
        glUniform4fv(ambient_color_loc, 1, artf->surface_material.ambient_color.as_float_up().get());
        glUniform4fv(diffuse_color_loc, 1,  artf->surface_material.diffuse_color.as_float_up().get());
        glUniform4fv(specular_color_loc, 1, artf->surface_material.specular_color.as_float_up().get());
        glUniform1f(specular_shininess_loc, artf->surface_material.shininess);
        glUniform1f(specular_strength_loc, artf->surface_material.specular_strength);


        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start,
			artf->the_shape->buffer_size);
		
	}
}




int
main(int argc, char** argv)
{
	glfwInit();
	auto *window = cs4722::setup_window("Point Lighting", 0.9);
    gladLoadGL(glfwGetProcAddress);
	cs4722::setup_debug_callbacks();

	init();

	glfwSetWindowUserPointer(window, the_view);
	cs4722::setup_user_callbacks(window);

	while (!glfwWindowShouldClose(window))
	{
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray25.as_float_up().get());
        glClear(GL_DEPTH_BUFFER_BIT);

        display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
