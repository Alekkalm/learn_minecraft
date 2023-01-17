//
// Created by Иван Ильин on 14.03.2021.
//

#include "Player.h"
#include "engine/utils/Log.h"

Player::Player() : 
RigidBody(Mesh::Cube(ObjectNameTag("Player"))) {
    // TODO: implemented (lesson 2)
    setAcceleration(Vec3D(0, -MinecraftConsts::GRAVITY, 0));
    setCollision(true);
    setColor({240,168,168});
    setVisible(true);//сделаем игрока невидимым
    scale(Vec3D(0.5, 0.8, 0.5)*MinecraftConsts::WORLD_SCALE);
}

//меняем тип блока который выбран у игрока, который сейчас он может поставить
void Player::nextBlock() {
    // TODO: implemented (lesson 2)
    //увеличиваем каждый раз на 1.
    //grass=0 ... none=8
    //каждый раз прибавляем остаток от деления на 8:
    //selectedBlock = (0 + 1)%8 = 1%8 = 1 //т.к. 1/8 = 0 целых, остаток 1.
    //selectedBlock = (1 + 1)%8 = 2%8 = 2 //т.к. 2/8 = 0 целых, остаток 2.
    //..
    //selectedBlock = (6 + 1)%8 = 7%8 = 7 //т.к. 7/8 = 0 целых, остаток 7.
    //selectedBlock = (7 + 1)%8 = 8%8 = 0 //т.к. 8/8 = 1 целых, остаток 0.
    _selectedBlock = static_cast<Cube::Type>(((int)_selectedBlock + 1) % (int) Cube::Type::none);
}

//меняем тип блока который выбран у игрока, который сейчас он может поставить
//как в методе nextBlock(), только в другую сторону
void Player::previousBlock() {
    // TODO: implemented (lesson 2)
    if(int(_selectedBlock > 0)){
            _selectedBlock = static_cast<Cube::Type>((int)_selectedBlock - 1);
    }else{
        _selectedBlock = static_cast<Cube::Type>((int)Cube::Type::none - 1);
    }
}

void Player::collisionCallBack(const CollisionInfo &info) {
    // setting vertical velocity to zero after collision

    Vec3D velocity_parallel = info.normal * velocity().dot(info.normal);
    Vec3D velocity_perpendicular = velocity() - velocity_parallel;

    if (velocity().dot(info.normal) > 0) {
        setVelocity(velocity_perpendicular);
    }
}
