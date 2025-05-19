---
title: Multi-Arch Docker Push
description: How to push docker images for multiple architectures to the same tag.
comments: true
tags:
  - Docker
---

## Multi-Arch From One Host

Building and pushing docker images for different architectures are simple from one host with QEMU setup.
You can do it with docker buildx command:

```bash
docker buildx build --ssh default --platform linux/amd64,linux/arm64 -t org/repo:tag -f Dockerfile --push --load .
```

Above command will build `Dockerfile` for `amd64` and `arm64`, push them to `org/repo:tag` and make the image available locally.

### Multi-Arch From Different Machines

We have to create a manifest list manually.

#### Build and Push images

Images built on different machines have to be pushed with different tags:

```bash
docker build -f Dockerfile -t org/repo:tag-amd64 .
docker push org/repo:tag-amd64
```

On the other arm machine:

```bash
docker build -f Dockerfile -t org/repo:tag-arm64 .
docker push org/repo:tag-arm64
```

#### Create and Push Multi-Arch Manifest

The multi-arch manifest can be create on any machine:

```bash
docker manifest create org/repo:tag \
  --amend org/repo:tag-amd64 \
  --amend org/repo:tag-arm64
```

Push the manifest to docker hub.

```bash
docker manifest push org/repo:tag
```

This command creates a manifest list that points to the correct image depending on the architecture of the pulling machine.
