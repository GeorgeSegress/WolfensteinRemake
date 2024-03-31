# WolfensteinRemake
A fully functional psuedo-3D Raycast rendering game engine recreating the original Wolfenstein created in C++. The project features a customizable level editor that allows for rapid iteration and development of levels faithful to the original game. The raycast renderer is lightweight and able to adapt to different wall patterns. 

# Structure
Generally the Model.cxx file works as an intermediate and the primary engine script, handling all frame updates and calling functions in every agent present from the player to the grunts in the levels. The View.cxx file contains the actual raycast rendering code, using mathematics and linear algebra to rapidly calculate raycast positions as it sweeps across the screen each frame, rendering any other object that inherits from the Object.cxx class, scaling them based on distance. 
