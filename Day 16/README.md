# Day 16

To solve part one I traversed the input connections to get the distances from one cell to another.

I didn't move to cells that didn't contain a valve to open, and I used a priority queue of states. I think I essentially ended up using some sort of BFS algorithm, but I'm not sure.

I also had some other minor optimizations... but I'm pretty sure that my solution isn't nearly as optimized as it could be.

For part two I just kept track of the paths that I generated, and then found the best pair that didn't visit the same locations... This is probably not the best way of doing this as it takes quite a while to run part two... I think my problem comes from the fact that there are a *lot* of generated paths, and my method for checking whether or not the pair is valid is also slow...

I might come back to this later, but for now it works... And it's not *that* slow... Part two finishes under a minute...

Okay... it's pretty dang slow...

Maybe future me will be able to make this go a bit faster, maybe not.
