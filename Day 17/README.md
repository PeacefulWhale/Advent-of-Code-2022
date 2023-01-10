# Day Seventeen

For part one I simply simulated the falling shapes in a "large" character array...

However, I doubt that this would work for part two, even if I had enough memory to create an array large enough.

It's possible that I could store only parts of the current stack (perhaps down to where it's impossible for any brick to get below), but then I would still have to drop 1 trillion blocks... If I had to guess this would still take a very long time to run.

So I'll instead save the current state as a hash map, and look for when it begins to repeat. Once I find a repetition I should be able to rapidly "build" a tower by just keeping track of it's height. Once it is too close to the target to complete an entire pattern, I'll simulate it again until it reaches the target.

I was able to get the example pretty quickly with a simple hashmap method, however my actual input was proving to be a little bit more difficult... Which is really weird because one would figure that if it works for the example, it would work for the actual input as well...

However it turns out that I just needed a slightly larger cache, as my input doesn't have a repeated pattern until 3430 blocks in at a height of 5199... So my old `MAX_HEIGHT` of `5000` was just too small to solve this problem. This also means that my input doesn't have a repeating pattern for part 1.
