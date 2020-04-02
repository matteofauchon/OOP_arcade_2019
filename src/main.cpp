/*
** EPITECH PROJECT, 2020
** OOP_arcade_2019
** File description:
** main.cpp
*/

#include <unistd.h>

#include "engine/core/Core.hpp"

int main()
{
    engine::core::Core core;

    core.loadGames();
    core.loadGraphics();

    core.setCurrentGraphical("lib_arcade_sfml.so");
    core.setCurrentGame("emulator");

    core.getCurrentGraphical().init();
    core.getCurrentGame().init();

    auto &universe = core.getUniverse();

    universe.init();

    for (int i = 0; i >= 0; ++i) {
        core.getCurrentGraphical().dispatchEvent();
        universe.update();
        universe.render();

        usleep(16666);
    }
}