



#include "sharing.h"


static GLuint transform_loc;
static GLuint sampler_loc;
static int fb_size_loc;

static GLuint program;

static cs4722::view *view;

static std::vector<cs4722::artifact*> parts_list;

static GLuint vao;

void view_in_view_setup(cs4722::view *the_view) {

    view = the_view;


    program = cs4722::compile_shaders("image_processing_vertex_shader.glsl",
                                   "image_processing_fragment_shader.glsl");
    std::cout << "view in view program " << program << std::endl;
    glUseProgram(program);
    transform_loc = glGetUniformLocation(program, "transform");
    sampler_loc = glGetUniformLocation(program, "sampler");


    fb_size_loc = glGetUniformLocation(program, "fb_size");
    glUniform1i(fb_size_loc, frame_buffer_width);



    auto* p = new cs4722::artifact();
    p->the_shape = new cs4722::rectangle(1,1);
    p->world_transform.scale = glm::vec3(2,2,2);
    p->world_transform.translate = glm::vec3(-1, -1, 0);

    parts_list.push_back(p);

    vao  = cs4722::init_buffers(program, parts_list, "bPosition","", "bTextureCoord");

}

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


    for (auto obj : parts_list) {

        obj->animate(time, delta_time);

        auto model_transform = obj->animation_transform.matrix() * obj->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(model_transform));
        glUniform1i(sampler_loc, fb_texture_unit);

        glDrawArrays(GL_TRIANGLES, obj->the_shape->buffer_start, obj->the_shape->buffer_size);
    }
}
