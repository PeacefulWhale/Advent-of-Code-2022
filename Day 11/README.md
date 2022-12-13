# Day Eleven

Today wasn't terribly difficult. Once I got the monkeys parsed, simulating them was easy. I did stumble a little bit at the start because I didn't realize that the input had `old * old`... So I had to make a special case so I could parse that.

I also didn't have any `-` or `/` in my input, and that's not in the example, so I'm guessing we always multiply.

Part 2 instantly stood out to me as "you're going to need to mod this", probably because it's quite similar to some of the CTF challenges I've done. The chinese remainder theorem / all that mod math madness is hammered into me at this point.

Modding the values was pretty quick and easy, though I did upgrade some of my `int` variables to `unsigned long` variables just in case they got too big even with the mod. I did end up needing this as the answer to part 2 was larger than an `unsigned int`.

I did end up using both `stack` and `vector`, but I think that using them here makes sense, and they're both pretty light weight when used in the way I did.
