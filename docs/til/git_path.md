---
comments: true
tags:
  - git
---

# Disambiguity path in git command

The `--` tells Git: "Everything after this is a file/directory path, not a branch name or option."

Without it git can get confused when a branch or tag name is same as filename:

```bash
# Imagine you have both:
# - A branch named "foo"
# - A file named "foo" in your project

git checkout foo foo  # ERROR: Which "foo" is which?
```

With `--`, it is clear:

```bash
git checkout foo -- foo
#              ↑      ↑
#          branch name file name
```
