---
title: Vim, Tmux, VS Code and Vimium Cheatsheet
description: Quick reference for keyboard shortcuts and commands in Vim, Tmux, VS Code, and Vimium.
comments: true
tags:
  - vim
  - tmux
  - vimium
  - Dev-toolkit
---

## vim

### Movement

| Key         | Description         | Key       | Description      |
| ----------- | ------------------- | --------- | ---------------- |
| **h j k l** | character/direction | **w b e** | words            |
| **0 ^ $**   | line                | **{ }**   | paragraph        |
| **C-d C-u** | half screen         | **gg G**  | file             |
| **f t**     | character in line   | **\%**    | matching bracket |
| **\/**      | search              | **\***    | search word      |
| **n**       | next match          | **N**     | previous match   |

### Operators & Text Objects

| Key     | Description         | Key     | Description              | Key     | Description   |
| ------  | --------------------| ------- | ------------------------ | ------- | ------------- |
| **d**   | delete              | **c**   | change                   | **y**   | yank (copy)   |
| **iw**  | inside word         | **i"**  | inside quotes            | **i\{** | inside braces |
| **i\(** | inside parentheses  | **"+y** | change registry and yank |         |               |

### Editing

| Key         | Description           | Key     | Description           | Key          | Description             |
| ----------- | --------------------- | ------- | --------------------- | ------------ | ----------------------- |
| **i a I A** | insert \| append text | **a A** | append text           | **o O**      | Open new line           |
| **x**       | delete one character  | **r**   | replace one character | **~**        | toggle case             |
| **dd**      | delete/cut line       | **yy**  | copy line             | **c**        | change line             |
| **J**       | join lines            | **p P** | paste                 | **>> << ==** | indent block            |
| **u**       | undo                  | **^r**  | redo                  | **.**        | repeat last editing cmd |


### Selection

| Key     | Description         | Key     |  Description    | Key    | Description     |
| ------- | ------------------- | ------- | -------------- | ------- | --------------- |
| **v**   | character selection | **V**   | line selection | **C-v** | block selection |

### Files

| Key           | Description           | Key        | Description           | Key                   | Description               |
| ------------- | --------------------- | ---------- | --------------------- | --------------------- | ------------------------- |
| **:w <name>** | write file            | **:q (!)** | quit (without saving) | **:n(!)**             | new file (without saving) |
| **:q (!)**    | quit (without saving) | **:E**     | explorer              | **:e \<path\|file\>** | edit                      |
| **:ls**       | show buffer           | **:bn**    | next buffer           | **:bp**               | next buffer               |

### Windows

| Key         | Description      | Key        | Description   | Key                | Description   |
| ----------- | ---------------- | ---------- | ------------- | ------------------ | ------------- |
| **:vsplit** | left-right split | **:split** | up-down split | **Ctrl-w h j k l** | switch window |

## tmux

| Command  | Description   | Command    | Description              | Command   | Description                     |
| -------- | ------------- | ---------- | ------------------------ | --------- | ------------------------------- |
| **/**    | explain key   | **q**      | quit mode                | **s**     | choose session                  |
| **- \|** | split         | **!**      | break pane to new window | **space** | next layout                     |
| **x**    | close         | **arrows** | move to pane             | **q1**    | show pane index and switch to 1 |
| **,**    | name window   | **1**      | window 1                 | **c**     | new window                      |
| **w**    | chosse window | **t**      | clock                    | **z**     | toggle zoom                     |
| **[**    | copy mode     | **]**      | paste last               | **?**     | help                            |
| **g G**  | top/bottom    | **]**      | paste last               | **/ ?**   | search                          |

## VS Code

| Command     | Description       | Command   | Description  | Command | Description | Command    | Description        |
| ----------- | ----------------- | --------- | ------------ | ------- | ----------- | ---------- | ------------------ |
| **C-Enter** | open splitttly    | **M-o**   | toggle h/cpp | **C-g** | goto line   | **C-M-\\** | toggle {}          |
| **C-k v**   | open preview side | **C-S-v** | open preview | **C-w** | close tab   | **C-k o**  | copy to new window |
| **M-h**     | git file history  | **C-\\**  | split right  | **C-w** | close tab   | **C-k o**  | copy to new window |

## vimium

| Command | Description       | Command | Description   | Command | Description         | Command  | Description       |
| ------- | ----------------- | ------- | ------------- | ------- | ----------------- | -------- | ------------------- |
| **h**   | left              | **j**   | down          | **k**   | up                | **l**    | right               |
| **d**   | half page down    | **u**   | half page up  | **H**   | back in history   | **L**    | forward in history  |
| **r**   | reload page       | **yy**  | copy url      | **p P** | open copied url   | **f F**  | open link           |
| **o O** | open Vomnibar     | **b B** | open book mark| **T**   | search opened tab | **ge gE**| edit url            |
| **/**   | search on page    | **n**   | search forward| **N**   | search backward   | **gi**   | focus 1st text input|
| **t**   | new tab           | **gT**  | left tab      | **gt**  | right tab         | **g0 g$**| first/last tab      |
| **yt**  | duplicate tab     | **x**   | close tab      | **X**  | Restore closed tab| **?**    | help                |
