#include "SupClass.h"
#include <cmath>
Reg_FBH_Node::Reg_FBH_Node(){
    
}
/*
Reg_FBH_Node(Registration*) initiate a heap node from a Registration pointer 
*/
Reg_FBH_Node::Reg_FBH_Node(Record* p){
    this->p = p;
    this->degree = 0;
    this->marked = false;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->child = nullptr;
}
FBH::FBH(bool (*pr) (const Record* , const Record* )){
    count = 0;
    Priority_Rule = pr;
    max = nullptr;
}
/*
Max() returns the registration with maximum priority in the heap. Return nullptr when the heap is empty.
*/
Record* FBH::Max() const{
    if (this->count == 0) return nullptr;
    return max->p;
}
/*
Dequeue() returns the registration with maximum priority in the heap and remove it. Return nullptr when the heap is empty
*/
Record* FBH::Dequeue() {
    if (this->count == 0) return nullptr;
    Record* rtv = this->max->p;
    Reg_FBH_Node* tpm = max;
    if (max->left == max){      //if max has no neighbor, set max to be its child.
        max = tpm->child;
    }else{
        if (max->degree == 0){
            max = max->right;
            tpm->right->left = tpm->left;
            tpm->left->right = tpm->right;  //remove the max node from the root list
            if (this->count == 1) max = nullptr;
        }else{
            max = max->right;
            Reg_FBH_Node* cl = tpm->child;
            Reg_FBH_Node* cr = tpm->child->left;
            cl->left = tpm->left;
            tpm->left->right = cl;
            cr->right = tpm->right;
            tpm->right->left = cr;  //link the child nodes onto the root list
        }
    }
    Consolidate();
    this->count --; //reduce the count
    delete tpm;     //remove the node
    return rtv;
}
/*
Enqueue(Record*) Put a new registration into the heap, return the pointer to the node
*/
Reg_FBH_Node* FBH::Enqueue(Record* toeq){
    Reg_FBH_Node* rtn = new Reg_FBH_Node(toeq);
    if (this -> count == 0) {       //Add the node into the root list
        rtn ->left = rtn;
        rtn ->right = rtn;
        max = rtn;
        this -> count++;
        return rtn;     
    }
    rtn->right = max->right;
    rtn->left = max;
    max->right->left = rtn;
    max->right = rtn;
    if (this->PriorThan(rtn,max)) max = rtn;     //Update maximum
    this->count ++;
    return rtn;
}
/*
Consolidate() reorganize the heap
*/
void FBH::Consolidate(){
    if (this->max == nullptr) return;
    const int maxdegree = (int)(log(count)/logp);
    Reg_FBH_Node** degc = new Reg_FBH_Node*[maxdegree+1]();
    Reg_FBH_Node* endnode = max->left;
    bool flag = true;
    Reg_FBH_Node* iter = max;
    do {
        Reg_FBH_Node* tpnode = iter;
        if (tpnode == endnode) flag = false; //The last node is found, this is the last node.
        iter = iter->right;
        while(degc[tpnode->degree] != nullptr) {
            Reg_FBH_Node* ap = degc[tpnode->degree];
            degc[tpnode->degree] = nullptr;
            if (this->PriorThan(tpnode, ap)) {
                Reg_FBH_Node* swap_node = ap;
                ap = tpnode;
                tpnode = swap_node;
            }
            tpnode->left->right = tpnode->right;
            tpnode->right->left = tpnode->left; //remove the smaller node from the node list.
            tpnode->marked = false;
            if (ap->degree == 0){
                ap->child = tpnode;
                tpnode->parent = ap;    //create parent-child connection
                tpnode->left = tpnode;
                tpnode->right = tpnode;    //create sibling connection
                ap->degree ++;
            }else{
                tpnode->parent = ap;    //create parent-child connection
                tpnode->left = ap->child;
                tpnode->right = ap->child->right;   
                ap->child->right->left = tpnode;
                ap->child->right = tpnode;  //create sibling connection
                ap->degree++;
            }
            tpnode = ap;
            max = ap;
        }
        degc[tpnode->degree] = tpnode;
    } while(flag);
    for (int i = 0; i <= maxdegree; i++) {
        if (degc[i] == nullptr) continue;
        degc[i]->parent = nullptr;  //set the parent of the nodes in the root list to be null
        if (this->PriorThan(degc[i],max)) max = degc[i]; //find the new maximum
    }
    delete[] degc;
}
/*
UpdateNode(Reg_FBH_Node*): Update the priority of the node
*/
void FBH::UpdateNode(Reg_FBH_Node* handle) {
    if (handle->parent != nullptr){
        if (this -> PriorThan(handle, handle -> parent)){ //Update only when the priority increases.
            Reg_FBH_Node* ph = handle->parent;
            Cut(handle);
            Conscut(ph);
            //increase_key
        }
    }
    if (this->PriorThan(handle,max)) max = handle;
    return;
}
/*
Delete() remove the node from the heap
*/
Record* FBH::Delete(Reg_FBH_Node* todel) {
    todel->deleted = true;
    UpdateNode(todel);
    return Dequeue();
}
void FBH::Cut(Reg_FBH_Node* p){
    p->parent->degree--;
    p->marked = false;
    p->parent = nullptr;
    p->left = max;
    p->right = max->right;
    max->right->left = p;
    max->right = p;
}
/*
Conscut(): Perform cascade cutting in FBH
*/
void FBH::Conscut(Reg_FBH_Node* p){
    if (p == nullptr) return;
    if (p->parent == nullptr) return;
    if (p->marked == false) {
        p->marked = true;
        return;
    }else{
        Reg_FBH_Node* ph = p->parent;
        Cut(p);
        Conscut(ph);
    }
}
///PriorThan() decides the priority of two queueing nodes.
bool FBH::PriorThan(const Reg_FBH_Node *a, const Reg_FBH_Node *b) const{
    if (a->deleted) return true;
    if (b->deleted) return false;    //delete check:Highest priority;
    return Priority_Rule(a->p, b->p);
}
/*
isEmpty(): Return true when the 
*/
bool FBH::isEmpty() {
    return (count == 0);
}