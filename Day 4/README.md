# Day Four

Today was pretty simple thanks to C++ `sstream` library and `>>` + `<<` operators (not the bit shift one...)

Once I parsed the input I only need 2 if statements for this.

Part 1: `((i <= x && j >= y) || (i >= x && j <= y))`

Part 1 is just a simple bounds check to see if they intersect.

Part 2: `(y - i <= (j - i) + (y - x))`

I noticed that the second pair is never before the first pair, so I don't need to do `(std::max(j, y) - std::min(i, x) <= (j - i) + (y - x))`.

Part 2 uses the distance between the two edges, and then checks to see if the width of the two sections together are greater than that distance. If they are, then they must be intersecting.

I wish I had been at home right as this was released today, as I could have probably gotten a decent score. It only took me about 5 minutes for part 1 and 2.
