---
title: "[TIL] Put include in .hpp or .cpp"
description:
comments: true
tags:
  - C++
---

In general, you should only include headers in .hpp files that are needed by those headers. 
Otherwise, always include headers only in .cpp files. This keeps compilation time to a minimum and better shows what files are needed.

- The contents of a source file aren’t visible to other source files
- The header exposes the relevant interfaces to sources that will be resolved at linking
- The header should be able to be included in other source files without adding unforeseen dependencies

In short all of the above means that you should place every include into your source rather than your headers, whenever practical.
Headers are better if they have fewer dependencies.

