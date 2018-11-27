//
// Created by Janči on 24/11/2018.
//
#ifndef PPGSO_SNAKE_H
#define PPGSO_SNAKE_H


#include <vector>
#include "object.h"
#include "scene.h"

class Snake final : public Object{
public:
    /*!
     * Create a new Snake
     */
    //std::list< std::unique_ptr<Object> > snake_sections;
    //std::unique_ptr<Object> first_section;
    Snake(Scene &scene);

    glm::vec3 check_limitations(glm::vec3 position);
    //bool snake_colision_happen(unique_ptr)

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

private:
    const float render_constatnt = 0.2;
    float render_time = -render_constatnt;
    int x_direction = -1;
    int y_direction = 0;
};


#endif //PPGSO_SNAKE_H
