# Day Thirteen

For this I have to parse packets. I won't lie, I think that this would be easier in python, where I can have lists of anything. However I think I can work around the need for creating a list of lists (or what have you) for this challenge.

Part one was decently simple once I built up my functions.

Part two requires me to sort all the packets based off the logic for whether or not the packets are ordered. This shouldn't be terribly difficult, as I can utilize an insertion sort with my `compare` function.

Yep, after adding a vector of the current elements, I just need to get the locations of where I insert `[[2]]` and `[[6]]`.

I did this by having my `insert` return the index that the value was inserted at, and the following function call `insert(&packets, convert("[[2]]")) * insert(&packets, convert("[[6]]"))`.

Honestly this feels like a day that could be done mostly through fancy parsing / regex / string stuff, but making a custom `packet` structure was just easier for me.
