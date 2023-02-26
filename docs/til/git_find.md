---
comments: true
tags:
  - git
---

# Search Git Log by Message

To find commit by message, I add a alias in `~/.gitconfig`:

```bash
[alias]
    find = log --all --pretty=\"format:%Cgreen%H %Cblue%s\n%b%Creset\" --name-status --grep
```

Example

```bash
git find concurrency

6a786f09b9d56b98df9379e587ce2f8d7428d386 Update concurrency.md

M       docs/blog/cpp/concurrency.md

0381f01063e42b19319cc41ffff9dfe9345dd744 Fix tag of concurrency.md

M       docs/blog/cpp/concurrency.md

48359253b4f101445d3f0ef0632b8c48e64fde5e Merge pull request #6 from leonhartyao/concurrency
Update blog.

```
