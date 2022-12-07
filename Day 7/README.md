# Day Seven

Today I spent most of my time creating the directory hierarchy from the input file.

Once I had that, solving the problem was quite easy. I used recursion for both part 1 and part 2.

I used vectors because I was too lazy to handle dynamic arrays. I originally too lazy to set up a newer standard of C++ on my intellisense / compiler so I didn't use any of the fancy `auto x : vector` stuff... but then I decided to actually put in some miniscule amount of effort to make the code prettier. Adding `c++20` to my intellisense settings wasn't as difficult as I feared it would be.

With the benefit of hindsight there are a few optimizations I could make:

- A directory is never `cd`-ed into more than once, so you don't need the directory file names.
- Files themselves are not used, so you can just add their sizes to the directory that they are contained in.

I'm too lazy to go back and change my code to include these optimizations however.
