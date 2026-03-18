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

### Navigation

| Command | Description            | Command | Description            | Command | Description            |
| ------- | ---------------------- | ------- | ---------------------- | ------- | ---------------------- |
| **w W** | beginning of next word | **b B** | beginning of last word | **e E** | end of current word    |
| **0**   | line start             | **^**   | first non-whitespace   | **$**   | end of line            |
| **{**   | previous blank line    | **}**   | next blank line        | **12G** | go to line 12          |
| **gg**  | start of file          | **G**   | end of file            | **%**   | jump to matching brace |
| **C-d** | down half screen       | **C-u** | up half screen         | **12G** | go to line 12          |

### Editing

| Command           | Description           | Command | Description           | Command | Description             |
| ----------------- | --------------------- | ------- | --------------------- | ------- | ----------------------- |
| **u**             | undo                  | **^r**  | redo                  | **.**   | repeat last editing cmd |
| **i I**           | insert text           | **a A** | append text           | **o O** | Open new line           |
| **i I**           | insert text           | **a A** | append text           | **o O** | Open new line           |
| **c<navigation>** | change text of motion | **C**   | change to end of line | **cc**  | change whole line       |
| **d<navigation>** | delete text of motion | **D**   | delete to end of line | **dd**  | delete(cut) whole line  |
| **y**             | copy                  | **yy**  | copy line             | **p P** | paste                   |

### Blocks

| Command | Description         | Command | Description                | Command | Description         |
| ------- | ------------------- | ------- | -------------------------- | ------- | ------------------- |
| **v**   | visual block stream | **V**   | visual block line          | **C-v** | visual block column |
| **> <** | indent block        | **gv**  | reselect last visual block |         |                     |

### Searching

| Command | Description             | Command | Description | Command | Description    |
| ------- | ----------------------- | ------- | ----------- | ------- | -------------- |
| **/ ?** | search forward/backward | **n**   | next match  | **N**   | previous match |

### Files

| Command       | Description           | Command  | Description         | Command   | Description               |
| ------------- | --------------------- | -------- | ------------------- | --------- | ------------------------- |
| **:w <name>** | write file            | **ZZ**   | write file and quit | **:n(!)** | new file (without saving) |
| **:q (!)**    | quit (without saving) | **:e .** | directory explorer  |           |                           |

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

## Dev-tookit
