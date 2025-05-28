#pragma once

#include <string>
#include <algorithm>
#include <iostream>
#include "ActionType.hpp"
#include "Game.hpp"
#include "./include/Player.hpp"

// base class for all actions
class Action
{
public:
    ActionType getType() const;

    virtual bool equals(const Action *other) const;
    virtual ~Action();

protected:
    Game &game;
    Player *player;
    ActionType actionType;

    virtual void execute() = 0;
    virtual void undo() = 0;

    Action(Player *player, Game &game, ActionType actionT);
    friend class Game;
};

class TaxAction : public Action
{
private:
    int amount;

public:
    TaxAction(Player *player, Game &game, int amount);
    Player *getPlayer();
    void execute() override;

    void undo() override;
};
class disabel_taxAction : public Action
{
private:
    TaxAction *action;

public:
    disabel_taxAction(Player *player, Game &game, TaxAction *action)
        : Action(player, game, ActionType::DISABLE_TAX), action(action) {}

    void execute() override
    {
        if (action)
            action->undo();
    }

    void undo() override
    {
    }
};

class GatherAction : public Action
{
private:
public:
    GatherAction(Player *player, Game &game);

    void execute() override;
    void undo() override;
};

class arrestAction : public Action
{
private:
    Player *target;
public:
    arrestAction(Player *player, Player *target, Game &game);
    Player *getTarget();

    void execute() override;

    void undo() override;
};

class sanctionAction : public Action
{
private:
    Player *target;
    ActionType whattoblok;

public:
    sanctionAction(Player *player, Player *target, Game &game);
    Player *getTarget();
    void execute() override;

    void undo() override;
};

class coupAction : public Action
{
private:
    Player *target;

public:
    coupAction(Player *player, Player *target, Game &game);
    Player *getTarget();
    void execute() override;

    void undo() override;
};

class disabel_arrestAction : public Action
{
private:
    Player *target;

public:
    Player *getTarget();
    disabel_arrestAction(Player *player, Player *target, Game &game);
    void execute() override;

    void undo() override;
};
class bribeAction : public Action
{
private:
    int amount;

public:
    bribeAction(Player *player, Game &game);

    void execute() override;

    void undo() override;
};

class investAction : public Action
{
public:
    investAction(Player *player, Game &game);

    void execute() override;

    void undo() override;
};
class disabel_coupAction : public Action
{
private:
    coupAction *action;

public:
    disabel_coupAction(Player *player, Game &game, coupAction *action);

    void execute() override;

    void undo() override;
};
class seeCoinsOfAction : public Action
{
private:
    Player *target;

public:
    Player *getTarget();
    seeCoinsOfAction(Player *player, Player *target, Game &game);

    void execute() override;

    void undo() override;
};
