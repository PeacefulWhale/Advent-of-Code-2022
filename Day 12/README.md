# Day Twelve

Today's challenge is a simple path finding challenge.

If my brain isn't completely dead, I'm pretty sure that this challenge should work fine with a breadth-first algorithm, though implementing an A* algorithm probably wouldn't be too difficult (though I don't know if it would run any faster in this scenario).

I'm slightly concerned that part 2 for this challenge is going to punish a slow algorithm, but I'll see how far a BSF algorithm gets me.

I don't actually need to generate any trees or node connections, but can instead just use a queue / stack / vector (it doesn't matter) of current locations. I ended up using a nested loop with 2 queues so I could keep track of the depth. I could have used a stack, but mentally I thought the process through with just one queue first, so using a queue made more sense to me.

For part 1 I just did a BSF search from the start to the end.

It appears that I didn't need to worry about using a BSF algorithm for part 1, as part 2 seems easier than part 1 (both computationally and logically).

For part 2 I made it so my part 1 solution could accept a flag which would determine what the "finished" condition was. Then I just passed it the end location as the start location, and searched for the first path that had an elevation of `a`.
