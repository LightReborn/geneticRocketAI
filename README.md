# geneticRocketAI
Simple ai to get a grasp on genetic algorithms. The idea is that the rockets in a two dimensional plane start at a single spot, and have the goal of getting to another spot on the screen.
The genetic code will be built with two types of genes. Move and AdjustAngle. If it has a move then it moves along it's vector by a magnitude, the vector is stored in the rocket object. If it has adjustAngle then the vector is modified to change the angle by some magnitude.
Magnitudes of change for move, and adjustAngle will be controlled by a singleton called settings, where the two variable will be declared, and instantiated along with other variables that may be needed throughout the project.
------
It should be noted that the current build has a bug in it that causes erratic behaivior and does not tend towards a solution as intended.
