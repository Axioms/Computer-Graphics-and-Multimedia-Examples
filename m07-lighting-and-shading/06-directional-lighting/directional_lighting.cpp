
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
 *   This example contains a different approach to the memory leak discussed in the last example.
 *   The approach is more efficient in terms of time and space, BUT it is dangerous.
 *   The discussion starts about on line 154.
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
static GLint light_direction_loc; // direction toward the light
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
    a_light.light_direction_position = glm::vec4(0,-1,0,0);
    a_light.is_directional = true;

	program = cs4722::compile_shaders("vertex_shader06.glsl",
                                   "fragment_shader06.glsl");
	glUseProgram(program);



    // fragment shader uniformas
    ambient_light_loc = glGetUniformLocation(program, "ambient_light");
    specular_light_loc = glGetUniformLocation(program, "specular_light");
    diffuse_light_loc = glGetUniformLocation(program, "diffuse_light");
    ambient_color_loc = glGetUniformLocation(program, "ambient_color");
    specular_color_loc = glGetUniformLocation(program, "specular_color");
    diffuse_color_loc = glGetUniformLocation(program, "diffuse_color");
    light_direction_loc = glGetUniformLocation(program, "light_direction");
    camera_position_loc = glGetUniformLocation(program, "camera_position");
    specular_shininess_loc = glGetUniformLocation(program, "specular_shininess");
    specular_strength_loc = glGetUniformLocation(program, "specular_strength");
    // vertex shader uniforms
    m_transform_loc = glGetUniformLocation(program, "m_transform");
    vp_transform_loc = glGetUniformLocation(program, "vp_transform");
    normal_transform_loc = glGetUniformLocation(program, "normal_transform");


    glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);


	// create a list of shapes that will be shared by the objects we have

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
				artf->surface_material.specular_strength = 1.0;
				artf->surface_material.shininess = 10.0;
				artifact_list.push_back(artf);
			}
		}
	}
	

    vao = cs4722::init_buffers(program, artifact_list, "bPosition","","","bNormal");
}

/**
 *  The following arrays are used in a variation of the as_float function that copies the floating point color
 *      data into an array provided by the caller.
 *  These arrays are allocated once and remain through the execution of the program.
 *      They are filled with data over and over but no extra memory is needed at any point.
 *
 *  BUT WARNING:  these arrays must be at least length 4 because the as_float method will copy four floating
 *      values into memory.
 *      If these arrays are too short, then the extra data may change some unrelated variable and cause
 *      program malfunction. Or, maybe not.  Murphy's law corollary: malfunction will occur at the worst
 *      possible time.
 *
 *  Look in the display loop below for how these arrays are used.
 */
static GLfloat light_ambient[4];
static GLfloat light_diffuse[4];
static GLfloat light_specular[4];
static GLfloat material_ambient[4];
static GLfloat material_diffuse[4];
static GLfloat material_specular[4];

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

    glUniform4fv(light_direction_loc, 1, glm::value_ptr(a_light.light_direction_position));
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
         *          is here replaced by two separate parts: the model-view transform and
         *          the projection.
         *          These will be combined in the vertex shader.
         *          But, the model-view transform is needed there so it's easier to set the transforms
         *          up this way
         */
        glUniformMatrix4fv(m_transform_loc, 1, GL_FALSE, glm::value_ptr(model_transform));
        glUniformMatrix4fv(vp_transform_loc, 1, GL_FALSE, glm::value_ptr(vp_transform));
        glUniformMatrix4fv(normal_transform_loc, 1, GL_FALSE,
                           glm::value_ptr(glm::inverseTranspose(model_transform)));



        /**
         * Put the floating point color data into the variables declared above.
         */
        a_light.ambient_light.as_float(light_ambient);
        a_light.diffuse_light.as_float(light_diffuse);
        a_light.specular_light.as_float(light_specular);
        artf->surface_material.ambient_color.as_float(material_ambient);
        artf->surface_material.diffuse_color.as_float(material_diffuse);
        artf->surface_material.specular_color.as_float(material_specular);
        /**
         * Send the values to the fragment shader.
         */
        glUniform4fv(ambient_light_loc, 1, light_ambient);
        glUniform4fv(diffuse_light_loc, 1, light_diffuse);
        glUniform4fv(specular_light_loc, 1, light_specular);
        glUniform4fv(ambient_color_loc, 1, material_ambient);
        glUniform4fv(diffuse_color_loc, 1,  material_diffuse);
        glUniform4fv(specular_color_loc, 1, material_specular);
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
	auto *window = cs4722::setup_window("No Lighting", 0.9);
    gladLoadGL(glfwGetProcAddress);
	cs4722::setup_debug_callbacks();

	init();

	glfwSetWindowUserPointer(window, the_view);
	cs4722::setup_user_callbacks(window);

    float *clear_color = cs4722::x11::gray25.as_float();

	while (!glfwWindowShouldClose(window))
	{
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClear(GL_DEPTH_BUFFER_BIT);

        display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
