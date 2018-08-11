# Projects
This repository is a growing collection of various programming problems and solutions I
have worked on and found to be interesting.

My solutions can be found in the Solutions folder.

Additional personal information can be found in the Other folder.

## Topics

### Graphics & Image Analysis
 - [WIP] ***Image Tiling Project***: Reconstruct an image using tiles determined by MSE
   and average image color

 - **Registration Errors Demonstration**: Demonstrate some key registration concepts,
   including fiducial localization error, fudicial registration error, and target registration error

### Machine Learning & Artificial Intelligence

### Graph
 - **Reordering a Tree**: Reorder an N-ary tree by decreasing order of the size of subtrees recursively

 - **Efficient Array Combination**: Implement abstract data type representing an array and
   support efficient `ArrayCombine()` and the typical `create()`, `destroy()`, `get()`, `set()`, and `size()`


### Text

 - **Deck of Cards**: Implement a data type representing a deck of cards with time-efficient
   `create()`, `destroy()`, `get()`, `put()`, `print()`, `split()`, and `shuffle()`

 - **Shrink Text Solitaire**: Simplify a string in a solitaire-manner with the following rules:
     1. Pick two cards that are either adjacent or separated by two other cards.
     2. The cards match if they are both vowels or if ASCII values differ by at most 5.
     3. If cards match, replace the left card with the right card and remove the left card.
     4. The game terminates if the original word can be reduced to one character.

 - **3-Dimensional Ants**: Move 256 ants in a universe of dimension 2<sup>32</sup> × 2<sup>32</sup> × 2<sup>32</sup>.
     - Set current ant to following character: \\n
     - Directional characters (move by one): [(h, j), (k, l), (<, >)]
     - Double character position: *
     - Write current ant to current position: .
     - Read character at position and print: ?
     - Terminate: EOF

 - **Transposition Cipher**: Simple transposition cipher (inputs `n`, `a`, and `b`) that
   scrambles text in blocks of character length n using the rule `j = (ai + b) mod n`

 - **Pig Esperanto**: Encode a string using Pig Esperanto

### Numbers
 - **Efficient, Large, Non-Negative Integer Calculations**: Implement a data type supporting
   addition and multiplication of large non-negative integers

### Files

### Utilities

## Contributions to Open Source Projects

* [SimpleITK](../../../SimpleITK)
* [SimpleITK-Notebooks](../../../SimpleITK-Notebooks)