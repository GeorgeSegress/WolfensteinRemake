# WolfensteinRemake
A fully functional psuedo-3D Raycast rendering game engine recreating the original Wolfenstein created in C++. The project features a customizable level editor that allows for rapid iteration and development of levels faithful to the original game. The raycast renderer is lightweight and able to adapt to different wall patterns. 

# Structure
Generally the Controller.cxx file works as the overarching "engine" script, calling the model's draw functions and handling win and loss functions. 

The Model.cxx file contains the actual raycasting information and is the most complex and important piece of code. It handles level reading from files and performs the actual raycast rendering. The View.cxx handles the higher level rendering functions, calling the model's raycasting and object scanning functions. Generally the raycast function works using mathematics and linear algebra to rapidly calculate raycast positions as it sweeps across the screen each frame, rendering any other object that inherits from the Object.cxx class, scaling them based on distance. The get_bits_of_line function is a sophisticated and lightweight algorithm to rescan each line using recursive midpoint averages to find all additional objects between the scanned line and player position. This allows objects to be off access and prevent unessecary calls to objects that may be behind a visible wall. 

All other scripts work standard to normal games, with player.cxx handling movement, and grunt.cxx handling rudimentary AI and enemy animations, etc.

# Demonstrations
Top down raycasting demonstration
[LINK](https://youtube.com/shorts/aVMGVafDNmo)
Full gameplay demonstration with combat, scoring, health pickups, and a win condition
[LINK](https://youtube.com/shorts/LtafD_69Ots?feature=share)
