#pragma once
#include <tuple>
using namespace std;

struct node {
private:
	node* prev;
	node* next;
	node* child;
	node* parent;
	int degree;
	bool marked;
	
public:
	int Value;
	int Vertice;
	friend class Heap;
	node* getPrev() { return prev; }
	node* getNext() { return next; }
	node* getChild() { return child; }
	node* getParent() { return parent; }
	int getValue() { return Value; }
	bool isMarked() { return marked; }
	bool hasChildren() { return child; }
	bool hasParent() { return parent; }
};

class Heap
{
protected:
	node* heap;
public:
	Heap();
	virtual ~Heap();
	node* insert(int value,int vertice);
	void merge(Heap& other);
	bool isEmpty();
	node* getMinimum();
	int removeMinimum();
	node* decreaseKey(node* n, int value);
	node* find(int value);
private:
	node * _empty();
	node* _singleton(int value, int vertice);
	node* _merge(node* a, node* b);
	void _deleteAll(node* n);
	void _addChild(node* parent, node* child);
	void _unMarkAndUnParentAll(node* n);
	node* _removeMinimum(node* n);
	node* _cut(node* heap, node* n);
	node* _decreaseKey(node* heap, node* n, int value);
	node* _find(node* heap, int value);
};