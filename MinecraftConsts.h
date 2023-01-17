//
// Created by Иван Ильин on 23.02.2022.
//

#ifndef MINECRAFT_MINECRAFTCONSTS_H
#define MINECRAFT_MINECRAFTCONSTS_H

namespace MinecraftConsts {
    const double WORLD_SCALE = 10; //масштабирование кубиков и скорости
    const double REACH_RANGE = 10; //максимальная длинна с которой мы можем ставить и ломать блоки (измеряется в кубиках)

    const double GRAVITY = 20;
    const double JUMP_HEIGHT = 1.2;
    const double WALK_SPEED = 5;
    const double MOUSE_SENSITIVITY = 1.0 / 1000.0;

    const std::string MAP_NAME = "maps/map_test";
    const std::string AIM_TEXTURE = "textures/aim.png";
}

#endif //MINECRAFT_MINECRAFTCONSTS_H
