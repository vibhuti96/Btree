//btree 20/4/16 by vibhuti
#include<iostream>
#include<stdlib.h>
#include<math.h>
using namespace  std;
int order;
struct node
{
    int keys[11];
    struct node *child[11];
    bool leaf;
    int keys_fill;
    struct node *par;
}*root;
void sort_node(struct node *n,int split)
{
    int i,j,k,maximum,max_pt,temp;
    //if sort in case of split because keys_fill is not increased and no. of keys =order
    if(split==1)
        k=order;
    else
        k=n->keys_fill;
        //selection sort
    for(i=k-1;i>=0;i--)
    {
        maximum=n->keys[0];
        max_pt=0;
        for(j=0;j<=i;j++)
        {
         if(n->keys[j]>maximum)
         {
             maximum=n->keys[j];
             max_pt=j;
         }
        }
       temp=n->keys[i];
       n->keys[i]=n->keys[max_pt];
       n->keys[max_pt]=temp;
    }
}
void traversal(struct node *root);
struct node * split(struct node *n)
{
    int i,j;
   int mid=(order-1)/2;
    //for first node to be splitted
  if(n->par==NULL)
{
    //creating new node as root
      struct node *one=(struct node *)malloc(sizeof(struct node));
      //initialing parameters of root
      one->keys[0]=n->keys[mid];
      one->keys_fill=1;
      one->leaf=0;
      one->par=NULL;
      root=one;
      //initialising new children
      one->child[1]=(struct node *)malloc(sizeof(struct node));
      one->child[1]->keys_fill=order-1-mid;
      one->child[1]->par=one;
      //initialising keys i+mid+1
      for(i=0;i<order-mid-1;i++)
      {
          one->child[1]->keys[i]=n->keys[i+mid+1];
      }
      one->child[1]->leaf=1;
      //assigning child values to 1
      for(i=0;i<=one->child[1]->keys_fill;i++)
      {
      one->child[1]->child[i]=n->child[i+mid+1];
      if(one->child[1]->child[i]!=NULL)
      one->child[1]->child[i]->par=one->child[1];
      }
      //changing pARAMETERS OF the existing node
      if(n->child[0]!=NULL)
        one->child[1]->leaf=0;
      n->keys_fill=mid;
      n->leaf=1;
      //child values to 2
      if(n->child[0]!=NULL)
        n->leaf=0;
      n->par=one;
      one->child[0]=n;
      return root;
}
  //for further nodes
  else
  {
      //locating child index
      for(i=0;i<=n->par->keys_fill;i++)
      {
          if(n->par->child[i]==n)
            break;
      }
      //shifting-
      for(j=n->par->keys_fill;j>=i+1;j--)
      {
          n->par->keys[j]=n->par->keys[j-1];
      }
      n->par->keys[i]=n->keys[mid];
      //shifting child
      for(j=n->par->keys_fill+1;j>=i+2;j--)
      {
          n->par->child[j]=n->par->child[j-1];
      }
      //initialising new child
      n->par->child[i+1]=(struct node *)malloc(sizeof(struct node));
      if(n->leaf==1)
      n->par->child[i+1]->leaf=1;
      else
        n->par->child[i+1]->leaf=0;
      n->par->child[i+1]->par=n->par;
      n->par->child[i+1]->keys_fill=order-mid-1;
      //initialising keys
      for(j=0;j<order-mid-1;j++)
      {
          n->par->child[i+1]->keys[j]=n->keys[j+mid+1];
      }
       n->keys_fill=mid;
       //initialising child
       if(n->leaf!=1)
       {
        for(j=0;j<=n->par->child[i+1]->keys_fill;j++)
      {
      n->par->child[i+1]->child[j]=n->child[j+mid+1];
      if(n->par->child[i+1]->child[j]!=NULL)
      {
       n->par->child[i+1]->child[j]->par=n->par->child[i+1];
      }
      }
       }
      if(n->par->keys_fill!=order-1)
      n->par->keys_fill++;
      //splitting again if==order-1
      else
      {
        root=split(n->par);
      }
}
return root;
}
struct node * insert_num(struct node *n,int num)
{int i;
    if(n->keys_fill<order-1)
    {
    n->keys_fill++;
   n->keys[n->keys_fill-1]=num;
   for(i=0;i<=n->keys_fill;i++)
    n->child[i]=NULL;
   sort_node(n,0);
    }
    else
    {
    n->keys[order-1]=num;
     sort_node(n,1);
     root=split(n);
    }
    return root;
}
struct node * insertion(int num,struct node *n)
{
    int i;
    if(n->leaf==1)
    {
        root=insert_num(n,num);
    }
    else
    {
        for(i=0;i<n->keys_fill;i++)
        {
            if(num<n->keys[i])
            {
                root=insertion(num,n->child[i]);
                return root;
            }
        }
        if(i==n->keys_fill)
            root=insertion(num,n->child[i]);
}
return root;
}
//index is that variable which denotes the child index of n
struct node *combine(struct node *n,int index,int i)
{int j,k=0,flag=0,take2;
//num is at last child
  if(index+1>n->par->keys_fill)
  {
      //not deletion further
      if(i!=n->keys_fill)
{
    //shifting to right so as to allocate space for parent
for(j=i;j>0;j--)
   {
n->keys[j]=n->keys[j-1];
   }
   n->keys[0]=n->par->keys[index-1];
  }
  //in case of further deletion
   else
   {
        for(j=n->keys_fill;j>=1;j--)
            n->keys[j]=n->keys[j-1];
        n->keys[0]=n->par->keys[index-1];
        n->keys_fill++;
        flag=1;
   }
   if(flag!=1)
    take2=0;
   else
    take2=1;
    //merging left sibling with node and intervening parent key
    //first shift keys of node itself so as to provide space to sibling
   for(j=n->keys_fill+n->par->child[index-1]->keys_fill-1;k<n->keys_fill;j--)
   {
       n->keys[j]=n->keys[n->keys_fill-1-k];
       n->child[j+1]=n->child[n->keys_fill-take2-k];
       k++;
   }
   if(flag!=1)
   n->child[j+1]=n->child[n->keys_fill-k];
   //merging  sibling keys in the space allocated
   for(j=0;j<n->par->child[index-1]->keys_fill;j++)
   {
       n->keys[j]=n->par->child[index-1]->keys[j];
       //child of par
       n->child[j]=n->par->child[index-1]->child[j];
       //parent of child
       if(n->leaf!=1)
        n->child[j]->par=n;
   }
   n->child[j]=n->par->child[index-1]->child[j];
     if(n->leaf!=1)
        n->child[j]->par=n;
   n->keys_fill=n->keys_fill+n->par->child[index-1]->keys_fill;
   n->par->child[index-1]=n;
   n->par->keys_fill--;
  }
  //in case node is not the last child
  else
  {
      //shifting keys to left
   for(j=i;j<n->keys_fill-1;j++)
   {
n->keys[j]=n->keys[j+1];
   }
   //bringing parent down keys are not increased as num is deleted and parent brought down
   if(i!=n->keys_fill)
   n->keys[n->keys_fill-1]=n->par->keys[index];
   //in case of further deletion keys are increased
   else
   {
        n->keys[n->keys_fill]=n->par->keys[index];
        n->keys_fill++;
        flag=1;
   }
   int take;
   if(flag!=1)
    take=1;
   else
    take=0;
    //merging it with right sibling
   for(j=0;j<n->par->child[index+1]->keys_fill;j++)
   {
       //keys and children shifted and child parent is changed
       n->keys[n->keys_fill+j]=n->par->child[index+1]->keys[j];
       n->child[n->keys_fill+j+take]=n->par->child[index+1]->child[j];
       if(n->leaf!=1)
       n->child[n->keys_fill+j+take]->par=n;
   }
   n->child[n->keys_fill+j+take]=n->par->child[index+1]->child[j];
   if(n->leaf!=1)
       n->child[n->keys_fill+j+take]->par=n;
   n->keys_fill=n->keys_fill+n->par->child[index+1]->keys_fill;
   //shifting parent keys to left from the intervening key as it is brought down
   for(j=index;j<n->par->keys_fill-1;j++)
    n->par->keys[j]=n->par->keys[j+1];
       for(j=index+1;j<n->par->keys_fill;j++)
    n->par->child[j]=n->par->child[j+1];
    n->par->keys_fill--;
  }
  //if par is root and root becomes NULL root is changed to the only child
  if(n->par==root&&root->keys_fill==0)
    root=n;
    //if  par becomes deficient of keys further merging is to take place
    else if(n->par->keys_fill<ceil((float)order/2)-1&&n->par!=root)
    {
        if(n->par->par!=NULL)
    {
        for(j=0;j<=n->par->par->keys_fill;j++)
        {
            if(n->par->par->child[j]==n->par)
            break;
        }
        combine(n->par,j,n->par->keys_fill);
    }
    }
}
//preorder traversal(hash i.e - is to for keys of same node;',' is used for left and right child;'()' are usedv for further children of a node
void traversal(struct node *root)
{
int i;
if(root!=NULL)
{
for(i=0;i<root->keys_fill;i++)
{
    cout<<root->keys[i];
    if(i!=root->keys_fill-1)
        cout<<"-";
    if(root->leaf!=1)
    cout<<"(";
    if(root->leaf!=1)
traversal(root->child[i]);
if((i==(root->keys_fill-1))&&(root->leaf!=1))
{
    cout<<",";
traversal(root->child[root->keys_fill]);
}
if(root->leaf!=1)
cout<<")";
}
}
}
//search the node for deletion
struct node * searching(struct node *n,int num)
{
    int index,i;
    static int j;
    struct node *ptr;
    for(i=0;i<n->keys_fill;i++)
    {
        //if found return ptr to node
        if(n->keys[i]==num)
        {
            cout<<"found\n";
        return n;
        }
        //if less than the key
        else if(num<n->keys[i]&&n->leaf!=1)
        {

            ptr=searching(n->child[i],num);
            return ptr;
        }
    }
    //if greater than key
    if(num>n->keys[i-1]&&n->leaf!=1)
    {
        ptr=searching(n->child[n->keys_fill],num);
        return ptr;
    }
        if(n->leaf==1&&i==n->keys_fill)
        {
            cout<<"not found\n";
            return NULL;
        }
        return ptr;
}
struct node *deletion(struct node * n,int num)
{
    int i,j,temp,k;
    //find position of num in node
    for(k=0;k<n->keys_fill;k++)
        {
            if(n->keys[k]==num)
                break;
        }
        //find the index at which it is child
        if(n->par!=NULL)
        {
        for(i=0;i<=n->par->keys_fill;i++)
        {
            if(n->par->child[i]==n)
                break;
        }
        }
        //if leaf node
    if(n->leaf==1)
    {
        if(n==root)
        {
            for(j=k;j<n->keys_fill-1;j++)
            n->keys[j]=n->keys[j+1];
            n->keys_fill--;
        }
        else if((n->keys_fill)==ceil((float)order/2)-1)
        {
            //if left sibling has greater keys than minimum
        if(i-1>=0&&(n->par->child[i-1]->keys_fill>ceil((float)order/2)-1))
        {
            temp=n->par->keys[i-1];
            n->par->keys[i-1]=n->par->child[i-1]->keys[n->par->child[i-1]->keys_fill-1];
         n->par->child[i-1]->keys_fill--;
         for(j=k;j>=1;j--)
         {
             n->keys[j]=n->keys[j-1];
         }
         n->keys[0]=temp;
        }
        //if right sibling more keys than minimum
        else if(i+1<=n->par->keys_fill&&(n->par->child[i+1]->keys_fill>ceil((float)order/2)-1))
        {
         temp=n->par->keys[i];
         n->par->keys[i]=n->par->child[i+1]->keys[0];
         for(j=0;j<n->par->child[i+1]->keys_fill-1;j++)
            n->par->child[i+1]->keys[j]=n->par->child[i+1]->keys[j+1];
         n->par->child[i+1]->keys_fill--;
         for(j=k;j<n->keys_fill-1;j++)
            n->keys[j]=n->keys[j+1];
         n->keys[n->keys_fill-1]=temp;
        }
        //if none than bring
        else
         combine(n,i,k);
        }
        else
        {
           for(i=0;i<n->keys_fill;i++)
           {
                if(n->keys[i]==num)
                break;
           }
          for(j=i;j<n->keys_fill-1;j++)
                    n->keys[j]=n->keys[j+1];
          n->keys_fill--;
        }
    }
    //in case of internal node swapped with right child first node until reaches leaf and then deleted from leaf
    else
    {
        struct node *n1;
        n1=n->child[k+1];
       while(n1->leaf!=1)
        n1=n1->child[0];
        temp=n->keys[k];
        n->keys[k]=n1->keys[0];
        n1->keys[0]=temp;
        deletion(n1,num);
    }
}
int main()
{
    int n;
    struct node *n1=(struct node *)malloc(sizeof(struct node));
    struct node *found;
    root=n1;
    root->leaf=1;
    root->par=NULL;
    root->keys_fill=0;
    cout<<"enter order\n";
    cin>>order;
    if(order==1)
    {
    cout<<"cannot be formed\n";
    exit(0);
    }
    cout<<"enter no. of elements\n";
    cin>>n;
    int i,num;
    cout<<"enter numbers\n";
    for(i=0;i<n;i++)
    {
        cin>>num;
        insertion(num,root);
        cout<<"traversal=";
        traversal(root);
        cout<<"\n";

    }
    cout<<"how many times you want to delete\n";
    cin>>n;
    for(i=0;i<n;i++)
    {
    cout<<"enter no. you want to delete\n";
    cin>>num;
    found=searching(root,num);
    if(found==NULL)
        cout<<"cannot be deleted\n";
        else
        {
            if(found==root&&root->leaf==1&&root->keys_fill==1)
            cout<<"cannot be further deleted\n";
            else
                {
            deletion(found,num);
            traversal(root);
            cout<<"\n";
                }
        }
}
}

