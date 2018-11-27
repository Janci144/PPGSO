//
// Created by Janči on 25/11/2018.
//

#include "SnakeSection.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <image_bmp.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;
using namespace ppgso;

// shared resources
unique_ptr<Mesh> SnakeSection::mesh;
unique_ptr<Texture> SnakeSection::texture;
unique_ptr<Shader> SnakeSection::shader;

SnakeSection::SnakeSection() {
    // scale *= 1.0f;
    scale = vec3{1,1,1};

    // Initialize static resources if needed
    if (!shader) shader = make_unique<Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = make_unique<Texture>(image::loadBMP("blbost.bmp"));
    if (!mesh) mesh = make_unique<Mesh>("cube.obj");
}

bool SnakeSection::update(Scene &scene, float dt) {
    // Fire delay increment

    position = check_limitations(position);
    std::cout << position.x << "  " << position.y << endl;

    // Hit detection
    for ( auto& obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with asteroids, ignore other objects
//        auto asteroid = dynamic_cast<Asteroid*>(obj.get());
//        if (!asteroid) continue;
//
//        if (distance(position, asteroid->position) < asteroid->scale.y) {
//            // Explode
//            auto explosion = make_unique<Explosion>();
//            explosion->position = position;
//            explosion->scale = scale * 3.0f;
//            scene.objects.push_back(move(explosion));
//
//            // Die
//            return false;
//        }
    }

    // Keyboard controls
//    if(scene.keyboard[GLFW_KEY_LEFT]) {
//        position.x += 10 * dt;
//        rotation.z = -PI/4.0f;
//    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
//        position.x -= 10 * dt;
//        rotation.z = PI/4.0f;
//    } else {
//        rotation.z = 0;
//    }

    // Firing projectiles
//    if(scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
//        // Reset fire delay
//        fireDelay = 0;
//        // Invert file offset
//        fireOffset = -fireOffset;
//
//        auto projectile = make_unique<Projectile>();
//        projectile->position = position + glm::vec3(0.0f, 0.0f, 0.3f) + fireOffset;
//        scene.objects.push_back(move(projectile));
//    }

    generateModelMatrix();
    return true;
}

void SnakeSection::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    //shader->setUniform("OverallColor", color);
    mesh->render();
}

glm::vec3 SnakeSection::check_limitations(glm::vec3 position) {
    glm::vec3 new_position = position;

    if(position.x > 8 )
        new_position.x = -8;
    if(position.x < -8 )
        new_position.x = 8;

    if(position.y > 8)
        new_position.y = -8;
    if(position.y < -8 )
        new_position.y = 8;

    return new_position;

}

void SnakeSection::onClick(Scene &scene) {
    cout << "Player has been clicked!" << endl;
}

