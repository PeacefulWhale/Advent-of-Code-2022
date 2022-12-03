# Day Three

Today's was pretty simple. The first part was just finding matching characters within two halves of a line and adding them up. The second part was the same as the first part, but across 3 entire lines.

I didn't do anything fancy, I read and then looped through the lines until I got a match. I think that C++ has a `find` function, but a for loop works as well so why bother?

Also I was able to figure out how to map the values without an if statement for the capital values: `((item - 96) + 58) % 58`. However, I don't think that this is quicker or more readable than the ternary statement... Especially with that many modulo statements...

Yeah, I tested this with [Hyperfine](https://github.com/sharkdp/hyperfine) (which is a pretty neat terminal utility), and using the ternary statement drops a full millisecond off, which is pretty significant because the quicker version runs in only 2 milliseconds.
