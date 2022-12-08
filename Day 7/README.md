# Day Seven

Today I spent most of my time creating the directory hierarchy from the input file.

Once I had that, solving the problem was quite easy. I used recursion for both part 1 and part 2.

I used vectors because I was too lazy to handle dynamic arrays. I originally too lazy to set up a newer standard of C++ on my intellisense / compiler so I didn't use any of the fancy `auto x : vector` stuff... but then I decided to actually put in some miniscule amount of effort to make the code prettier. Adding `c++20` to my intellisense settings wasn't as difficult as I feared it would be.

With the benefit of hindsight there are a few optimizations I could make:

- A directory is never `cd`-ed into more than once, so you don't need the directory file names.
- Files themselves are not used, so you can just add their sizes to the directory that they are contained in.

Adding in these optimizations (even though I've already gotten the stars for today) cuts down about 50 lines of code, and saves a fair bit of space.

I wonder why the problem didn't state this. I wouldn't be surprised if it was just a quirk of how the input was generated.

I could also probably optimize out the actual tree structure, as I can just have a list of directory sizes in the end... However I'm too lazy to do that right now... Maybe later.
