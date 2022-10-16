#ifndef BPLUSH
#define BPLUSH
#include "NodeBlock.h"
/*Class for BPlusTree Tree
  A typical BPlusTree Tree that is exclusive to duplicated keys*/
template<class K, class T> class BPlusTree {
public:
    ///MAXNODE: degree of the tree; CompareTo: function pointer that compare two keys.
    BPlusTree(int degree,int (*Compare)(const K&, const K&));
    virtual ~BPlusTree();  
    /*key: an instant of key; item: corresponding item pointer;
    If the key is not in the tree, insert it. Otherwise, update its T* with the new one */
    virtual bool Insert(K key, T* data);
    virtual bool Insert_InternalNode(InternalNode<K,T>* pNode, BPlusNode<K,T>* rightson,K key,T* data);
    /*key: an instant of key; 
    Return the corresponding value. 
    Return nullptr if not found.*/              
    virtual T* find(K key); 
    virtual T* Find(K key); 
    /*key_min: define the lower bound of the range; key_max: define the upper bound of the range. min_inclusive: decide if the lower bound is inclusive. max_inclusive: decide if the upper bound is inclusive. 
    Return a pointer to vector<T*> (created by "new") containing all the items whose keys are within the range.
    Return nullptr if there is no such item.*/
    vector<T*>* Range (K key_min, K key_max, bool min_inclusive, bool max_inclusive); 

    LeafNode<K,T>* SearchLeafNode(K key); // Return the best leaf node for a key


    /*key: the key to be deleted. Remove a key and its corresponding T*, if the key is in the node*/  
    virtual bool Delete (K key);
    virtual bool Delete_InternalNode(InternalNode<K,T>* pNode, K key);
    virtual void ClearTree();



    // Use for debugging
    bool CheckTree();   // Check if current tree satisfy the property of B+Tree
    void PrintTree();
    void PrintNode(BPlusNode<K,T>*);
    void PrintData();
    bool CheckNode(BPlusNode<K,T>* pNode);  // Recursively check whether pNode and its children satisfy property of B+Tree

    BPlusNode<K,T>* Get_root(){
    return root;
    }

    void Set_root(BPlusNode<K,T>* r){
    root=r;
    }

    int Get_depth(){
    return depth;
    }


    LeafNode<K,T>* LeafHead;  // Head node of leaf link list
    LeafNode<K,T>* LeafTail;  // Tail node of leaf link list

protected:
  int (*CompareTo) (const K& left, const K& right);    //CompareTo returns -1 when left < right, 0 when left == right, 1 when left > right.
  // more private members
  BPlusNode<K,T>* root;
    int depth;
};


//---- This part is for B+Tree with single key ----


template<class K,class T> BPlusTree<K,T>::BPlusTree(int degree,int (*Compare)(const K&, const K&))
{
    depth = 0;
    degree;
    root = NULL;
    LeafHead = NULL;
    LeafTail = NULL;
    CompareTo=Compare;
}
template<class K,class T> BPlusTree<K,T>::~BPlusTree()
{
    ClearTree();
}

/*
  find corresponding data by key.
  If there is not such key in B+Tree, function will return NULL,
  or it will return T* we want.
*/
template<class K,class T>  T* BPlusTree<K,T>::find(K key)
{
    if(this->root==NULL){
        return NULL;
    }

    LeafNode<K,T>* pNode=SearchLeafNode(key);
    if(pNode->Type()==LEAF && pNode!=NULL){
        T* ret=((LeafNode<K,T>*)pNode)->Get_Data(key);
        if(NULL!=ret){
            return ret;
        }else{
        return NULL;
        }
    }
    // 在叶子结点中继续找
    return NULL;

}

template<class K,class T>  T* BPlusTree<K,T>::Find(K key){
    T* find_data=find(key);
    if (find_data == NULL){
        return NULL;
    }
    T* ret= new T;
    *ret = *find_data;
    return ret;
}

/* 在B+树中插入数据
插入数据首先要找到理论上要插入的叶子结点，然后分三种情况：
(1) 叶子结点未满。直接在该结点中插入即可；
(2) 叶子结点已满，且无父结点(即根结点是叶子结点)，需要首先把叶子结点分裂，然后选择插入原结点或新结点，然后新生成根节点；
(3) 叶子结点已满，但其父结点未满。需要首先把叶子结点分裂，然后选择插入原结点或新结点，再修改父结点的指针；
(4) 叶子结点已满，且其父结点已满。需要首先把叶子结点分裂，然后选择插入原结点或新结点，接着把父结点分裂，再修改祖父结点的指针。
    因为祖父结点也可能满，所以可能需要一直递归到未满的祖先结点为止。
*/
template<class K,class T> bool BPlusTree<K,T>::Insert(K key,T* data) 
{
    // 检查是否重复插入,如果是重复插入后续操作有所不用
    //bool found = find(key);
    // 查找理想的叶子结点
    LeafNode<K,T>* pOldNode = SearchLeafNode(key);
    // 如果没有找到，说明整个树是空的，生成根结点
    if (NULL == pOldNode)
    {
        pOldNode = new LeafNode<K,T>(this->CompareTo);
        LeafHead = pOldNode;
        LeafTail = pOldNode;
        Set_root(pOldNode);
    }

    // 叶子结点未满，对应情况1，直接插入
    if (pOldNode->Get_count() < MAXNUM_KEY)
    {
      pOldNode->Insert(key,data);
      return true;
    }

    // 原叶子结点已满，新建叶子结点，并把原结点后一半数据剪切到新结点
    LeafNode<K,T>* pNewNode = new LeafNode<K,T>(CompareTo);
    K key_split = pOldNode->Split(pNewNode);

    // 在双向链表中插入结点
    LeafNode<K,T>* pOldNext = pOldNode->m_pNextNode;
    pOldNode->m_pNextNode = pNewNode;
    pNewNode->m_pNextNode = pOldNext;
    pNewNode->m_pPrevNode = pOldNode;
    if (NULL == pOldNext)
    {
      LeafTail = pNewNode;
    }
    else
    {
      pOldNext->m_pPrevNode = pNewNode;
    }


    // 判断是插入到原结点还是新结点中，确保是按数据值排序的
    if (CompareTo(key,key_split)==-1)
    {
        pOldNode->Insert(key,data);    // 插入原结点
    }
    else
    {
        pNewNode->Insert(key,data);    // 插入新结点
    }

    // 父结点
    InternalNode<K,T>* pFather = (InternalNode<K,T>*)(pOldNode->Get_Father());

    // 如果原结点是根节点，对应情况2
    if (NULL == pFather)
    {
        BPlusNode<K,T>* pNode1 = new InternalNode<K,T>(CompareTo);
        pNode1->Set_ptr(1, pOldNode);                           // 指针1指向原结点
        pNode1->Set_key(1, key_split);                                // 设置键
        pNode1->Set_ptr(2, pNewNode);                           // 指针2指向新结点
        pOldNode->Set_Father(pNode1);                               // 指定父结点
        pNewNode->Set_Father(pNode1);                               // 指定父结点
        pNode1->Set_count(1);

        Set_root(pNode1);                                           // 指定新的根结点
        return true;
    }

    // 情况3和情况4在这里实现
    bool ret = Insert_InternalNode(pFather,pNewNode, key_split, pNewNode->Get_Data(key_split));
    return ret;
}



/* 删除某数据
删除数据的算法如下：
(1) 如果删除后叶子结点填充度仍>=50%，只需要修改叶子结点，如果删除的是父结点的键，父结点也要相应修改；
(2) 如果删除后叶子结点填充度<50%，需要先找到一个最近的兄弟结点(左右均可)，然后分两种情况：
    A. 如果该兄弟结点填充度>50%，把该兄弟结点的最近一个数据剪切到本结点，父结点的键值也要相应修改。
    B. 如果该兄弟结点的填充度=50%，则把两个结点合并，父结点键也相应合并。(如果合并后父结点的填充度<50%，则需要递归)
*/
template<class K,class T> bool BPlusTree<K,T>::Delete(K key)
{
    // 查找理想的叶子结点
    LeafNode<K,T>* pOldNode = SearchLeafNode(key);
    // 如果没有找到，返回失败
    if (NULL == pOldNode)
    {
        return false;
    }

    // 删除数据，如果失败一定是没有找到，直接返回失败
    bool success = pOldNode->Delete(key);
    if (false == success)
    {
        return false;
    }

    // 获取父结点
    InternalNode<K,T>* pFather = (InternalNode<K,T>*)(pOldNode->Get_Father());
    if (NULL == pFather)
    {
        // 如果一个数据都没有了，删除根结点(只有根节点可能出现此情况)
        if (0 == pOldNode->Get_count())
        {
            delete pOldNode;
            LeafHead = NULL;
            LeafTail = NULL;
            Set_root(NULL);
        }

        return true;
    }

   
    // 删除后叶子结点填充度仍>=50%，对应情况1
    if (pOldNode->Get_count() >= MAXNODE)
    {
        for (int i = 1; (  (CompareTo(key,pFather->Get_key(i))==0)  ||(CompareTo(key,pFather->Get_key(i))==1)) && (i <= pFather->Get_count()); i++)
        {
            // 如果删除的是父结点的键值，需要更改该键
            if (CompareTo(pFather->Get_key(i),key)==0)
            {
                pFather->Set_key(i, pOldNode->Get_key(1));    // 更改为叶子结点新的第一个元素
            }
        }

        return true;
    }

    // 找到一个最近的兄弟结点(根据B+树的定义，除了叶子结点，总是能找到的)
    int flag = FLAG_LEFT;
    LeafNode<K,T>* pBrother = (LeafNode<K,T>*)(pOldNode->Get_sibling(flag));

    // 兄弟结点填充度>50%，对应情况2A
    K new_key;
    if (pBrother->Get_count() > MAXNODE)
    {
        if (FLAG_LEFT == flag)    // 兄弟在左边，移最后一个数据过来
        {
            new_key = pBrother->Get_key(pBrother->Get_count());
        }
        else    // 兄弟在右边，移第一个数据过来
        {
            new_key = pBrother->Get_key(1);
        }

        pOldNode->Insert(new_key,pBrother->Get_Data(new_key));
        pBrother->Delete(new_key);

        // 修改父结点的键值
        if (FLAG_LEFT == flag)
        {
            for (int i = 1; i <= pFather->Get_count() + 1; i++)
            {
                if (pFather->Get_ptr(i) == pOldNode && i > 1)
                {
                    pFather->Set_key(i - 1 , pOldNode->Get_key(1));    // 更改本结点对应的键
                }
            }
        }
        else  // 兄弟为右的情况
        {
            for (int i = 1; i <= pFather->Get_count() + 1; i++)
            {
                if (pFather->Get_ptr(i) == pOldNode && i > 1)
                {
                    pFather->Set_key(i - 1, pOldNode->Get_key(1));    // 更改本结点对应的键
                }
                if (pFather->Get_ptr(i) == pBrother && i > 1)
                {
                    pFather->Set_key(i - 1 , pBrother->Get_key(1));    // 更改兄弟结点对应的键
                }
            }
        }


        return true;
    }

    // 情况2B
   
    // 父结点中要删除的键
    K NewKey ;

    // 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针
   
    if (FLAG_LEFT == flag)
    {
        (void)pBrother->Combine(pOldNode);
        NewKey = pOldNode->Get_key(1);

        LeafNode<K,T>* pOldNext = pOldNode->m_pNextNode;
        pBrother->m_pNextNode = pOldNext;
        // 在双向链表中删除结点
        if (NULL == pOldNext)
        {
            LeafTail = pBrother;
        }
        else
        {
            pOldNext->m_pPrevNode = pBrother;
        }
        // 删除本结点
        delete pOldNode;
    }
    else
    {
        // MODIFIED
        for (int i = 1; i <= pFather->Get_count(); i++)
        {
            if (CompareTo(pFather->Get_key(i) , key)==0)
            {
                pFather->Set_key(i, pOldNode->Get_key(1));
                //cout << "Change father's key successfully!" << endl;
            }
        }
        //

        (void)pOldNode->Combine(pBrother);
        NewKey = pBrother->Get_key(1);

        LeafNode<K,T>* pOldNext = pBrother->m_pNextNode;
        pOldNode->m_pNextNode = pOldNext;
        // 在双向链表中删除结点
        if (NULL == pOldNext)
        {
            LeafTail = pOldNode;
        }
        else
        {
            pOldNext->m_pPrevNode = pOldNode;
        }
         // 删除本结点
        delete pBrother;
    }

    return Delete_InternalNode(pFather, NewKey);
}


// 清除整个树，删除所有结点
template<class K,class T> void BPlusTree<K,T>::ClearTree()
{
    BPlusNode<K,T>* pNode = Get_root();
    if (NULL != pNode)
    {
        pNode->Delete_Children();
        delete pNode;
    }

    LeafHead = NULL;
    LeafTail = NULL;
    Set_root(NULL);
}



// 检查树是否满足B+树的定义
template<class K,class T> bool BPlusTree<K,T>::CheckTree()
{
    LeafNode<K,T> * pThisNode = LeafHead;
    LeafNode<K,T> * pNextNode = NULL;
    while (NULL != pThisNode)
    {
        pNextNode = pThisNode->m_pNextNode;
        if (NULL != pNextNode)
        {
           if (CompareTo(pThisNode->Get_key(pThisNode->Get_count()), pNextNode->Get_key(1))==1)
           {
               return false;
           }
        }
        pThisNode = pNextNode;
    }
       
    return CheckNode(Get_root());
}

// 递归检查结点及其子树是否满足B+树的定义
template<class K,class T>bool BPlusTree<K,T>::CheckNode(BPlusNode<K,T>* pNode)
{
    if (NULL == pNode)
    {
        return true;
    }
   
    int i = 0;
    bool ret = false;
   
    // 检查是否满足50%的填充度
    if ((pNode->Get_count() < MAXNODE) && (pNode != Get_root()))
   {
        return false;
    }

    // 检查键或数据是否按大小排序
    for (i = 1; i < pNode->Get_count(); i++)
    {
        if (CompareTo(pNode->Get_key(i) , pNode->Get_key(i + 1))==1 )
        {
            return false;
        }
    }

    if (LEAF == pNode->Type())
    {
        return true;
    }

    // 对中间结点，递归检查子树
    for (i = 1; i <= pNode->Get_count() + 1; i++)
    {
        ret = CheckNode(pNode->Get_ptr(i));
     // 只要有一个不合法就返回不合法
        if (false == ret)
        {
            return false;
        }
    }

    return true;

}

template<class K,class T> LeafNode<K,T>* BPlusTree<K,T>::SearchLeafNode(K key)
{
    int i = 0;

    BPlusNode<K,T> * pNode = Get_root();
    // 循环查找对应的叶子结点
    while (NULL != pNode)
    {        
        // 结点为叶子结点，循环结束
        if (LEAF == pNode->Type())
        {
            break;
        }

        // 找到第一个键值大于等于key的位置
        for (i = 1; i <= pNode->Get_count(); i++)
        {
            if (CompareTo(key,pNode->Get_key(i))==-1)
            {
                break;
            }
        }

        pNode = pNode->Get_ptr(i);
    }

    return (LeafNode<K,T>*)pNode;
}


//递归函数：插入键到中间结点
template<class K,class T> bool BPlusTree<K,T>::Insert_InternalNode(InternalNode<K,T>* pNode, BPlusNode<K,T>* rightson,K key,T* data)
{
    if (NULL == pNode || LEAF ==pNode->Type())
    {
        return false;
    }

    // 结点未满，直接插入
    if (pNode->Get_count() < MAXNUM_KEY)
    {
        return pNode->Insert(key, rightson);
    }

    InternalNode<K,T>* pBrother = new InternalNode<K,T>(this->CompareTo);  //C++中new 类名表示分配一个类需要的内存空间，并返回其首地址；
    // 分裂本结点
    K NewKey = pNode->Split(pBrother, key);   

    if (pNode->Get_count() < pBrother->Get_count())
    {
        pNode->Insert(key, rightson);
    }
    else if (pNode->Get_count() > pBrother->Get_count())
    {
         pBrother->Insert(key, rightson);
    }
    else    // 两者相等，即键值在第V和V+1个键值中间的情况，把子节点挂到新结点的第一个指针上
    {
        pBrother->Set_ptr(1,rightson);
        rightson->Set_Father(pBrother);
    }

    InternalNode<K,T>* pFather = (InternalNode<K,T>*)(pNode->Get_Father());
    // 直到根结点都满了，新生成根结点
    if (NULL == pFather)
    {
        pFather = new InternalNode<K,T>(this->CompareTo);
        pFather->Set_ptr(1, pNode);                           // 指针1指向原结点
        pFather->Set_key(1, NewKey);                          // 设置键
        pFather->Set_ptr(2, pBrother);                        // 指针2指向新结点
        pNode->Set_Father(pFather);                               // 指定父结点
        pBrother->Set_Father(pFather);                            // 指定父结点
        pFather->Set_count(1);

        Set_root(pFather);                                        // 指定新的根结点
        return true;
    }

    // 递归
    return Insert_InternalNode(pFather, pBrother,NewKey,data);
}



// 递归函数：在中间结点中删除键
template<class K,class T> bool BPlusTree<K,T>::Delete_InternalNode(InternalNode<K,T>* pNode, K key)
{
    // 删除键，如果失败一定是没有找到，直接返回失败
    bool success = pNode->Delete(key);
    if (false == success)
    {
        return false;
    }

    // 获取父结点
    InternalNode<K,T>* pFather = (InternalNode<K,T>*)(pNode->Get_Father());
    if (NULL == pFather)
    {
        // 如果一个数据都没有了，把根结点的第一个结点作为根结点
        if (0 == pNode->Get_count())
        {
            this->Set_root(pNode->Get_ptr(1));
            delete pNode;
        }

        return true;
    }
   
    // 删除后结点填充度仍>=50%
    if (pNode->Get_count() >= MAXNODE)
    {
        for (int i = 1; (CompareTo(key, pFather->Get_key(i))==1||CompareTo(key, pFather->Get_key(i))==0); i++)
        {
            // 如果删除的是父结点的键值，需要更改该键
            if (CompareTo(pFather->Get_key(i),key)==0)
            {
                pFather->Set_key(i, pNode->Get_key(1));    // 更改为叶子结点新的第一个元素
            }
        }

        return true;
    }

    //找到一个最近的兄弟结点(根据B+树的定义，除了根结点，总是能找到的)
    int flag = FLAG_LEFT;
    InternalNode<K,T>* pBrother = (InternalNode<K,T>*)(pNode->Get_sibling(flag));

    // 兄弟结点填充度>50%

    // MODIFIED
    K NewKey_l = pBrother->Get_key(pBrother->Get_count());
    K NewKey_r = pBrother->Get_key(1);
    if (pBrother->Get_count() > MAXNODE)
    //

    {
        pNode->MoveOneElement(pBrother);

        // 修改父结点的键值
        if (FLAG_LEFT == flag)
        {
            for (int i = 1; i <= pFather->Get_count() + 1; i++)
            {
                if (pFather->Get_ptr(i) == pNode && i > 1)
                {
                    pFather->Set_key(i - 1 , NewKey_l);    // MODIFIED 更改本结点对应的键
                }
            }
        }
        else
        {
            for (int i = 1; i <= pFather->Get_count() + 1; i++)
            {
                if (pFather->Get_ptr(i) == pNode && i > 1)
                {
                    pFather->Set_key(i - 1, pNode->Get_key(1));    // 更改本结点对应的键
                }
                if (pFather->Get_ptr(i) == pBrother && i > 1)
                {
                    pFather->Set_key(i - 1 , NewKey_r);    // MODIFIED 更改兄弟结点对应的键
                }
            }
        }

        return true;
    }
   
    // 父结点中要删除的键：兄弟结点都不大于50，则需要合并结点，此时父结点需要删除键
    K NewKey;

    // 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针
    if (FLAG_LEFT == flag)
    {
        (void)pBrother->Combine(pNode);
        NewKey = pNode->Get_key(1);
        delete pNode;
    }
    else
    {
        (void)pNode->Combine(pBrother);
        NewKey = pBrother->Get_key(1);
        delete pBrother;
    }

    // 递归
    return Delete_InternalNode(pFather, NewKey);
}


/*
    Return data between key_min and key_max. You can choose whther include min and max.
    IMPORTANT: The vector returned is not sorted.
*/
template<class K,class T> vector<T*>* BPlusTree<K,T>::Range(K key_min, K key_max, bool min_inclusive, bool max_inclusive){

    // Search for the right node we want
    LeafNode<K,T>* pNode_min = SearchLeafNode(key_min);

    LeafNode<K,T>* pNode_max = SearchLeafNode(key_max);

    // If one of them is NULL, error appears.
    if(pNode_min==NULL || pNode_max==NULL){
        cout<<"Sorry, one key is unavailable.";
        return NULL;
    }

    // Pushback all the elements in the blocks into vector
    vector<T*>* ret=new vector<T*>;
    int i;
    K pkey;  // Particular key


    while(pNode_min!=pNode_max){
        for(i=0;i<pNode_min->main_num;i++){
            // Search in main block.
            pkey=pNode_min->main_block[i].first;
            if( (CompareTo(pkey,key_min)==1) &&  (CompareTo(pkey,key_max)==-1) )
            // If pKey is between min and max, pushback the corresponding data into vector.
                ret->push_back(pNode_min->main_block[i].second);
            if (CompareTo(pkey,key_max)==1)
            // If pKey > max, since main_block is ordered, break the loop
                break;
        }
        for(i=0;i<pNode_min->overflow_num;i++){
            // Search in overflow block.
            pkey=pNode_min->overflow_block[i].first;
            if( (CompareTo(pkey,key_min)==1) &&  (CompareTo(pkey,key_max)==-1))
            // If pKey is between min and max, pushback the corresponding data into vector.
                ret->push_back(pNode_min->overflow_block[i].second);
        }
        pNode_min=pNode_min->m_pNextNode;
    }
    // This part is for pNode_max.
    for(i=0;i<pNode_max->main_num;i++){
        // Search in main block.
        pkey=pNode_max->main_block[i].first;
        if( (CompareTo(pkey,key_min)==1) &&  (CompareTo(pkey,key_max)==-1))
        // If pKey is between min and max, pushback the corresponding data into vector.
            ret->push_back(pNode_max->main_block[i].second);
        if (CompareTo(pkey,key_max)==1)
        // If pKey > max, since main_block is ordered, break the loop
            break;
    }
    for(i=0;i<pNode_max->overflow_num;i++){
        // Search in overflow block.
        pkey=pNode_max->overflow_block[i].first;
        if( (CompareTo(pkey,key_min)==1) &&  (CompareTo(pkey,key_max)==-1))
        // If pKey is between min and max, pushback the corresponding data into vector.
            ret->push_back(pNode_max->overflow_block[i].second);
    }

    if(min_inclusive){
        if(NULL!=this->find(key_min))
            ret->push_back(this->find(key_min));
    }
    if(max_inclusive){
        if(NULL!=this->find(key_max))
            ret->push_back(this->find(key_max));
    }
    return ret;

}



//----use to debug----
template<class K,class T>void BPlusTree<K,T>::PrintTree()
{
    BPlusNode<K,T>* pRoot = Get_root();
    if (NULL == pRoot) return;

    BPlusNode<K,T>* p1, *p2, *p3;
    int i, j, k;
    int total = 0;

    printf("\nFirst layer\n | ");
    PrintNode(pRoot);
    total = 0;
    printf("\nSecond layer\n | ");
    for (i = 1; i <= MAXNUM_POINTER; i++)
    {
        p1 = pRoot->Get_ptr(i);
        if (NULL == p1) continue;
        PrintNode(p1);
        total++;
        if (total%4 == 0) printf("\n | ");
    }
    total = 0;
    printf("\nThird layer\n | ");
    for (i = 1; i <= MAXNUM_POINTER; i++)
    {
        p1 = pRoot->Get_ptr(i);
        if (NULL == p1) continue;
        for (j = 1; j <= MAXNUM_POINTER; j++)
        {
            p2 = p1->Get_ptr(j);
            if (NULL == p2) continue;
            PrintNode(p2);
            total++;
            if (total%4 == 0) printf("\n | ");
        }
    }
    total = 0;
    printf("\nForth layer\n | ");
    for (i = 1; i <= MAXNUM_POINTER; i++)
    {
        p1 = pRoot->Get_ptr(i);
        if (NULL == p1) continue;
        for (j = 1; j <= MAXNUM_POINTER; j++)
        {
            p2 = p1->Get_ptr(j);
            if (NULL == p2) continue;
            for (k = 1; k <= MAXNUM_POINTER; k++)
            {
                p3 = p2->Get_ptr(k);
                if (NULL == p3) continue;
                PrintNode(p3);
                total++;
                if (total%4 == 0) printf("\n | ");
            }
        }
    }
}

// 打印某结点
template<class K,class T> void BPlusTree<K,T>::PrintNode(BPlusNode<K,T>* pNode)
{
    if (NULL == pNode)
    {
        return;
    }
   
    for (int i = 1; i <= MAXNUM_KEY; i++)
    {
        // printf("%3d ", pNode->GetElement(i));
        if(i <= pNode->Get_count()){
            cout<<' '<<pNode->Get_key(i)<<' ';
        }
        else{
            cout<<'x'<<' ';
        }
        if (i >= MAXNUM_KEY)
        {
            printf(" | ");
        }
    }
}

template<class K,class T> void BPlusTree<K,T>::PrintData(){
    LeafNode<K,T>* pNode=LeafHead;
    int i;
    while(pNode!=LeafTail){
        cout<<"| ";
        for(i=0;i<pNode->overflow_num;i++){
             cout<<" ";
            cout<<*(pNode->overflow_block[i].second);
        }
        cout<<" ";
        for(i=0;i<pNode->main_num;i++){
            cout<<" ";
            cout<<*(pNode->main_block[i].second);
        }
        cout<<" |";
        pNode=pNode->m_pNextNode;
    }
    cout<<"| ";
    for(i=0;i<pNode->overflow_num;i++){
         cout<<" ";
        cout<<*(pNode->overflow_block[i].second);
    }
    cout<<" ";
    for(i=0;i<pNode->main_num;i++){
         cout<<" ";
        cout<<*(pNode->main_block[i].second);
    }
}
#endif