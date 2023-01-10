//
// Created by Иван Ильин on 15.03.2021.
//

#include "Map.h"
#include <iostream>
#include <fstream>

//добавление куба на карту (в мир)
void Map::addCube(const Vec3D& pos, Cube::Type t) {
    // TODO: implemented (lesson 1)
    //формируем имя куба из слова "Cube" и координат расположения куба.
    std::string name = "Cube_X_" + std::to_string((int)pos.x()) + "_Y_" + std::to_string((int)pos.y()) + "_Z_" + std::to_string((int)pos.z());

    world->addBody(std::make_shared<Cube>(ObjectNameTag(name), pos, t));//добавляем куб в мир
    //добавляем куб в карту (запишем себе в записную книжку что мы куда добавили)
    cubes.insert({name, {t, pos}});//добавляем в словарь: имя -> тип + позиция. (этот словарь - чтобы было легче сохранять и загружать карту)

}

//удаление куба
void Map::removeCube(const Vec3D& pos) {
    // TODO: implemented (lesson 1)
    //формирование имени куба - точно такое же как и у функции добавления.
    std::string name = "Cube_X_" + std::to_string((int)pos.x()) + "_Y_" + std::to_string((int)pos.y()) + "_Z_" + std::to_string((int)pos.z());

    world->removeBody(ObjectNameTag(name));//удаляем из мира
    cubes.erase(name);//удаляем из нашей записной книжки (из словаря)
}

//сохранение карты в файл
void Map::saveMap(const std::string& mapName) {
    // TODO: implemented (lesson 1)
    //открываем файл на запись с удалением всего что в нем было до этого.
    std::ofstream outfile(mapName, std::fstream::trunc); //конструктор

    //записываем данные наших кубов из словаря.
    for(auto& cube : cubes) {
        outfile << cube.second.first << "\t" <<
        (int)cube.second.second.x() << "\t" <<
        (int)cube.second.second.y() << "\t" <<
        (int)cube.second.second.z();
    }

    outfile.close();
}

//загрузка карты:
//читаем данные из файла, применяем метод addCube() - создаем кубы и добавляем информацию о них в словарь
void Map::loadMap(const std::string &mapName) {
    // TODO: implemented (lesson 1)
    //открываем файл на чтение
    std::ifstream infile(mapName, std::fstream::out); //конструктор


    while(!infile.eof()){
        int t, x, y, z; //временные переменные для чтения информации об одном кубе.

        infile >> t >> x >> y >> z;
        addCube(Vec3D(x,y,z), Cube::Type(t)); //добавляем куб в мир и на карту.
    }

    infile.close();
}
