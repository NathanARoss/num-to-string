# num-to-string

I created a minimal and efficient, but lossy, implementation of dtoa for use in WebAssembly.  Cost size has priority over execution speed and correctness.  In this code, I call math.h's pow() function to calculate powers of 10 for integer powers, but in production code these powers would be cached.

I took a few ideas from Florian Loitsch's paper "Printing Floating-Point Numbers Quickly and Accurately with Integers"
https://drive.google.com/file/d/0BwvYOx00EwKmcTZUM0Q4VW1DTHc/view.

Specifically:
1) I generate digits from an incremented or rounded up version of the number to be printed rather than from the number itself.  This avoids the needs to check if the last digit rounded up still fits within the upper bound.

2) Rather than checking if the remaining digits are perfectly above a lower bound or below an upper bound that rounds to the original number, I instead calculate a range of acceptable values that I terminate once the remaining digits are less than.  This greatly simplifies the digit generation loop since I am comparing the remaining digits to one range rather than two bounds.

3) Generating digits using floating point logic is fine as long as you have a few extra bits available to avoid losing data.  It is not required to generate digits using rational numbers or large precision integers.

My implementation diverges from Loitsch's because I do not require the digits I output to uniquely identify the original number. I use the built-in 64 bit floating point format for multiplication, subtraction, and flooring rather than creating my own floating point format.  This drastically reduces the code I need to maintain at the cost of precision.
