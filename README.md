
# C++ Role-Based Strategy Game

This is a C++ implementation of a strategic multiplayer board game inspired by "Coup," developed as part of a university-level C++ course. Players take on various roles such as Governor, Spy, Baron, General, Judge, and Merchant. The goal is to eliminate other players through strategic actions such as coups, tax collection, investing, arrests, and more, until one winner remains.

The project was developed with full object-oriented design, includes action history tracking, and uses SFML for GUI rendering. It is modular, extensible, and designed to be tested with Doctest unit tests.

## Features

Modular design using classes for Game, Player, and specialized roles

Support for actions: gather, tax, bribe, invest, sanction, arrest, coup, undo, and disable

Full turn management and round-based logic

Game state tracking, including coins and player status

Undo mechanisms and special role-based abilities

Victory condition detection

Doctest-based unit tests

SFML-based GUI for interactive play (GameGUI)

## Requirements

- C++17 compiler
- [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
- Make
- Valgrind (for memory leak checks, optional)



## File Structure

- `Game.*` - Core game logic and rules.
- `Action.*` - Actions and their types.
- `GameGUI.*` - SFML-based graphical interface.
- `main.cpp` - Entry point of the game.
- `game_tests.cpp` - Unit tests using doctest.
- `Makefile` - Compilation instructions.



