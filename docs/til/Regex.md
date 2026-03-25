---
# title: "[TIL] Regex (regular expressions)"
description:
comments: true
tags:
  - shell
---

Regex (Regular Expressions) is a powerful search-and-replace language. Instead of searching for a specific word, you describe the shape of the text you're looking for.

## The Literal & The Wildcard

    abc : Matches exactly "abc".
    . (dot) : Matches any single character (except a newline).
        Example: h.t matches "hat", "hit", or "h0t".

## Character Classes (The "Any Of" Box)

    [aeiou] : Matches any one vowel.
    [a-z] : Matches any lowercase letter.
    [0-9] : Matches any single digit.
    [^0-9] : Matches anything except a digit.

## Quantifiers (How Many?)
These follow a character to tell you how many times it repeats:

    * : 0 or more times.
    + : 1 or more times.
    ? : 0 or 1 time (makes it optional).
    {3} : Exactly 3 times.
    {2,5} : Between 2 and 5 times.

## Anchors (Where?)

    ^ : The start of a line.
    $ : The end of a line.
        Example: ^Error only finds "Error" if it's the first word on the line.

## Shorthand Escapes

    \d : Any digit (same as [0-9]).
    \w : Any "word" character (letters, numbers, underscores).
    \s : Any whitespace (space, tab, newline).
    \b : A word boundary (helps you find "cat" but not "catastrophe").

## Examples

Look at this pattern: ^\d{3}-\d{4}$

    ^ : Start at the beginning of the line.
    \d{3} : Find exactly 3 digits.
    - : Find a literal hyphen.
    \d{4} : Find exactly 4 digits.
    $ : Must be the end of the line.

    Result: It matches a phone number format like 555-1234.
