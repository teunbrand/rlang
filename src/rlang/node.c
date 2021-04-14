#include "rlang.h"


// Shallow copy of a node tree
r_obj* r_node_tree_clone(r_obj* x) {
  if (r_typeof(x) != R_TYPE_pairlist) {
    r_abort("Internal error: Expected node tree for shallow copy");
  }

  x = KEEP(r_clone(x));

  r_obj* rest = x;
  while (rest != r_null) {
    r_obj* head = r_node_car(rest);
    if (r_typeof(head) == R_TYPE_pairlist) {
      r_node_poke_car(rest, r_node_tree_clone(head));
    }
    rest = r_node_cdr(rest);
  }

  FREE(1);
  return x;
}

r_obj* r_pairlist_find(r_obj* node, r_obj* tag) {
  while (node != r_null) {
    if (r_node_tag(node) == tag) {
      return node;
    }
    node = r_node_cdr(node);
  }

  return r_null;
}

r_obj* r_pairlist_rev(r_obj* node) {
  if (node == r_null) {
    return node;
  }

  r_obj* prev = r_null;
  r_obj* tail = node;
  r_obj* next;
  while (tail != r_null) {
    next = r_node_cdr(tail);
    r_node_poke_cdr(tail, prev);
    prev = tail;
    tail = next;
  }

  return prev;
}
