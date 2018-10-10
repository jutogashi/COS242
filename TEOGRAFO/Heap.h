
#ifndef Heap_h
#define Heap_h


#endif /* Heap_h */

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
    float Value;
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
    node* insert(float value,int vertice);
    void merge(Heap& other);
    bool isEmpty();
    node* getMinimum();
    int removeMinimum();
    node* decreaseKey(node* n, float value);
    node* find(float value);
private:
    node * _empty();
    node* _singleton(float value, int vertice);
    node* _merge(node* a, node* b);
    void _deleteAll(node* n);
    void _addChild(node* parent, node* child);
    void _unMarkAndUnParentAll(node* n);
    node* _removeMinimum(node* n);
    node* _cut(node* heap, node* n);
    node* _decreaseKey(node* heap, node* n, float value);
    node* _find(node* heap, float value);
};
