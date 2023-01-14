---
# title: "[TIL] How to Escape Characters in YAML Front Matter"
description:
comments: true
tags:
  - MkDocs
---

# How to Escape Characters in YAML Front Matter

To escape characters in YAML front matter, add a `>` (greater than) symbol and put the line containing the character on a new line.

```yaml
---
title: >
  Docs: add custom build example for Docsify
---
```

Actually, you should quote all strings that are not simple, alpha-numeric values. You can use double quotes if you want to use escape sequences or use single quotes for literal strings:

```yaml
title: "[Learning notes.]"
title: "\"Learning notes.\""
title: "Learning notes: cpp"

title: '[Learning notes.]'
title: '"Learning notes."'
title: 'Learning notes: cpp'
```
