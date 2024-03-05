#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../scene/scene.h"
#include "./viewer.h"
#include "../basic/transform.h"
#include "../basic/object.h"
#include "../render/components/mesh_displayer.h"
#include "../render/components/mesh_filter.h"
#include "../render/texture2d.h"
#include "../render/material.h"
#include "../physics/rigid_body_dynamics.h"
#include "../physics/box_collider.h"
#include "../physics/mesh_collider.h"

int main()
{
    Viewer viewer("test", 2000, 1200);
    auto shader = std::make_shared<Shader>("../../src/render/shader.vs", "../../src/render/shader.fs");

    // add grid box
    // ==========================================================================
    auto gridbox = std::make_shared<Object>("gridbox");
    auto gridbox_transform = gridbox->add_component<Transform>();
    auto gridbox_mesh_filter = gridbox->add_component<MeshFilter>();
    auto gridbox_displayer = gridbox->add_component<MeshDisplayer>();
    auto gridbox_plane_collider = gridbox->add_component<PlaneCollider>();

    gridbox_plane_collider->position = glm::vec3(0.f, 0.f, 0.f);
    gridbox_plane_collider->normal = glm::vec3(0.f, 1.f, 0.f);
    // load mesh
    gridbox_mesh_filter->make_plane(); 
    // set transform
    gridbox_transform->set_scale(glm::vec3(50.f, 50.f, 50.f));
    gridbox_transform->set_rotation_by_angle(glm::vec3(glm::radians(90.f), 0.f, 0.f));
    // set shader
    gridbox_displayer->set_shader(shader);
    // set material
    auto gridbox_tex = Texture2D::load_texture("../../resources/textures/marble.jpg");
    auto gridbox_mtl = std::make_shared<Material>();
    gridbox_mtl->textures.push_back(std::make_pair("material.tex_diffuse", gridbox_tex));
    gridbox_displayer->set_material(gridbox_mtl);

    // add wall
    // ==========================================================================
    auto wall = std::make_shared<Object>("wall");
    auto wall_transform = wall->add_component<Transform>();
    auto wall_mesh_filter = wall->add_component<MeshFilter>();
    auto wall_mesh_displayer = wall->add_component<MeshDisplayer>();
    auto wall_plane_collider = wall->add_component<PlaneCollider>();
    wall_plane_collider->position = glm::vec3(0.f, 0.f, -3.f);
    wall_plane_collider->normal = glm::vec3(0.f, 0.f, 1.f);
    wall_transform->set_scale(glm::vec3(10.f, 7.f, 1.f));
    wall_transform->set_position(glm::vec3(0.f, 0.f, -3.f));
    // set shader
    wall_mesh_displayer->set_shader(shader);
    // set material
    auto wall_tex = Texture2D::load_texture("../../resources/textures/bricks2.jpg");
    auto wall_mtl = std::make_shared<Material>();
    wall_mtl->textures.push_back(std::make_pair("material.tex_diffuse", wall_tex));
    wall_mesh_displayer->set_material(wall_mtl);
    // load mesh
    wall_mesh_filter->make_plane();

    // add bunny
    // ===========================================================================
    auto bunny = std::make_shared<Object>("bunny");
    auto transform = bunny->add_component<Transform>();
    auto mesh_filter = bunny->add_component<MeshFilter>();
    auto mesh_displayer = bunny->add_component<MeshDisplayer>();
    auto rigid_body = bunny->add_component<RigidBodyDynamic>();
    auto mesh_collider = bunny->add_component<MeshCollider>();
    transform->set_position(glm::vec3(0.f, 1.f, 0.f));
    transform->set_scale(glm::vec3(2.f, 2.f, 2.f));
    // set shader
    mesh_displayer->set_shader(shader);
    // set material
    auto tex = Texture2D::load_texture("../../resources/textures/bunny.jpg");
    auto mtl = std::make_shared<Material>();
    mtl->textures.push_back(std::make_pair("material.tex_diffuse", tex));
    mesh_displayer->set_material(mtl);
    // load mesh
    mesh_filter->load_mesh("../../resources/objects/Stanford_Bunny.obj");
    rigid_body->awake();
    mesh_collider->awake();

    // ================================================================
    Scene::Instance().add_object(gridbox);
    Scene::Instance().add_object(wall);
    Scene::Instance().add_object(bunny);

    viewer.run();
}


