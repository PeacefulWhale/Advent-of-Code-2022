# Day Twenty One

I could have probably made a tree with the input (I feel like that was what the challenge wanted), but I was too lazy to read through the input to do that, so instead I just stuck it all in an `unordered_map`... It's probably less performant as I don't actually need random access, but it's quick enough for my purposes.

Part one is a simple recursive function that does the required math on all the leaves.

Part two searches towards `humn` and applies the inverse of the operations on leaves when it "branches" towards `humn`. TL;DR: It does the required math on all the leaves (but occasionally the inverse math).

I forgot about order of operations for a little bit, which gave me a headache because it solved the example just fine, but not my input. Once I remembered that the order of operators was important for certain operations, I finished up part two easily.
