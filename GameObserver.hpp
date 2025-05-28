#pragma once
#include <string>
#include <vector>
#include "GameUpdate.hpp"



class GameObserver {
    public:
        virtual void onGameStateChanged(const GameUpdate& update) = 0;
        virtual ~GameObserver() {}
    };