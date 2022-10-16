#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "BPlus_multi.h"


int Compare(const int &a,const int &b){
    if(a<b){
        return -1;
    }
    if(a==b){
        return 0;
    }
    return 1;
}

bool ISvalid(const int &a,const int* b){
    if(a-*b>10 ||*b-a>10)
        return false;
    return true;
}


// 随机建立一棵树
void Test1(BPlus_multi<int,int>* pTree, int count,int* z)
{
    pTree->ClearTree();
    //srand(12);
    //srand( (unsigned)time( NULL ) );//这是一个种子，如果不要随机功能，请把此句话注释掉
    for (int i = 0; i < count; i++)
    {
        z[i] = rand()%999 + 1;
        (void)pTree->Insert(z[i],&z[i]);
    }

    printf("successed!\n");
}

// 在树中查找某数据
void Test2(BPlus_multi<int,int>* pTree, int data)
{
    //char sPath[255] = {0, };
    int i,j;
    vector<int*>* vec=pTree->Find(data);
    if(vec==NULL){
        printf("Sorry, there is no data for this key!");
        return;
    }
    vector<int*>  vect=*vec;
    printf("\nthe data you want");
    for(i=0;i<vect.size();i++){
        cout<<*vect[i];
        cout<<" ";
    }

}

// 在树中插入某数据
void Test3(BPlus_multi<int,int>* pTree,int* key, int* data)
{
    int k=*key;
    bool success = pTree->Insert(k,data);
    if (true == success)
    {
        printf("\nsuccessed!\n");
    }
    else
    {
        printf("\nfailed!\n");
    }
}

// 在树中删除某数据
void Test4(BPlus_multi<int,int>* pTree, int data)
{
    bool success = pTree->Delete(data);
    if (true == success)
    {
        printf("\nsuccessed!\n");
    }
    else
    {
        printf("\nfailed!\n");
    }
}

// // 对树进行旋转
// BPlus_multi<int,int>* Test5(BPlus_multi<int,int>* pTree)
// {
//     BPlus_multi<int,int>* pNewTree = pTree->RotateTree();
//     delete pTree;
//     printf("\nsuccessed!\n");
//     return pNewTree;
// }

// 打印
void Test6(BPlus_multi<int,int>* pTree)
{
    pTree->PrintTree();
    cout<<"\n";
    cout<<"Besides, the data in blocks should be like:";
    pTree->PrintData();
}

// 对树进行检查
void Test7(BPlus_multi<int,int>* pTree)
{
    bool success = pTree->CheckTree();
    if (true == success)
    {
        printf("\nsuccessed!\n");
    }
    else
    {
        printf("\nfailed!\n");
    }
}

// Range Test
void Test8(BPlus_multi<int,int>* pTree, int a, int b)
{
    vector<int*>* vec_ptr= pTree->Range(a,b,true,true);
    vector<int*>  vec=*vec_ptr;
    cout<<"The result of range is: ";
    for(int i=0;i<(int)vec.size();i++){
        cout<<*(vec[i])<<" ";
    }
}


int main(int argc, char* argv[])
{
    BPlus_multi<int,int>* pTree = new BPlus_multi<int,int>(1,Compare,ISvalid);

    int x = 1;
    int z[150];
    int h[150];
    int i =0;
    int y = 0;
    while (0 != x)
    {
        printf("\n\n");
        printf("    *******************************************************************\n");
        printf("    *           B+Tree demmo. Enter number to call function          *\n");
        printf("    *           1.Randomly build a B+Tree；                           *\n");
        printf("    *           2.Find a key's data ；                               *\n");
        printf("    *           3.Insert a key/data in B+Tree；                      *\n");
        printf("    *           4.Delte a key in B+Tree；                             *\n");
        printf("    *           5.To be done(useless function for CA2)；            *\n");
        printf("    *           6.Show whole B+Tree；                                *\n");
        printf("    *           7.Check whole B+Tree；                                 *\n");
        printf("    *           8.Range；                                             *\n");
        printf("    *           0.Exit the program；                                   *\n");
        printf("    *******************************************************************\n");
        printf("\n    Please enter here：");



        scanf("%d", &x);
        switch (x)
        {
        case 1:
            printf("How many keys do you want？(10-150):");
            scanf("%d", &y);
            Test1(pTree, y,z);
            break;

        case 2:
            printf("The key you want to find：");
            scanf("%d", &y);
            Test2(pTree, y);
            break;

        case 3:
            printf("Enter both key and data you want to insert：");
            scanf("%d %d", &z[i],&h[i]);
            Test3(pTree, &z[i],&h[i]);
            i++;
            break;

        case 4:
            printf("The key you want to delete：");
            scanf("%d", &y);
            Test4(pTree, y);
            break;

        case 5:
            // pTree = Test5(pTree);
            printf("sorry，this function has not been implemented since it is not important for CA2");
            break;

        case 6:
            Test6(pTree);
            break;

        case 7:
            Test7(pTree);
            break;

        case 8:
            int a,b;
            printf("Enter the minimum key and the max key：");
            scanf("%d %d", &a,&b);
            Test8(pTree,a,b);
            break;

        case 0:
            delete pTree;
            return 0;
            break;

        default:
            break;
        }
    }

    delete pTree;
    return 0;
}