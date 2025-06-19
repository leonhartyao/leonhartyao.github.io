---
title: Git Large File Storage (LFS)
description: Handling large files for git repositories.
comments: true
tags:
  - git
---

## What is Git LFS

Git Large File Storage (LFS) is a open souce git extension, which replaces large and binary files such as audio samples, videos, datasets, and graphics with text pointers inside git. And it stores the file content on a remote server, outside the git protocol (which is not designed for large files).

Per default remote servers like Github or Gitlab have file size limit at several MegaBytes while the maximum file size in Git LFS is 5GB.
If you regularly push large or binary files, you should consider introducing Git LFS as part of your workflow.
In best case, enable LFS while creating the git repository.

## Install Git LFS

Download and run the installation script.

```bash
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
```
Set up Git LFS for your user account by running the command below.. You only need to run this once per user account.

```bash
git lfs install
```

!!! note

    The settings will be saved in the file `.gitattributes`.

## Track Large Files

In each Git repositiry, configure Git LFS to manage a specific file type (e.g., .mov and .bin). Alternatively, you can directly edit your `.gitattributes`.

```bash
git lfs track *.mov
git lfs track "*.bin"
```

Configure Git LFS to track only files in a specific directory:

```bash
git lfs track 'images/'
```

Check which file types are currently under Git LFS:

```bash
git lfs track
```

!!! note

    Note that defining the file types to be tracked will not, by itself, convert any pre-existing files to Git LFS, such as files on other branches or in your prior commit history. Only from that point in time when you added it, the files will be stored to the external storage.
    To do that, use the [`git lfs migrate`](https://github.com/git-lfs/git-lfs/blob/main/docs/man/git-lfs-migrate.adoc?utm_source=gitlfs_site&utm_medium=doc_man_migrate_link&utm_campaign=gitlfs) command, which has a range of options designed to suit various potential use cases.

Make sure the config file is tracked.

```bash
git add .gitattributes
```

You can then commit and push to remote git repositories as normal, e.g.,

```bash
git add example.mov
git commit -m "Add video file."
git push origin develop
```

## Untrack Large Files

To remove files from tracking:

```bash
git lfs untrack '<file-type>'
git rm --cached '<file-type>'
git add '<file-type>'
git commit -m "restore '<file-type>' to git from lfs"
```

## Migrate Existing Non-LFS Repositories

As the way to migrate depends on the version of your git lfs, it’s described very detailed here.

Create a bare clone of the repository

```bash
git clone --bare https://<old_repository>.git
```

Pull in the repository’s Git Large File Storage objects

```bash
cd old_repository.git && git lfs fetch --all
```

Mirror-push to the new repository

```bash
git push --mirror https://<new-repository>.git +
# If you have files which are larger than 50 MB, you will get an error if you have files which exceed file size limit.
```

Push the repository’s Git LFS objects to your mirror

```bash
git lfs push --all https://<new-repository>.git
```

Remove the temporary local repository you created earlier

```bash
cd .. && rm -rf <old-repository>.git
```
