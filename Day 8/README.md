# Day Eight

Today (at least for part 1, I don't know what part 2 is yet) one could just go through each tree and then check the rows to see if it is visible. While this isn't terrible, I think that there is a better way.

If I go from the edge, and move in a given direction, I can keep track of which trees are visible, and increment a "tallest tree" count for trees after. This would allow me "solve" a row / column of trees with only a single loop through the input of the row / column, instead of a looping through part of the input for each tree in that row / column. This should be $O(N)$ instead of $O(N^2)$.

I'll keep track of which trees can be seen in a separate array, and then at the end count up how many trees have been seen.

Part one was easy, though I decided not to try to force the 4 different "looking" directions into a single block of (probably) 4 loops.

Part two however... Is there really no way to do this accept for checking the entire row / column from each tree's perspective? I can't think of any that work in every case... So I guess I just brute force this? Dang...

I gave up on trying to find a "spiffy" method for part 2. Brute forcing it is still "instant" but I worry it isn't optimal.

I also gave up trying to figure out a way to avoid having 4 loops that essentially do the same thing. I figured out a way to do it with a `direction` variable and a `swap` variable, but it did *not* look pretty... So I decided to just do the 4 loops approach.
