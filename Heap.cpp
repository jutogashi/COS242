#include "stdafx.h"
#include "Heap.h"
#include <iostream>

using namespace std;

	Heap::Heap() {
		heap = _empty();
	}

	Heap::~Heap() {
		if (heap) {
			_deleteAll(heap);
		}

	}

	node* Heap::insert(int value, int vertice) {
		node* ret = _singleton(value,vertice);
		ret->Vertice = vertice;
		heap = _merge(heap, ret);
		return ret;
	}

	void Heap::merge(Heap& other) {
		heap = _merge(heap, other.heap);
		other.heap = _empty();
	}

	bool Heap::isEmpty() {
		return heap == NULL;
	}

	node* Heap::getMinimum() {
		return heap;
	}

	int Heap::removeMinimum() {
		node* old = heap;
		heap = _removeMinimum(heap);
		int ret = old->Value;
		delete old;
		return ret;
	}

	void Heap::decreaseKey(node* n, int value) {
		heap = _decreaseKey(heap, n, value);
	}

	node* Heap::find(int value) {
		return _find(heap, value);
	}

	node* Heap::_empty() {
		return NULL;
	}
	node* Heap::_singleton(int value, int vertice) {
		node* n = new node;
		n->Vertice = vertice;
		n->Value = value;
		n->prev = n->next = n;
		n->degree = 0;
		n->marked = false;
		n->child = NULL;
		n->parent = NULL;
		return n;
	}

	node* Heap::_merge(node* a, node* b) {
		if (a == NULL)return b;
		if (b == NULL)return a;
		if (a->Value>b->Value) {
			node* temp = a;
			a = b;
			b = temp;
		}
		node* an = a->next;
		node* bp = b->prev;
		a->next = b;
		b->prev = a;
		an->prev = bp;
		bp->next = an;
		return a;
	}

	void Heap::_deleteAll(node* n) {
		if (n != NULL) {
			node* c = n;
			do {
				node* d = c;
				c = c->next;
				_deleteAll(d->child);
				delete d;
			} while (c != n);
		}
	}
	void Heap::_addChild(node* parent, node* child) {
		child->prev = child->next = child;
		child->parent = parent;
		parent->degree++;
		parent->child = _merge(parent->child, child);
	}

	void Heap::_unMarkAndUnParentAll(node* n) {
		if (n == NULL)return;
		node* c = n;
		do {
			c->marked = false;
			c->parent = NULL;
			c = c->next;
		} while (c != n);
	}

	node* Heap::_removeMinimum(node* n) {
		_unMarkAndUnParentAll(n->child);
		if (n->next == n) {
			n = n->child;
		}
		else {
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n = _merge(n->next, n->child);
		}
		if (n == NULL)return n;
		node* trees[64] = { NULL };

		while (true) {
			if (trees[n->degree] != NULL) {
				node* t = trees[n->degree];
				if (t == n)break;
				trees[n->degree] = NULL;
				if (n->Value<t->Value) {
					t->prev->next = t->next;
					t->next->prev = t->prev;
					_addChild(n, t);
				}
				else {
					t->prev->next = t->next;
					t->next->prev = t->prev;
					if (n->next == n) {
						t->next = t->prev = t;
						_addChild(t, n);
						n = t;
					}
					else {
						n->prev->next = t;
						n->next->prev = t;
						t->next = n->next;
						t->prev = n->prev;
						_addChild(t, n);
						n = t;
					}
				}
				continue;
			}
			else {
				trees[n->degree] = n;
			}
			n = n->next;
		}
		node* min = n;
		node* start = n;
		do {
			if (n->Value<min->Value)min = n;
			n = n->next;
		} while (n != start);
		return min;
	}

	node* Heap::_cut(node* heap, node* n) {
		if (n->next == n) {
			n->parent->child = NULL;
		}
		else {
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n->parent->child = n->next;
		}
		n->next = n->prev = n;
		n->marked = false;
		return _merge(heap, n);
	}


	node* Heap::_decreaseKey(node* heap, node* n, int value) {
		if (n->Value<value)return heap;
		n->Value = value;
		if (n->parent) {
			if (n->Value<n->parent->Value) {
				heap = _cut(heap, n);
				node* parent = n->parent;
				n->parent = NULL;
				while (parent != NULL && parent->marked) {
					heap = _cut(heap, parent);
					n = parent;
					parent = n->parent;
					n->parent = NULL;
				}
				if (parent != NULL && parent->parent != NULL)parent->marked = true;
			}
		}
		else {
			if (n->Value < heap->Value) {
				heap = n;
			}
		}
		return heap;
	}

	node* Heap::_find(node* heap, int value) {
		node* n = heap;
		if (n == NULL)return NULL;
		do {
			if (n->Value == value)return n;
			node* ret = _find(n->child, value);
			if (ret)return ret;
			n = n->next;
		} while (n != heap);
		return NULL;
	}
