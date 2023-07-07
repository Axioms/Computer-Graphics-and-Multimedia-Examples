



#include "sharing.h"



static GLuint texture_scale_translate_loc;
static GLint transform_loc;
static GLint sampler_loc;

static GLint program;

static cs4722::view *view;

static std::vector<cs4722::artifact*> artifact_list;

static GLuint vao;

void view_in_view_setup(cs4722::view *the_view) {

    view = the_view;


    program = cs4722::compile_shaders("view_in_view_vertex_shader04.glsl",
                                   "view_in_view_fragment_shader04.glsl");
    std::cout << "view in view program " << program << std::endl;
    glUseProgram(program);
    texture_scale_translate_loc = glGetUniformLocation(program, "texture_scale_translate");
    transform_loc = glGetUniformLocation(program, "transform");
    sampler_loc = glGetUniformLocation(program, "sampler");


    /*
     * Setup transformation between model coordinates and texture coordinates.
     * The texture that is used for rendering the scene is square, so only a part will
     * be used to cover the view-in-view.
     *
     * The transform is stored in a vec4: two coordinates for the scale, two for the translation
     */
    if(rectangle_height < rectangle_width) {
        const auto scale_x = 1.0;
        const auto scale_y = rectangle_height / rectangle_width;
        const auto translate_x = 0.0;
        const auto translate_y = (1.0 - scale_y) / 2.0;
        glUniform4f(texture_scale_translate_loc,
                    scale_x, scale_y, translate_x, translate_y);
    } else {
        const auto scale_y = 1.0f;
        const auto scale_x = rectangle_width / rectangle_height;
        const auto translate_y = 0.0f;
        const auto translate_x = (1.0f - scale_y) / 2.0f;
        glUniform4f(texture_scale_translate_loc,
                    scale_x, scale_y, translate_x, translate_y);

    }


    auto* p = new cs4722::artifact();
    p->the_shape = new cs4722::rectangle(1,1);
//    p->the_shape = new cs4722::sphere();
    // obj->set_translate(glm::vec3(.5, -.5, 0));
    p->world_transform.translate = (glm::vec3(-rectangle_width/2, rectangle_height/2, 0));
    p->world_transform.scale = (glm::vec3(rectangle_width, rectangle_height, 1));
//    p->the_color = (cs4722::x11::olive_drab);

    artifact_list.push_back(p);

    vao  = cs4722::init_buffers(program, artifact_list, "bPosition","", "bTextureCoord");

}


/**
 * Standard display
 */
void view_in_view_display() {

    glBindVertexArray(vao);
    glUseProgram(program);

    static auto last_time = 0.0;

    auto view_transform = glm::lookAt(glm::vec3(0,0,2),
                                      glm::vec3(0,0,0),
                                      glm::vec3(0,1,0));
    auto projection_transform = glm::infinitePerspective(view->perspective_fovy,
                                                         view->perspective_aspect, view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto obj : artifact_list) {

        obj->animate(time, delta_time);

        auto model_transform = obj->animation_transform.matrix() * obj->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform1i(sampler_loc, fb_texture_unit);

        glDrawArrays(GL_TRIANGLES, obj->the_shape->buffer_start, obj->the_shape->buffer_size);
    }
}
