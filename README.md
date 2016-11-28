## Hyphen
Hy-phen-ate (Eng-lish) words by syl-la-bles.

A C implement of Knuth-Liang algorithm optimized with Aho-Corasick automaton.

# Installation

	$ make install

# Usage
The `hyphen` reads words separated by blanks from the standard input and prints results one per line to the standard output.

	$ echo algorithm computer programming hacker mathematics hyphenate | hyphen
	al-go-rithm
	com-put-er
	pro-gram-ming
	hack-er
	math-e-mat-ics
	hy-phen-ate

Execute `hyphen -h` for details.
