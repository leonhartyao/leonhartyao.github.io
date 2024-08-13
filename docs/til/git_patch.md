---
comments: true
tags:
  - git
---

# Create and Apply Patches with Git

## git diff & git apply

### Creat Patches

* Make your changes in the git repository
* 'git add .'
* `git diff --cached > patch_file_name.patch`

### Apply Patches

```bash
cd your_git_repo
git apply --check path_to_patch_file.patch
git apply path_to_patch_file.patch
```

### Note
It is a good practice to check before apply patches.

Unlike `git merge`, if there are conflicts when applying a patch with git apply, the process might just fail without any further help.
You'd have to manually resolve conflicts and apply the patch again.

## git am

`git am` applies patches generated from commits. It also creates a commit with the metadata (author, date, commit message) from the patch.
It provides more sophisticated conflict handling and resolution capabilities, similar to those encountered during a regular merge or rebase.
you can easily undo the applied patches with git reset or git reflog, as it automatically creates commits.

### Create Patches

```bash
git format-patch -1 HEAD
```
This command generates a patch file for the most recent commit (HEAD).
The -1 indicates that you want to create a patch for one commit.
This will create a file named by the commit like 0001-Commit-message.patch.

### Apply Patches

```bash
cd your_git_repo
git am 0001-Commit-message.patch
```

This command will apply the patch and automatically create a new commit in your repository with the same commit message, author, and date as the original commit.
If there are any conflicts while applying the patch, `git am` will pause and let you resolve them.
The process is similar to resolving conflicts during a merge or rebase.

After resolving the conflicts, you would use `git am --continue` to continue applying pathces.
you can abort the process with `git am --abort`.
