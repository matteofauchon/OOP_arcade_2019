/*
** EPITECH PROJECT, 2020
** OOP_arcade_2019
** File description:
** Game.cpp
*/

#include "Game.hpp"

#include <fstream>
#include <iostream>

#include "../../../engine/component/AAI.hpp"
#include "../../../engine/component/AUser.hpp"
#include "../../../engine/component/Animations.hpp"
#include "../../../engine/component/Hitbox.hpp"
#include "../../../engine/component/Motion.hpp"
#include "../../../engine/component/Size.hpp"
#include "../../../engine/component/Transform.hpp"
#include "../../../engine/system/Movement.hpp"
#include "../../../engine/system/Physics.hpp"
#include "component/AI.hpp"
#include "component/User.hpp"
#include "system/AI.hpp"
#include "system/User.hpp"

static void initEntity(engine::ecs::World& world);
static void initSpecialPacGums(engine::ecs::World& world, int x, int y);
static void initVoid(engine::ecs::World& world, int x, int y);
static void initGhost(engine::ecs::World& world, int x, int y);
static void initPacGums(engine::ecs::World& world, int x, int y);
static void initWall(engine::ecs::World& world, int x, int y);
static void initPacman(engine::ecs::World& world, int x, int y);
static void initBackGround(engine::ecs::World& world);
//static void errorMapHandler();

game::Game::Game(engine::ecs::Universe& universe): AGame("PACMAN", universe)
{
}

extern "C"  game::IGame* create(engine::ecs::Universe* universe) {
    return new game::Game(*universe);
}

void game::Game::init()
{
    auto& world = getUniverse().createWorld("main");
    initBackGround(world);
    initEntity(world);
    world.addSystem<pacman::system::AI>();
    world.addSystem<pacman::system::User>();
    world.addSystem<engine::system::ARender>();
    world.addSystem<engine::system::Physics>();
    world.addSystem<engine::system::AAnimations>();
    world.addSystem<engine::system::Movement>();
    world.addSystem<engine::system::AAudio>();
    getUniverse().setCurrentWorld("main");

}
void game::Game::destroy()
{
    getUniverse().deleteWorld("main");
}

/*void errorMapHandler()
{
    std::ifstream file;
    file.open(MAP_PATH);
    if (!file.is_open())
        throw std::exception();
    std::string string;
    int y = 0;
    for (; getline(file, string); y++) {
        for (const auto& i : string) {
            if (i != '1') {
                file.close();
                throw std::exception();
            }
        }
        for (const auto& i : string) {
            if (i != '0' && i != '1' && i != '2' && i != '3' && i != '4' && i != '5') {
                file.close();
                throw std::exception();
            }
        }
        if (string[0] != '1') {
            int i = 0;
            for (; string[i] == '0'; i++);
            if (i != '1') {
                file.close();
                throw std::exception();
            }
        }
        if (string[string.size()] != '1')
        {
            int i = 0;
            for (i = string.size(); string[i] == '0'; i--);
            if (i != '1') {
                file.close();
                throw std::exception();
            }
        }
        if (y == 20) {
            for (const auto& i : string) {
                if (i != '1') {
                    file.close();
                    throw std::exception();
                }
            }
        }
    }
    if (y != 20) {
        file.close();
        throw std::exception();
    }
}*/

static void initEntity(engine::ecs::World& world)
{
    std::ifstream file;
    std::string string;
    //errorMapHandler();
    file.open(MAP_PATH);
    for (int y = 0; getline(file, string); y++) {
        for (size_t x = 0; x < string.size(); x++) {
            switch (string[x]) {
                case '1':
                    initWall(world, x, y);
                    break;
                case '2':
                    initPacGums(world, x, y);
                    break;
                case '3':
                    initPacman(world, x, y);
                    break;
                case '4':
                    initGhost(world, x, y);
                    break;
                case '5':
                    initSpecialPacGums(world, x, y);
                    break;
                default:
                    initVoid(world, x, y);
                    break;
            }
        }
    }
    file.close();
}

static void initBackGround(engine::ecs::World& world)
{
    auto& bg = world.createEntity();
    bg.addComponent<engine::component::ARender>(BG_PATH);
    bg.addComponent<engine::component::Transform>(engine::type::Vector2D(0, 0), 0);
}

static void initPacman(engine::ecs::World& world, int x, int y)
{
    auto& pacman = world.createEntity();
    pacman.addComponent<pacman::component::User>();
    pacman.addComponent<engine::component::ARender>(PACMAN_PATH);
    pacman.addComponent<engine::component::Transform>(engine::type::Vector2D(x * DEFAULT_SIZE_BLOCK + DEFAULT_POSITION, y * DEFAULT_SIZE_BLOCK), 2);
    pacman.addComponent<engine::component::Animations>(PACMAN_ANIMATION);
    pacman.getComponent<engine::component::Animations>().currentAnimation = "living";
    pacman.addComponent<engine::component::Motion>(engine::type::Vector2D(0, 0), engine::type::Vector2D(0, 0));
    pacman.addComponent<engine::component::Hitbox>(40, 40);
    pacman.addComponent<engine::component::Size>(40, 40);
}

static void initWall(engine::ecs::World& world, int x, int y)
{
    auto& wall = world.createEntity();
    wall.addComponent<engine::component::ARender>(WALL_PATH);
    wall.addComponent<engine::component::Transform>(engine::type::Vector2D(x * DEFAULT_SIZE_BLOCK + DEFAULT_POSITION, y * DEFAULT_SIZE_BLOCK), 2);
    wall.addComponent<engine::component::Hitbox>(40, 40);
    wall.addComponent<engine::component::Size>(40, 40);
    world.addToGroup(wall, "wall");
}

static void initPacGums(engine::ecs::World& world, int x, int y)
{
    auto& pacGums = world.createEntity();
    pacGums.addComponent<engine::component::ARender>(PACGUMS_PATH);
    pacGums.addComponent<engine::component::Transform>(engine::type::Vector2D(x * DEFAULT_SIZE_BLOCK + 20 - 4 + DEFAULT_POSITION,y * DEFAULT_SIZE_BLOCK + 20 - 4), 1);
    pacGums.addComponent<engine::component::Hitbox>(8, 8);
    pacGums.addComponent<engine::component::Size>(8, 8);
    world.addToGroup(pacGums, "pacGums");
}

static void initGhost(engine::ecs::World& world, int x, int y)
{
    auto& ghost = world.createEntity();
    ghost.addComponent<pacman::component::AI>();
    ghost.addComponent<engine::component::ARender>(GHOST_PATH);
    ghost.addComponent<engine::component::Transform>(engine::type::Vector2D(x * DEFAULT_SIZE_BLOCK + DEFAULT_POSITION,y * DEFAULT_SIZE_BLOCK), 2);
    ghost.addComponent<engine::component::Animations>(GHOST_ANIMATION);
    ghost.getComponent<engine::component::Animations>().currentAnimation = "living";
    ghost.addComponent<engine::component::Motion>(engine::type::Vector2D(0, 0), engine::type::Vector2D(1, 1));
    ghost.addComponent<engine::component::Hitbox>(40, 40);
    ghost.addComponent<engine::component::Size>(40, 40);
    ghost.getComponent<pacman::component::AI>().startPosX = x * DEFAULT_SIZE_BLOCK + DEFAULT_POSITION;
    ghost.getComponent<pacman::component::AI>().startPosY = y * DEFAULT_SIZE_BLOCK;
    world.addToGroup(ghost, "ghost");
}

static void initVoid(engine::ecs::World& world, int x, int y)
{
    auto& empty = world.createEntity();
    empty.addComponent<engine::component::ARender>(VOID_PATH);
    empty.addComponent<engine::component::Transform>(engine::type::Vector2D(x * DEFAULT_SIZE_BLOCK + DEFAULT_POSITION,y * DEFAULT_SIZE_BLOCK), 0);
    empty.addComponent<engine::component::Hitbox>(40, 40);
    empty.addComponent<engine::component::Size>(40,  40);
}

static void initSpecialPacGums(engine::ecs::World& world, int x, int y)
{
    auto& specialPacGums = world.createEntity();
    specialPacGums.addComponent<engine::component::ARender>(SPECIALPACGUMS_PATH);
    specialPacGums.addComponent<engine::component::Transform>(engine::type::Vector2D(x * DEFAULT_SIZE_BLOCK + 20 - 8 + DEFAULT_POSITION,y * DEFAULT_SIZE_BLOCK + 20 - 8), 1);
    specialPacGums.addComponent<engine::component::Hitbox>(16, 16);
    specialPacGums.addComponent<engine::component::Size>(16, 16);
    world.addToGroup(specialPacGums, "specialPacGums");
}