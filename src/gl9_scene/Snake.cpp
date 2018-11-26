//
// Created by Janči on 24/11/2018.
//

#include "Snake.h"
#include "SnakeSection.h"

using namespace std;
using namespace glm;
using namespace ppgso;

Snake::Snake(Scene &scene) {
    auto first_section = make_unique<SnakeSection>();

    first_section->position.y = 0;
    first_section->update(scene,0);
    first_section->render(scene);

    snake_sections.push_back(move(first_section));
//    for ( auto& obj : objects )
//        obj->render(*this);
    //scene.objects.push_back(move(first_section));
}

bool Snake::update(Scene &scene, float dt) {

    float time = glfwGetTime();

    if(scene.keyboard[GLFW_KEY_LEFT]) {
        x_direction = 1;
        y_direction = 0;
    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
        x_direction = -1;
        y_direction = 0;
    } else if(scene.keyboard[GLFW_KEY_UP]) {
        x_direction = 0;
        y_direction = 1;
    }else if(scene.keyboard[GLFW_KEY_DOWN]) {
        x_direction = 0;
        y_direction = -1;
    }
//    auto it = std::begin(snake_sections);
//    auto first = it->get();
//    first->position += direction;
//    first->position += direction;
    //std::cout << render_time << "   " << int(time) << endl;

    if(time - render_time >= 0.5)
    {
        render_time++;

        for(auto& obj : snake_sections) {
            obj->position.x += x_direction;
            obj->position.y += y_direction;
            obj->update(scene, dt);
            //obj->position += direction;
            std::cout << "Co to robi" << endl;
        }
    }

    for(auto& obj : snake_sections)
        obj->render(scene);

    return true;
}

void Snake::render(Scene &scene) {
    //nechceme renderovat
}

//bool Snake::update(Scene &scene, float dt) {
//    // Fire delay increment
//
//    // Hit detection
//    for ( auto& obj : scene.objects ) {
//        // Ignore self in scene
//        if (obj.get() == this)
//            continue;
//
//        // We only need to collide with asteroids, ignore other objects
////        auto asteroid = dynamic_cast<Asteroid*>(obj.get());
////        if (!asteroid) continue;
////
////        if (distance(position, asteroid->position) < asteroid->scale.y) {
////            // Explode
////            auto explosion = make_unique<Explosion>();
////            explosion->position = position;
////            explosion->scale = scale * 3.0f;
////            scene.objects.push_back(move(explosion));
////
////            // Die
////            return false;
////        }
//    }
//
//    // Keyboard controls
//    if(scene.keyboard[GLFW_KEY_LEFT]) {
//        position.x += 10 * dt;
//        rotation.z = -PI/4.0f;
//    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
//        position.x -= 10 * dt;
//        rotation.z = PI/4.0f;
//    } else {
//        rotation.z = 0;
//    }
//
//    // Firing projectiles
////    if(scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
////        // Reset fire delay
////        fireDelay = 0;
////        // Invert file offset
////        fireOffset = -fireOffset;
////
////        auto projectile = make_unique<Projectile>();
////        projectile->position = position + glm::vec3(0.0f, 0.0f, 0.3f) + fireOffset;
////        scene.objects.push_back(move(projectile));
////    }
//
//    generateModelMatrix();
//    return true;
//}
//
//void Snake::render(Scene &scene) {
//    shader->use();
//
//    // Set up light
//    shader->setUniform("LightDirection", scene.lightDirection);
//
//    // use camera
//    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
//    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
//
//    // render mesh
//    shader->setUniform("ModelMatrix", modelMatrix);
//    //shader->setUniform("Texture", *texture);
//    shader->setUniform("OverallColor", color);
//    mesh->render();
//}
//
//void Snake::onClick(Scene &scene) {
//    cout << "Player has been clicked!" << endl;
//}
