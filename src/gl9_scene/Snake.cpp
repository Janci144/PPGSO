//
// Created by Janči on 24/11/2018.
//

#include "Snake.h"
#include "SnakeSection.h"
#include <limits>

#include "Candy.h"

using namespace std;
using namespace glm;
using namespace ppgso;

Snake::Snake(Scene &scene) {
    auto first_section = make_unique<SnakeSection>();

    first_section->position.y = 0;
    scene.objects.push_back(move(first_section));

    //snake_sections.push_back(move(first_section));
//    for ( auto& obj : objects )
//        obj->render(*this);
    //scene.objects.push_back(move(first_section));
}

bool Snake::update(Scene &scene, float dt) {

    float time = glfwGetTime();

    if(scene.keyboard[GLFW_KEY_LEFT]){
        if(!(x_direction == -1 && y_direction == 0)) {
            x_direction = 1;
            y_direction = 0;
        }
    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
        if(!(x_direction == 1 && y_direction == 0)) {
            x_direction = -1;
            y_direction = 0;
            }
    } else if(scene.keyboard[GLFW_KEY_UP]) {
        if(!(x_direction == 0 && y_direction == -1)) {
            x_direction = 0;
            y_direction = 1;
        }
    }else if(scene.keyboard[GLFW_KEY_DOWN]) {
        if(!(x_direction == 0 && y_direction == 1)) {
            x_direction = 0;
            y_direction = -1;
        }
    }
//    auto it = std::begin(snake_sections);
//    auto first = it->get();
//    first->position += direction;
//    first->position += direction;
    //std::cout << render_time << "   " << int(time) << endl;

    //po stanovenom case sa obnovia pozicie snake-a
    if(time - render_time >= render_constatnt)
    {
        render_time += render_constatnt;
        //first_section->position.x += x_direction;
        //first_section->position.y += y_direction;
        bool first = true;
        glm::vec3 prev_pos{0,0,0};
        SnakeSection *first_section;

        for(auto& obj : scene.objects) {
            auto section = dynamic_cast<SnakeSection*>(obj.get());

            //menia sa pozicie jednotlivych castii snake-a
            if(section) {

                obj->position = check_limitations(obj->position);

                if(first) {
                    first_section = section;
                    prev_pos = obj->position;
                    obj->position.x += x_direction;
                    obj->position.y += y_direction;
                    first = false;
                }else{
                    glm::vec3 temp = obj->position;
                    obj->position = prev_pos;
                    prev_pos = temp;
                }

            }
        }

        //detekcia zrazky s Candy objektom alebo so sebou
        for(auto& obj : scene.objects) {
            auto candy = dynamic_cast<Candy*>(obj.get());

            if(!candy)
                continue;

            if(candy) {
                if (distance(first_section->position, obj->position) < (obj->scale.y + first_section->scale.y) * 0.5f) {

                    if (candy) {
                        auto new_section = make_unique<SnakeSection>();

                        new_section->position = prev_pos;
                        scene.objects.push_back(move(new_section));

                        //dorobit explode
                       candy->destroy();
                    }

                    break;
//            int pieces = 3;
//
//            // Too small to split into pieces
//            if (scale.y < 0.5) pieces = 0;

                    // The projectile will be destroyed
                    //if () ->destroy();
                    //section->destroy();

                    // Generate smaller asteroids
                    //explode(scene, (obj->position + position) / 2.0f, (obj->scale + scale) / 2.0f, pieces);

                    // Destroy self
                }
            }
        }
    }



    return true;
}



glm::vec3 Snake::check_limitations(glm::vec3 position) {
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
