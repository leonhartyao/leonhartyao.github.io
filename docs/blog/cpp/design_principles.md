---
title: OOP Design Principles and Patterns
description: Eight design principles and six design patterns for software development.
comments: true
tags:
  - C++
---

## Design Principles

### Single Responsibility Principle (SRP)

[Robert C. Martin](https://en.wikipedia.org/wiki/Robert_C._Martin) defines a responsibility as a reason to change, and concludes that a class or module should have one, and only one, reason to be changed, , i.e., every class should have only one responsibility.

### Open–Closed Principle (OCP)

software entities (classes, modules, functions, etc.) should be open for extension, but closed for modification.

The implementations can be changed and multiple implementations could be created and polymorphically substituted for each other. It advocates inheritance from abstract base classes. Interface specifications can be reused through inheritance but implementation need not be. The existing interface is closed to modifications and new implementations must, at a minimum, implement that interface.

### Liskov Substitution Principle (LSP)

An object (such as a class) may be replaced by a sub-object (such as a class that extends the first class) without breaking the program. In other words, functions that use pointers or references to base classes must be able to use objects of derived classes without knowing it.

### Interface Segregation Principle (ISP)

Clients should not be forced to depend upon interfaces that they do not use. ISP splits interfaces that are very large into smaller and more specific ones so that clients will only have to know about the methods that are of interest to them.

###  Dependency Inversion Principle (DIP)

High-level modules should not import anything from low-level modules. Both should depend on abstractions (e.g., interfaces). Abstractions should not depend on details. Details (concrete implementations) should depend on abstractions.

An example could be a supplier of autonomous driving system and several automobile companies using this system. When they depend on each other, the change on any of the both sides requires adaption on the other side. In contrast, when both sides depend on a automonous driving standard, they are free to have their concrete implementations.

### Encapsulate What Changes

what stays the same is isolated from what changes often. It reduces the impact of frequently changing code by encapsulating it. The encapsulated code can then change independently to code that relies on it.

### Composite Reuse Principle (CRP)

CRP means to use object composition (has-a)/aggregation instead of inheritance (contanis-a) relationship to achieve the purpose of software reuse. It can make the system more flexible and reduce the coupling between classes. The change of one class has relatively little impact on other classes.

Inheritance is called white box reuse, which is equivalent to exposing all implementation details to subclasses. Composition/aggregation is called black box reuse, and we cannot obtain the implementation details of objects outside the class.
