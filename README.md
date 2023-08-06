# Banana

en gros banana c'est le moteur et il faut le jeu qui définit les acteurs et leurs comportements en jeu.

## Project structure

This project contains two separate premake projects: Banana (the engine) and [insert game name here] (the game).

Banana is built to a library that needs to be linked to your game.

In the game project you will define all the actor's behavior and how Banana should handle them.

## Scenes

After you created all the actors you needed, you need to create a scene that includes the actors you created before.

You can switch scenes whenever you like.