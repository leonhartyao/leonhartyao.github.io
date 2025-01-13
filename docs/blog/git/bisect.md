---
title: Find Bug-introducing Commit
description: Use binary search to find the commit that introduced a bug
comments: true
tags:
  - git
---

## What is git bisect

Git bisect uses a binary search algorithm to find which commit in your project’s history that changed any property of your project, normally the one introduced a specific bug.

## How to use git bisect

Basic git bisect commands are start, good, bad.
As an example, let's assume that the commit history is "... 0 ... 100". A feature is broken at the current commit 100, but is known to work at commit 0 (commit/tag).
We want to figure out with the help of git bisect, which commit introduced the bug.
We can start a bisect session and specify the bad and good commit:

```bash
git bisect start
git bisect bad  # Current commit is bad
git bisect good 0  # Commit 0 is good
```
Git bisect will start the binary search by checking out the commit in the middle of the range.

For the example, it will checkout commit 50.
We can build the code and test whether the feature is broken. Tell bisect the result and it will carry on the binary search, and it will check out the next commit in the middle of the left range that needs testing.
Repeat this procedure until no more commit left to inspect. Git bisect will print out a description of the first bad commit. The reference `refs/bisect/bad` will be left pointing to it.

To clean up bisect session and return to the original HEAD, use

```bash
git bisect reset
```

By default, git will return to the commit that was checked out before `git bisect start`. 
It is possible to return to a specific commit with

```bash
git bisect reset <commit>
```

!!! note

    When a new bisect session starts, it will clean up the old bisection state.

## Alternate terms

When we looking for the commit caused a changed instead of breaking a feature, using the terms "good" and "bad" is confusing. Alternatively, "old" and "new" can be used.
```bash
git bisect start
git bisect old
git bisect new
```

You can even customize the term by starting with

```bash
git bisect start --term-new foo --term-old bar
git bisect bar
git bisect foo
```

## Visualization

To see the currently remaining suspects in `gitk`

```bash
git bisect view
```
