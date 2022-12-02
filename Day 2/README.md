# Day two

Apparently this is a rock-paper-scissors simulation thing... But part 1 to me seems to be deceptively simple. You can just read through line by line and keep tally of the total number of wins.

If I had to guess part 2 will ask you to find the total number of wins in A B C doesn't map directly to X Y Z (I find it suspicious that the elf didn't get to explain the strategy guide), or the scores will change.

So instead of just running through the file and tallying it up as I go, I'll go through and parse the file into a 2D array of who many of each occurrence there were, and then calculate the score. This should also be a little bit faster than tallying up as I go.

---

Okay, so I did part 1 and 2, and I was kinda right, but in the end it looks like doing it I could have done it either way. I think my way might be a *tad* bit faster, but I don't think it really matters.

I decided to write an alternative one, where I just did it in a loop. It works just as well and is a bit more compact. Also probably a little bit more readable.
