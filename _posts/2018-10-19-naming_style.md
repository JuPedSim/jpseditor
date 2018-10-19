---
layout: post
date: '2018-10-19'
title: Naming Convention for development of JPSeditor
permalink: 2018-10-19-naming_style.html
published: true 
---

This Naming convention is referred to followed documents:

* [Introduction to Design Patterns in C++ with Qt](https://www.ics.com/designpatterns/solutions/style.html)
* [Designing Qt-Style C++ APIs](https://doc.qt.io/archives/qq/qq13-apis.html#theartofnaming)

## General Naming Rules

* Names are sequences of letters and numerical digits, the first of which must not be a digit;
* Don't use abbreviations;
* Variables and pointers use only lowercase letters.

## Class

* The name of classes should a noun or a noun phrase;
* Use CamelCase to form multi-word names by joining the words together and capitalizing the interior word beginnings;
* Class names begin with a capital letter.

Examples: `class LargeFurryMammal`, `class Customer`.

## Class members

* Data members: `m_Color`, `m_Width` - prepend lowercase `m_`;
* Static data members: `s_Singleton`, `s_ObjCount` - prepend lowercase `s_`.

## Function

* Each function name should be a verb or a verb phrase: `processBookOrder()`;
* Function names begin with a lowercase letter.

# Getter and setter

* Non-boolean getters: `color()` or `getColor()`;
* Boolean getters: `isChecked()` or `isValid()`;
* setter: `setColor(const Color& newColor)`.


<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.3.1/css/all.css" integrity="sha384-mzrmE5qonljUremFsqc01SB46JvROS7bZs3IO2EmfFsd15uHvIt+Y8vEf7N7fWAU" crossorigin="anonymous">

If you have questions about [JPSeditor](http://www.jupedsim.org/jpseditor/), please contact <t.zhong@fz-juelich.de>.