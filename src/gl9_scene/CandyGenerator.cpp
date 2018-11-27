//
// Created by Janči on 24/11/2018.
//

#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>
#include "CandyGenerator.h"
#include "Candy.h"

using namespace std;
using namespace glm;
using namespace ppgso;

bool CandyGenerator::update(Scene &scene, float dt) {
    // Accumulate time
    time += dt;
    number_of_candys = 0;

    vector<pair<int, int>> snake_positions;
    for(auto& obj : scene.objects) {
        auto candy = dynamic_cast<Candy*>(obj.get());

        if(candy)
            number_of_candys++;

        auto snake_section = dynamic_cast<Candy*>(obj.get());
        if(snake_section)
            snake_positions.push_back(std::pair<int,int>(int(snake_section->position.x), int(snake_section->position.y)));

    }
//    auto it = std::begin(scene.objects);
//    if( *it == this)
//    scene.objects.erase(*this);

    // Add object to scene when time reaches certain level
    if ((time > create_after && number_of_candys  <= max_candys) || number_of_candys == 0) {
        number_of_candys++;
        create_after = rand() % 10 + 5;

        auto obj = make_unique<Candy>();
        //Treba osetrit, aby hra mala naozaj koniec
        //najdi miesto, kde sa moze zhodit Candy
        while(true)
        {
            bool can_be_deployed = true;
            int x = rand() % 16 - 8;
            int y = rand() % 16 - 8;

            for(auto pair : snake_positions)
            {
                if( pair.first == x && pair.second == y)
                   can_be_deployed = false;

            }

            if(can_be_deployed)
            {
                obj->position.x = rand() % 16 - 8;
                obj->position.y = rand() % 16 - 8;
                break;
            }
        }

        scene.objects.push_back(move(obj));
        std::cout << "ide to";
//        auto it = std::begin(scene.objects);
//
//        while (it != std::end(scene.objects)) {
//            // Najde seb
//            auto obj = it->get();
//            if( obj == this)
//                //scene.objects.erase(it);
//
//            it++;
//        }

//        auto obj = make_unique<Candy>();
//        obj->position = vec3(5,5,0);
        //obj->position.x += linearRand(-20.0f, 20.0f);

        //scene.objects.push_back(move(obj));
        time = 0;
    }

    return true;
}

void CandyGenerator::render(Scene &scene) {
    // Generator will not be rendered
}