# How to run:
Compile with ```g++-12 -std=c++23 main.cpp -o a.out```.

Then, run with ```./a.out```.

#To debug:

Compile with ```g++-12 -std=c++23 main.cpp -o a.out -g```.

Then, run with ```gdb ./a.out```, and use the command "run".

## System Requirements

Needs c++23 obviously (might be able to make do with c++20 I forget but I think there was one thing that is only doable in c++23).

I have a ASUS Creator Laptop Q OLED, and the screen for this fits the grid relatively nicely (a bar at the bottom and right side but that is it) when I use the normal Windows Subsystem for Linux zoom out function about 4 times (I forget).

I'm using the Avenir Next LT Pro font that I ripped off some website for free in the normal Powershell terminal running WSL to try and get square characters. This didn't help very much but I'll keep experimenting.


## Details of output

I will make a graphical output later, but for now the output is to console.

Output is the class containing the methods to actually put things onto the screen (aka console). It is a singleton because we only want one output, and it has blit methods to take an image, delete the current display, and replace it with the supplied image.

Frame is where the images are made. It represents a slice of Output, which gets passed to Output in a draw request.

Each drawable entity (dancers, arms, etc) inherits from Drawable. Thus, any drawable entity has a .draw() method which can either take a Frame or not. If a frame is supplied, it draws to that frame. Each drawable object also gets its own Frame by default, which can be set. If .draw() isn't given a Frame,
it defaults to drawing on the object's own Frame.

We also have a MovingFrame class. This class receives a vector of MovingDrawable, which is a class containing both a Drawable and a heap called futures of the times and positions it needs to be drawn, sorted by next time first. MovingFrame runs an internal clock, and asks Output to draw frames at a specified interval. In between releases,
it asks each MovingDrawable to pop from its futures until it gets to a time at or after the current time, with time 0 being the time the MovingFrame was made. If that MovingDrawable needs to be drawn now, it does so, and if the heap is empty, it removes this MovingDrawable from consideration.

Output can garbage collect its own Frames if asked, so we needn't worry about that, but we can also pass by reference if we want to (but Output will lock the Frame while processing so we can't make the next frame). Thus, we pass a copy instead.

# Important commands in the .bashrc

```
alias comp="clear;g++-12 -std=c++23 !(\"smallmain\").cpp -o a.out"
alias go="comp;./a.out"

alias compg="clear;g++-12 -std=c++23 !(\"smallmain\").cpp -o a.out -g"
alias gog="compg;gdb ./a.out"

alias comps="clear;g++-12 -std=c++23 !(\"main\").cpp -o a.out"
alias gos="comps;./a.out"

alias compgs="clear;g++-12 -std=c++23 !(\"main\").cpp -o a.out -g"
alias gogs="compgs;gdb ./a.out"
```