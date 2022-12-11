# Day Nine

I decided to do part one with a vector that keeps track of the rope's movement. I'll keep it sorted so adding new locations is easy, and I'll just increment the count of locations instead of new "segments". Then at the end I can just count the length of the vector to see where it is.

Part one was pretty easy, but part two is a little bit more complicated...

I wonder if I can generalize my day one solution so it works with any length of rope...

Yep...

This took me a little bit to work through... It took me a little bit to conceptualize the movements of the ropes, but once I did I was able to get it.

Moving the rope to the old position occupied by the segment before it wouldn't work unless I did it in terms of relative locations (I think), and that would have required me to essentially rewrite everything.So instead I updated the rope's position with a group of 3 if-statements that took me a little bit to work out.

If the rope is 2 units away but in a straight line, you want to move towards it, and if it is more than 2 units away (on a diagonal) you want to move towards it diagonally. Not ever moving more than 1 in either direction turned out to be what I was missing.
