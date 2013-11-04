/*
  calculator_stack.h - 'Library' for a quick character stack to use with my calculator sketch
  Created by Don van der Drift, last updated: November 3, 2013.
  Released into the public domain.
*/

#ifndef _CALCULATOR_STACK_H
#define _CALCULATOR_STACK_H

#include <Arduino.h>

enum Type {OPPERATOR, VALUE, EQUALS, CLEAR, NONE};

class Stack {
  public:
    Stack ();
    ~Stack ();
    void push (char);
    byte get_position ();
    long top_value;
    void collapse (byte, long);
    void clear ();

    byte position;
    long values[5];
    byte types[5];
};

Stack::Stack () {
  clear();
}

Stack::~Stack () {
  clear();
}

byte Stack::get_position () {
  return position;
}

void Stack::clear () {
  for (int i = 0; i < 5; i++) {
    values[i] = 0;
    types[i] = NONE;
  }
  types[0] = VALUE;
  position = 0;
}

void Stack::push (char ch) {
  long value = 0;
  byte type = NONE;

  switch (ch) {
    case '=': 
      type = EQUALS; value = 0; break;
    case '*': 
      type = OPPERATOR; value = 0; break;
    case '/': 
      type = OPPERATOR; value = 1; break;
    case '+': 
      type = OPPERATOR; value = 2; break;
    case '-': 
      type = OPPERATOR; value = 3; break;
    case 'C': 
      type = CLEAR; value = 0; break;
    case '0': 
      type = VALUE; value = 0; break;
    case '1': 
      type = VALUE; value = 1; break;
    case '2': 
      type = VALUE; value = 2; break;
    case '3': 
      type = VALUE; value = 3; break;
    case '4': 
      type = VALUE; value = 4; break;
    case '5': 
      type = VALUE; value = 5; break;
    case '6': 
      type = VALUE; value = 6; break;
    case '7': 
      type = VALUE; value = 7; break;
    case '8': 
      type = VALUE; value = 8; break;
    case '9': 
      type = VALUE; value = 9; break;
    default:
      type = NONE; value = 0;
  }

  if ((types[position] == VALUE) && (type == VALUE)) {
    values[position] = values[position] * 10 + value;
    if (values[position] >= 100000000) {
      values[position] = 88888888;
    }
  } else if ((types[position] == OPPERATOR) && (type == OPPERATOR)) {
    if ((position == 1) || ((position == 3) && (value <= 1))) {
      values[position] = value;
    } else {
      collapse(type, value);
    }
  } else if (type == EQUALS) {
    collapse(type, value);
  } else if (type == CLEAR) {
    clear();
  } else if ((types[position] == OPPERATOR) && (type == VALUE)) {
    position++; 
    values[position] = value;
    types[position] = type;
  } else if ((types[position] == VALUE) && (type == OPPERATOR)) {
    if ((position == 1) || ((position == 2) && (values[1] >= 2) && (value <= 1))) {
      position++; 
      values[position] = value;
      types[position] = type;   
    } else {
      collapse(type, value);
      position++; 
      values[position] = value;
      types[position] = type; 
    }
  }
  int i = 4;
  while ((types[i] != VALUE) && (i >= 0)) {
    i -= 1;
  }
  top_value = values[i];
}

void Stack::collapse (byte type, long value) {
  if (type == EQUALS) {
    if ((position == 1) || (position == 3)) {
        values[position] = 0;
        types[position] = NONE;
        position--;
    }
    if (position == 2) {
      long a = values[0];
      long b = values[2];
      switch (values[1]) {
        case 0:
          a *= b; break;
        case 1:
          a /= b; break;
        case 2:
          a += b; break;
        case 3:
          a -= b; break;
      }
      clear();
      values[0] = a;
      types[0] = VALUE;
    } else if (position == 4) {
      long a = values[0];
      long b = values[2];
      long c = values[4];
      if (values[3] == 0) {
        b *= c;
      } else {
        b /= c;
      }
      if (values[1] == 2) {
        a += b;
      } else {
        a -= b;
      }
      clear();
      values[0] = a;
      types[0] = VALUE;
    }
  } else if (type == OPPERATOR) {
    long a = values[0];
    long b = values[2];
    long c = values[4];
    if (position == 2) {
      switch (values[1]) {
        case 0:
          a *= b; break;
        case 1:
          a /= b; break;
        case 2:
          a += b; break;
        case 3:
          a -= b; break;
      }
      clear();
      values[0] = a;
    } else if (position == 4) {
      if (values[3] == 0) {
        b *= c;
      } else {
        b /= c;
      }
      if (value >= 2) {
        if (values[1] == 2) {
          a += b;
        } else {
          a -= b;
        }
        clear();
        values[0] = a;
        types[0] = VALUE;
      } else {
        values[2] = b;
        values[3] = 0;
        values[4] = 0;
        types[3] = NONE;
        types[4] = NONE;
        position = 2;
      }
    }
  }
}

#endif // _CALCULATOR_STACK_H