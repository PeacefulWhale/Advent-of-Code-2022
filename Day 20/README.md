# Day Twenty

For today I was thinking about using a doubly linked list, and then I realized that `std::vector` had everything I needed anyway. Last year I might have done my own linked list implementation, but this year i wanted to get more familiar with the C++ standard library.

For me the hardest part was getting the wrapping working right, because I didn't realize that inserting into 0 wasn't a valid option for a very long time... The fact that part 1 worked without this knowledge also proved to be a stumbling block. Once I realized that the valid range of possible insertions was `1` to `size - 1` I quickly solved today's challenge.

I could probably make my `wrap` function a single line of math, but I'm too lazy to work through that right now, so it will stay as 3 if statements.

Also I realized that `for (auto &x : part_2) x.value *= 811589153;` is a thing you can do. I think that's pretty neat.
