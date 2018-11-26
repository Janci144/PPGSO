//
// Created by Janči on 24/11/2018.
//
#ifndef PPGSO_SNAKE_H
#define PPGSO_SNAKE_H


#include <vector>
#include "object.h"
#include "scene.h"

class Snake final : public Object{
private:

public:
    /*!
     * Create a new Snake
     */
    std::list< std::unique_ptr<Object> > snake_sections;

    Snake(Scene &scene);

    bool update(Scene &scene, float dt) override;

    void render(Scene &scene) override;

private:
    int render_time = -0.5;
    int x_direction = -1;
    int y_direction = 0;
};


#endif //PPGSO_SNAKE_H
