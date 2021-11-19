p.s     This is just the idea behind the implementation


Design Document of ManderBrot Set:
-----------------------------------
Packages Used:
	1)SFML-2.5.1 (Graphics)
	2)mingw32

Overview:
----------
	This project is divided up into several parts

First Task:
------------
	I have set up the project directory in vs code using a sfml boilker plate in which folders like lib,env and other such
	folders are set with the necessary sfml files so as to connect vs code and sfml graphics.

Second Task:
------------
	MandelBrot Set Algorithm:

	For this the reference is https://en.wikipedia.org/wiki/Mandelbrot_set from which i have obtained a brief
	understanding of what property does the Points plotted on the set mean.
		
		f(z)=z^2+c; this must not diverge to infinity when iterated from 0;

	A function which calculates the number of iterations before the value exceeds a certain value is designed


Third Task:
------------
	For plotting these points ,Here comes the use of SFML Using SFML we create a window with fixed width and height and
	load a sprite (drawable and trandsfomable image) with Texture to display the points. And now set each point its
	respective color as a function of number of iterations.
	Interactive parts:
		1) W - move up
		2) A - move left
		3) S - move down
		4) D - move right
		5) + - zoom in
		6) - - zoom out

Fourth Task:
------------
	Update each image with respective values of offsets and zooms so as to achieve the desired results.

On combining this 4 features I have completed the project.



   