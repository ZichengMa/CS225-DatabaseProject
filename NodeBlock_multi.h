#ifndef NBLOCK_MULTI
#define NBLOCK_MULTI
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
#define m_multi 2   // Node's order
#define MAXNUM_KEY_MULTI (m_multi*2)
#define MAXNUM_POINTER_MULTI (MAXNUM_KEY_MULTI+1)
#define MAX_OVERFLOW_SIZE_MULTI 2
#define MAX_MAINBLOCK_SIZE_MULTI 2
// #define INVALID_MULTI 0
#define FLAG_LEFT_MULTI 1
#define FLAG_RIGHT_MULTI 2
#include <utility>





//Directly realize all the required types here.
///BPlusNode_multi is the node that is used in BPlus_multi Tree
// K是key,T是存的data
enum NODE_TYPE_MULTI{
    ROOT_MULTI = 1,
    INTERNAL_MULTI = 2,
    LEAF_MULTI = 3,
};

/*
 Base class for all different kinds of node.
*/
template <class K, class T> class BPlusNode_multi {
public:
    BPlusNode_multi(){
        parent=NULL;
        key_num=0;
    }
    BPlusNode_multi(int (*Compare)(const K&, const K&));
    virtual ~BPlusNode_multi(){Delete_Children();}

    NODE_TYPE_MULTI Type(){return node_type;}
    void Set_Type(NODE_TYPE_MULTI type){node_type=type;}

    int Get_count(){return key_num;}  // 获取当前有效元素数
    void Set_count(int c){key_num=c;}



    virtual K Get_key(int i)=0; // 获取某一个位置上的key
    virtual void Set_key(int i,K key)=0;   // 设置某一个位置上的key


    virtual BPlusNode_multi<K,T>* Get_ptr(int i){
        return NULL;
    }// 获取某一个孩子的指针
    virtual void Set_ptr(int i, BPlusNode_multi<K,T>* pointer)=0;  // 设置某一个孩子的指针


    BPlusNode_multi<K,T>* Get_Father() { return parent;} //获取父亲结点
    void Set_Father( BPlusNode_multi<K,T>* f){parent=f;}

    BPlusNode_multi<K,T>* Get_sibling(int& flag); //获取最近的一个兄弟结点

    void Delete_Children();
    
protected:
    NODE_TYPE_MULTI node_type;

    int (*CompareTo) (const K& left, const K& right);


    int key_num;

    BPlusNode_multi<K,T>* parent;
};





template <class K, class T> class InternalNode_multi:public BPlusNode_multi<K,T>  {
public:
    InternalNode_multi(int (*Compare)(const K&, const K&));
    ~InternalNode_multi();


    K Get_key(int i){
        if ((i > 0 ) && (i <= MAXNUM_KEY_MULTI))
        {
            return keys[i - 1];
        }
        else
        {
            // return INVALID_MULTI;
            K rubbish;
            return rubbish;
        }
    }
    void Set_key(int i,K key)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY_MULTI))
        {
            keys[i - 1] = key;
        }
    }


    BPlusNode_multi<K,T>* Get_ptr(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER_MULTI))
        {
            return children[i - 1];
        }
        else
        {
            return NULL;
        }
    }
    void Set_ptr(int i, BPlusNode_multi<K,T>* pointer )
    {
        if ((i > 0 ) && (i <= MAXNUM_POINTER_MULTI))
        {
           children[i - 1] = pointer;
        }
    }

    // 在结点pNode上插入键value
    bool Insert(K value, BPlusNode_multi<K,T>* pNode);
    // 删除键value
    bool Delete(K value);

    // 分裂结点
    K Split(InternalNode_multi<K,T>* pNode, K key);
    // 结合结点(合并结点)
    bool Combine(BPlusNode_multi<K,T>* pNode);
    // 从另一结点移一个元素到本结点
    bool MoveOneElement(BPlusNode_multi<K,T>* pNode);
    
protected:

    vector<K> keys;  // vector for keys

    vector<BPlusNode_multi<K,T>*> children;  // vector for pointers
};



template <class K, class T> class LeafNode_multi:public BPlusNode_multi<K,T> {
public:

    LeafNode_multi(int (*Compare)(const K&, const K&));
    virtual ~LeafNode_multi();

    // 获取和设置数据
    K Get_key(int i)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY_MULTI))
        {
            return keys[i - 1];
        }
        else
        {
            // return INVALID_MULTI;
            K rubbish;
            return rubbish;
        }
    }

    void Set_key(int i, K key)
    {
        if ((i > 0 ) && (i <= MAXNUM_KEY_MULTI))
        {
            keys[i - 1] = key;
        }
    }

    void Set_ptr(int i, BPlusNode_multi<K,T>* pointer )
    {
        return;
    }
    BPlusNode_multi<K,T>* Get_ptr(int i){ // No more ptr in the leaf node
        return NULL;
    }

    vector<T*>* Get_Data(K key);

    
    // 在结点pNode上插入键value
    bool Insert(K key, vector<T*>* data);
    // 删除键value
    bool Delete(K key);

    // 分裂结点
    K Split(LeafNode_multi<K,T>* pNode);
    // 结合结点(合并结点)
    bool Combine(LeafNode_multi<K,T>* pNode);
    // 从另一结点移一个元素到本结点
    bool MoveOneElement(LeafNode_multi<K,T>* pNode);

public:
    // 以下两个变量用于实现双向链表
    LeafNode_multi<K,T>* m_pPrevNode;                 // 前一个结点
    LeafNode_multi<K,T>* m_pNextNode;                 // 后一个结点
   
public:
    static int empty(const K& left, const K& right) {
        return 0;
    }
    static int (*Compare_comp) (const K& left, const K& right);
    static bool comp(pair<K,vector<T*>*> &a, pair<K,vector<T*>*> &b){
        if(Compare_comp(a.first,b.first)==-1){
            return true;
        }else{
            return false;
        }
    }
    vector<K> keys;
    vector< pair<K,vector<T*>*> > main_block;
    vector< pair<K,vector<T*>*> > overflow_block;
    int overflow_num;
    int main_num;  // number of elements in different blocks
    int overflow_size; // the size of overflow block
    int main_size;  // the size of main block     ps: overflow_size + main_size = m_multi
};







// ----- Code for BPlusNode_multi基类-----
template<class K, class T> BPlusNode_multi<K,T>::BPlusNode_multi(int (*Compare)(const K&, const K&)){
    key_num=0;
    parent=NULL;
    CompareTo=Compare;
}



/*
    Find neaest empty sibling. Return the pointer of that node,
    and flag=1 means we find left sibling while flag=2 means we find righ sibling
*/

template<class K, class T> BPlusNode_multi<K,T>* BPlusNode_multi<K,T>::Get_sibling(int& flag)
{
    BPlusNode_multi<K,T>* pFather =(InternalNode_multi<K,T>*) Get_Father();   //Get parent  MODIFIED
    if (NULL == pFather)
    {
        return NULL;
    }

    BPlusNode_multi<K,T>* pBrother = NULL; // build ptr for sibling

    for (int i = 1; i <= pFather->Get_count() + 1; i++)   //get number of elements, which is 1 smaller than ptrs' number
    {
        // find current node's position
        if (pFather->Get_ptr(i) == this)
        {
            if (i == (pFather->Get_count() + 1))   // current node is the most right child of parent
            {
                pBrother = pFather->Get_ptr(i - 1);    // 本身是最后一个指针，只能找前一个指针
                flag = FLAG_LEFT_MULTI;  // 1 shows that this will return a  left sibling
            }
            else
            {
                pBrother = pFather->Get_ptr(i + 1);    // 优先找后一个指针
                flag = FLAG_RIGHT_MULTI;  // 2 shows that this will return a right sibling
            }
        }
    }

    return pBrother;
}

// 递归删除子结点
template<class K, class T> void BPlusNode_multi<K,T>::Delete_Children()   // 疑问：这里的指针下标是否需要从0开始
{

    for (int i = 1; i <=this->Get_count()+1; i++)   //MODIFIED Get_count()为返回结点中关键字即数据的个数
    {
        BPlusNode_multi<K,T>* pNode = this->Get_ptr(i);
        if (NULL != pNode)    // 叶子结点没有指针
        {
            pNode->Delete_Children();
        }

        delete pNode;
    }
}


// ----- Next part is for internal node-----
template<class K, class T> InternalNode_multi<K,T>::InternalNode_multi(int (*Compare)(const K&, const K&))
{
    this->node_type=INTERNAL_MULTI;
    this->CompareTo=Compare;
    keys=vector<K>(MAXNUM_KEY_MULTI) ;  // vector for keys

    children=vector<BPlusNode_multi<K,T>*>(MAXNUM_POINTER_MULTI);  // vector for pointers
}
template<class K, class T> InternalNode_multi<K,T>::~InternalNode_multi()
{
    for (int i = 0; i < MAXNUM_POINTER_MULTI; i++)
    {
        children[i] = NULL;
    }
}

/*
    Insert a key to a node
*/
template<class K,class T> bool InternalNode_multi<K,T>::Insert(K key, BPlusNode_multi<K,T>* pNode)
{
    int i;
    // 如果中间结点已满，直接返回失败
    if (this->Get_count() >= MAXNUM_KEY_MULTI)
    {
        return false;
    }


    // 找到要插入键的位置
    for (i = 0; (this->CompareTo(key,keys[i])==1) && ( i < this->key_num ); i++)
    {
    }

    keys.insert(keys.begin()+i,key);
    children.insert(children.begin()+i+1,pNode);

    pNode->Set_Father(this);      // 非常重要  该函数的意思是插入关键字value及其所指向子树

    this->key_num++;

    // 返回成功
    return true;
}


/*
    Delte a key in a node and all pointer behind its position.
*/
template<class K,class T> bool InternalNode_multi<K,T>::Delete(K key)
{
    int i;
    // Find key's position
    for (i = 0; i < this->key_num; i++)
    {
        if(this->CompareTo(key,keys[i])==0){
            break;
        }
    }
    // i==key_num means that we didn't find key in keys
    if(i==this->key_num){
        return false;
    }

    keys.erase(keys.begin()+i);
    keys.resize(keys.size()+1);
    children.erase(children.begin()+(i+1));
    children.resize(children.size()+1);

    this->key_num--;

    return true;
}


/*
    Split a node
*/
template<class K,class T> K InternalNode_multi<K,T>::Split(InternalNode_multi<K,T>* pNode, K key)  //key是新插入的值，pNode是分裂结点
{
    int i = 0, j = 0;
   
    // 如果要插入的键值在第V和V+1个键值中间，需要翻转一下，因此先处理此情况
    if ( (this->CompareTo(key,this->Get_key(m_multi))==1)  && (this->CompareTo( key,this->Get_key(m_multi + 1))==-1)){
        // 把第V+1 -- 2V个键移到指定的结点中

        for (i = m_multi + 1; i <= MAXNUM_KEY_MULTI; i++)
        {
            j++;
            pNode->Set_key(j, this->Get_key(i));
            // this->Set_key(i, INVALID_MULTI);
            this->key_num--;
        }

        // 把第V+2 -- 2V+1个指针移到指定的结点中
        j = 1;
        for (i = m_multi + 2; i <= MAXNUM_POINTER_MULTI; i++)
        {
            j++;
            this->Get_ptr(i)->Set_Father(pNode);    // 重新设置子结点的父亲
            pNode->Set_ptr(j, this->Get_ptr(i));
            // this->Set_ptr(i, INVALID_MULTI);
        }

        // 设置好Count个数
        this->Set_count(m_multi);
        pNode->Set_count(m_multi);

        // 把原键值返回
        return key;
    }

    // 以下处理key小于第V个键值或key大于第V+1个键值的情况

    // 判断是提取第V还是V+1个键
    int position = 0;
    if (this->CompareTo(key,this->Get_key(m_multi))==-1)
    {
        position = m_multi;
    }
    else
    {
        position = m_multi + 1;
    }

    // 把第position个键提出来，作为新的键值返回
    K RetKey = this->Get_key(position);

    // 把第position+1 -- 2V个键移到指定的结点中
    j = 0;
    for (i = position + 1; i <= MAXNUM_KEY_MULTI; i++)
    {
        j++;
        pNode->Set_key(j, this->Get_key(i));
        // this->Set_key(i, INVALID_MULTI);
    }

    // 把第position+1 -- 2V+1个指针移到指定的结点中(注意指针比键多一个)
    j = 0;
    for (i = position + 1; i <= MAXNUM_POINTER_MULTI; i++)
    {
        j++;
        this->Get_ptr(i)->Set_Father(pNode);    // 重新设置子结点的父亲
        pNode->Set_ptr(j, this->Get_ptr(i));
        // this->Set_ptr(i, INVALID_MULTI);
    }

    // 清除提取出的位置
    // this->Set_key(position, INVALID_MULTI);

    // 设置好Count个数
    this->Set_count(position - 1);
    pNode->Set_count(MAXNUM_KEY_MULTI - position);


    return RetKey;
}


/*
    Combine different nodes which have few keys
*/
template<class K, class T> bool InternalNode_multi<K,T>::Combine(BPlusNode_multi<K,T>* pNode)
{
    // 参数检查
    if (this->Get_count() + pNode->Get_count() + 1> MAXNUM_KEY_MULTI)    // 预留一个新键的位置
    {
        return false;
    }
   
    // 取待合并结点的第一个孩子的第一个元素作为新键值
    K NewKey = pNode->Get_ptr(1)->Get_key(1);  //疑问：感觉应该改为KEY_TYPE NewKey = pNode->Get_key(1);

    keys[this->key_num] = NewKey;
    this->key_num++;
    children[this->key_num] = pNode->Get_ptr(1);   //疑问：感觉应该为children[this->key_num+1] = pNode->Get_ptr(1);

    pNode->Get_ptr(1)->Set_Father(this); // MODIFIED

    for (int i = 1; i <= pNode->Get_count(); i++)
    {
        keys[this->key_num] = pNode->Get_key(i);
        this->key_num++;
        children[this->key_num] = pNode->Get_ptr(i+1);

        pNode->Get_ptr(i+1)->Set_Father(this); // MODIFIED
    }

    return true;
}




// 从另一结点移一个元素到本结点
template<class K,class T> bool InternalNode_multi<K,T>::MoveOneElement(BPlusNode_multi<K,T>* pNode)
{
    // 参数检查
    if (this->Get_count() >= MAXNUM_KEY_MULTI)
    {
        return false;
    }

    int i,j;


    // 兄弟结点在本结点左边
    if (this->CompareTo(pNode->Get_key(1),this->Get_key(1))==-1)
    {
        // 先腾出位置
        for (i = this->key_num; i > 0; i--)
        {
            keys[i] = keys[i -1];
        }
        for (j = this->key_num + 1; j > 0; j--)
        {
            children[j] = children[j -1];
        }

        // 赋值
        // 第一个键值不是兄弟结点的最后一个键值，而是本结点第一个子结点的第一个元素的值
        
        // MODIFIED
        BPlusNode_multi<K,T> *k = this->Get_ptr(1);
        while (k->Type() != LEAF_MULTI)
        {
            k = k->Get_ptr(1);
        }
        keys[0] = k->Get_key(1);
        //
        
        
        

        // 第一个子结点为兄弟结点的最后一个子结点
        children[0] = pNode->Get_ptr(pNode->Get_count() + 1);
        pNode->Get_ptr(pNode->Get_count() + 1)->Set_Father(this); //Modified
       
        // 修改兄弟结点
        // pNode->Set_key(pNode->Get_count(), INVALID_MULTI);
        // pNode->Set_ptr(pNode->Get_count() + 1, INVALID_MULTI);
    }
    else    // 兄弟结点在本结点右边
    {
        // 赋值
        // 最后一个键值不是兄弟结点的第一个键值，而是兄弟结点第一个子结点的第一个元素的值
        
        //Modified
        BPlusNode_multi<K,T> *k = pNode->Get_ptr(1);
        while (k->Type() != LEAF_MULTI)
        {
            k = k->Get_ptr(1);
        }

        
        keys[this->key_num] = k->Get_key(1);
        //


        // 最后一个子结点为兄弟结点的第一个子结点
        children[this->key_num + 1] = pNode->Get_ptr(1);
        pNode->Get_ptr(1)->Set_Father(this); //Modified
       
        // 修改兄弟结点
        for (i = 1; i <= pNode->Get_count() - 1; i++) //MODIFIED
        {
            pNode->Set_key(i, pNode->Get_key(i + 1));
        }
        for (j = 1; j <= pNode->Get_count(); j++) //MODIFIED
        {
            pNode->Set_ptr(j, pNode->Get_ptr(j + 1));
        }
        // Modified(new)
        // pNode->Set_key(pNode->Get_count(), INVALID_MULTI);
        pNode->Set_ptr(pNode->Get_count() + 1, NULL);
        //
    }

    // 设置数目
    this->Set_count(this->Get_count() + 1);
    pNode->Set_count(pNode->Get_count() - 1);

    return true;
}



//---- Next part is for LeafNode_multi----

template<class K, class T> LeafNode_multi<K,T>::LeafNode_multi(int (*Compare)(const K&, const K&))
{
    this->node_type=LEAF_MULTI;

    // for (int i = 0; i < MAXNUM_KEY_MULTI; i++)
    // {
    //     main_keys[i] = INVALID_MULTI;
    // }
    overflow_size=MAX_OVERFLOW_SIZE_MULTI; // the size of overflow block
    main_size=MAX_MAINBLOCK_SIZE_MULTI;  // the size of main block     ps: overflow_size + main_size = m_multi
    overflow_num=0;
    main_num=0;
    keys=vector<K>(MAXNUM_KEY_MULTI);
    main_block=vector< pair<K,vector<T*>*> >();
    overflow_block=vector< pair<K,vector<T*>*> >() ;
    m_pPrevNode = NULL;
    m_pNextNode = NULL;
    this->CompareTo=Compare;
    LeafNode_multi<K,T>::Compare_comp=Compare;

}
template<class K, class T> LeafNode_multi<K,T>::~LeafNode_multi()
{

}

/*
    Get data in leaf ndoes by giving a key
*/
template<class K, class T> vector<T*>* LeafNode_multi<K,T>::Get_Data(K key){
    int i;
    bool found = false;
    // Find key in overflow block
    for (i=0;i<overflow_num;i++){
        if (this->CompareTo(key,overflow_block[i].first) ==0)
        {
            found = true;
            return overflow_block[i].second;
        }
    }
    // Find key in main block
    if(found==false){
        for(i = 0; i <main_num; i++){
            if (this->CompareTo(key,main_block[i].first)==0)
            {
                found = true;
                return main_block[i].second;
            }
        }
    }
    // 如果没有找到，返回失败
    if (false == found)
    {
        return NULL;
    }
    return NULL;
}


/*
    Insert a key and some data into leafnode(block)
*/
template<class K, class T> bool LeafNode_multi<K,T>::Insert(K key,vector<T*>* data)
{
    int i,j;
    pair<K,vector<T*>*> data_pair(key,data);
    // 如果利用key可以找到一个数据，说明原本key就在leaf里，
    if(NULL!=Get_Data(key)){
        for(i=0;i<overflow_num;i++){
            if(this->CompareTo(key,overflow_block[i].first)==0){
                overflow_block[i]=data_pair;
                return true;
            }
        }
        for(i=0;i<main_num;i++){
            if(this->CompareTo(key,main_block[i].first)==0){
                main_block[i]=data_pair;
                return true;
            }
        }
    }

    // 如果叶子结点已满，直接返回失败
    if (this->Get_count() >= MAXNUM_KEY_MULTI)
    {
        return false;
    }
    // 找到要插入数据的位置
    // 先将key插入到keys中对应位置中
    

    for (i = 0;  (i < this->Get_count()) && this->CompareTo(key,keys[i])==1 ; i++)
    {
    }

    // 当前位置及其后面的数据依次后移，空出当前位置
    for (j = this->key_num; j > i; j--)
    {
        keys[j] = keys[j - 1];
    }

    // 把数据存入当前位置
    keys[i] = key;

    this->key_num++;

    // 再插入数据资料
    // 查看overflow block是否满了，如果满了，转移到main block中
    if(overflow_num>=overflow_size){
        // 转移
        while(overflow_num!=0){
            // move all the elements to main block
            main_block.push_back(overflow_block.back());
            main_num++;
            overflow_block.pop_back();
            overflow_num--;
            if(main_num==MAX_MAINBLOCK_SIZE_MULTI)
                break;
        }
        sort(main_block.begin(),main_block.end(),this->comp);
    }

    
    // 如果overflowblock没有满，或者已经完成了转移
    overflow_block.push_back(data_pair);
    overflow_num++;
    // 返回成功
    return true;
}

/*
    Delete a key and corresponding data in leaf node
*/
template<class K, class T> bool LeafNode_multi<K,T>::Delete(K key)
{
    int i;
    bool main_found = false;
    bool overflow_found = false;
    bool found = false;
    // Firstly, deal with keys[]
    for (i = 0; i < this->key_num; i++)
    {
        if (this->CompareTo(key, keys[i])==0)
        {
            found = true;
            break;
        }
    }
    if (false==found)
    {
        return false;
    }

    keys.erase(keys.begin()+i); // Delte correspond key in keys[]
    keys.resize(keys.size()+1);
    this->key_num--;


    // Find key+data in main block
    for (i = 0; i <main_num; i++)
    {
        if (this->CompareTo(key,main_block[i].first)==0)
        {
            main_found = true;
            break;
        }
    }
    // Find key+data in overflow block
    if(main_found==false){
        for (i=0;i<overflow_num;i++){
            if (this->CompareTo(key,overflow_block[i].first)==0)
            {
                overflow_found = true;
                break;
            }
        }
    }


    // Delete keys+data in corresponding block
    if(true==main_found){
        main_block.erase(main_block.begin()+i);
        main_num--;
        //main_block.resize(main_block.size()+1);
    }
    if(true==overflow_found){
        overflow_block.erase(overflow_block.begin()+i);
        overflow_num--;
    }


    // 返回成功
    return true;
}

// 分裂叶子结点，把本叶子结点的后一半数据剪切到指定的叶子结点中
template <class K, class T> K LeafNode_multi<K,T>::Split(LeafNode_multi<K,T>* pNode)    
{
    // 把本叶子结点的后一半数据移到指定的结点中
    for (int i = m_multi ; i < MAXNUM_KEY_MULTI; i++)
    {
        pNode->Insert(keys[m_multi],Get_Data(keys[m_multi]));
        this->Delete(keys[m_multi]);
    }
    // 设置好Count个数,在我们的代码中应该不需要
    // this->Set_count(this->Get_count() - j);
    // pNode->Set_count(pNode->Get_count() + j);

    // 返回新结点的第一个元素作为键
    return pNode->Get_key(1);
}

template<class K,class T>bool LeafNode_multi<K,T>::Combine(LeafNode_multi<K,T>* pNode)
{
    // If there are too many elements, they can not be combined
    if ((this->Get_count() + pNode->Get_count()) > MAXNUM_KEY_MULTI)
    {
        return false;
    }
    // Insert all the elements from pNode into current node
    for (int i = 1; i <= pNode->Get_count(); i++)
    {
        this->Insert(pNode->Get_key(i),pNode->Get_Data(pNode->Get_key(i)));
    }
    // 后续还要对parent进行更改
    return true;
}

template<class K, class T> int (*LeafNode_multi<K,T>::Compare_comp)(const K&, const K&) = nullptr;//LeafNode_multi<K,T>::empty;

#endif