ProjectStorage
==============

Storage for what should be my 3rd year project

For this commit the main aim was to get a DirectX 11 compatible project setup. 
Previously I was following a book called Introduction to DirectX11 Game Programming by Frank D Luna, 
but as it turned the beginning never managed to work, not even once. 
As there was apparently files missing from the disc given with it. 
And so I've been following a tutorial provided by rastertek.com which works you through establishing a starting framework
(rather than using someone else's premade framework without any understanding of what the hell is going on and noone willing to tell you anything about it.

That being said I'm trying to utilise my learning in a focused direction. I am on the 10th iteration now

This include another iteration which runs through 2d rendering. 
I did it in such a way that both the 3d rendering and the 2d can work at the same time. 
Its interesting. The z buffer is disabled when 2d stuff is being rendered and then reactivated when 3d rendering begins.
However I have still yet to make and actual model parser for obj files. At the moment that is being put on the backburner so that I can actually progress. 
In the mean time I will be working towards Terrain Generation and a variable camera.
The 2d rendering on its own actually interests me. 
As I could see myself making a framework for 2d games with it. 
It might be an interesting way to take my project in 4th year or maybe a fork in the road with regard my third year project
