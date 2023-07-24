#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define size 5

struct restaurant *headrestaurant=NULL;
struct agent *headagent=NULL;
struct user *headuser=NULL;
struct order* headorder=NULL;
int q=101;


struct restaurant
{
	char name[20];
    char type[20];
    char food[20];
    char address[20];
     
	struct restaurant* left;
    struct restaurant* right;
    int height;
};

struct user
{
	int id;
    char name[20];
    char address[20];
    int phone;

    struct user* right;
	struct user* left;
    struct order* next;
    int height;
};


struct agent
{
	int id;
    char name[20];
    int phone;
    int commission;
    int status;
    char area[20];

	struct agent* left;
    struct agent* right;
    int height;
};

struct order
{
    char name[20];
    char address[20];
    int phone;
    char menu[20];
    char rest_name[20];
    int user_id;
    struct agent* delivery;
    time_t t;
    int order_id;
    int cost;

    struct order* left;
    struct order* right;
    int height;
};

int max(int a, int b)
{
    return (a > b)? a : b;
}




int height_order_history(struct order *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
struct order* rightRotate_order_history(struct order*y)
{
    struct order*x = y->left;
    struct order*T2 = x->right;
 
    
    x->right = y;
    y->left = T2;
 
    
    y->height = max(height_order_history(y->left),height_order_history(y->right)) + 1;
    x->height = max(height_order_history(x->left),height_order_history(x->right)) + 1;
 
    
    return x;
}
 

struct order* leftRotate_order_history(struct order*x)
{
    struct order*y = x->right;
    struct order*T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height_order_history(x->left),height_order_history(x->right)) + 1;
    y->height = max(height_order_history(y->left),height_order_history(y->right)) + 1;
 
    // Return new root
    return y;
}

int getBalance_order_history(struct order* N)
{
    if (N == NULL)
        return 0;
    return height_order_history(N->left) - height_order_history(N->right);
}

struct order* insert_user_hisory(struct order* node,struct order* nnode)//nnode is new node to be added
{
    
    if (node == NULL)
        return nnode;
 
    if (nnode->order_id < node->order_id)
        node->left  = insert_user_hisory(node->left,nnode);
    else if (nnode->order_id > node->order_id)
        node->right = insert_user_hisory(node->right,nnode);
    else 
        return node;
 
    
    node->height = 1 + max(height_order_history(node->left),height_order_history(node->right));
 
    
    int balance = getBalance_order_history(node);
 
    if (balance > 1 && nnode->order_id < node->order_id)
        return rightRotate_order_history(node);
 
    
    if (balance < -1 && nnode->order_id > node->order_id)
        return leftRotate_order_history(node);
 
    
    if (balance > 1 && nnode->order_id > node->order_id)
    {
        node->left =  leftRotate_order_history(node->left);
        return rightRotate_order_history(node);
    }
 
    
    if (balance < -1 && nnode->order_id < node->order_id)
    {
        node->right = rightRotate_order_history(node->right);
        return leftRotate_order_history(node);
    }
 
    
    return node;
}

struct order* minValue_order_history(struct order* node)

{
    struct order* current = node;
    struct order* prev = NULL;
 
    while (current->left != NULL)
    {
        prev=current;
        current = current->left;
    }
    if(prev!=NULL)
    {
        prev->left=NULL;
        //prev->right=NULL;
    }   
    
    return current;
}
struct order* deleteorder(struct order* root, int order_id)// written becauese we need it in 2 functions
{
 
    if (root == NULL)
        return root;
 
    
    if ( order_id < root->order_id )
        root->left = deleteorder(root->left, order_id);
 
    
    else if( order_id > root->order_id )
        root->right = deleteorder(root->right, order_id);
 
    
    else
    {
        // order with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct order *temp = root->left ? root->left :root->right;
 
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp;
            free(temp);
        }
        else
        {
            
            struct order* temp = minValue_order_history(root->right);
 
            // Copy the inorder successor's data to this order
            root->order_id = temp->order_id;
            strcpy(root->address,temp->address);
            root->cost=temp->cost;
            root->delivery=temp->delivery;
            strcpy(root->menu,temp->menu);
            strcpy(root->name,temp->name);
            root->phone=temp->phone;
            strcpy(root->rest_name,temp->rest_name);
            root->user_id=temp->user_id;
            // Delete the inorder successor
            root->right = deleteorder(root->right, temp->order_id);
        }
    }
 
    
    if (root == NULL)
      return root;
 
    
    root->height = 1 + max(height_order_history(root->left),height_order_history(root->right));
 
    
    int balance = getBalance_order_history(root);
 
    //lefy left
    if (balance > 1 && getBalance_order_history(root->left) >= 0)
        return rightRotate_order_history(root);
 
    // Left Right Case
    if (balance > 1 && getBalance_order_history(root->left) < 0)
    {
        root->left =  leftRotate_order_history(root->left);
        return rightRotate_order_history(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance_order_history(root->right) <= 0)
        return leftRotate_order_history(root);
 
    // Right Left Case
    if (balance < -1 && getBalance_order_history(root->right) > 0)
    {
        root->right = rightRotate_order_history(root->right);
        return leftRotate_order_history(root);
    }
 
    return root;
}

void delivery(int order_id)
{
    struct order* ptr=headorder;
    while(ptr->order_id!=order_id)
    {
        if(ptr->order_id>order_id)
        {
            ptr=ptr->left;
        }
        else
        {
            ptr=ptr->right;
        }
    }
            printf("\nfollowing order is delivered :\n");
            printf("Name - %s\n",ptr->name);
            printf("Address - %s\n",ptr->address);
            printf("Phone - %d\n",ptr->phone);
            printf("Menu - %s\n",ptr->menu);
            printf("Delivered by - %s, ID - %d, Phone - %d\n",ptr->delivery->name,ptr->delivery->id,ptr->delivery->phone);
    (ptr->delivery->commission)+=((ptr->cost)/10);
    ptr->delivery->status=0;
    headorder=deleteorder(headorder,order_id);
}


void cancel(int order_id)
{
    struct order* ptr=headorder;
    while(ptr->order_id!=order_id)
    {
        if(ptr->order_id>order_id)
        {
            ptr=ptr->left;
        }
        else
        {
            ptr=ptr->right;
        }
    }
    ptr->delivery->status=0;
            printf("\nfollowing order is cancelled :\n");
            printf("Name - %s\n",ptr->name);
            printf("Address - %s\n",ptr->address);
            printf("Phone - %d\n",ptr->phone);
            printf("Menu - %s\n",ptr->menu);
            printf("Delivered by - %s, ID - %d, Phone - %d\n",ptr->delivery->name,ptr->delivery->id,ptr->delivery->phone);
    headorder=deleteorder(headorder,order_id);
}



int height_restaurant(struct restaurant *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

struct restaurant* create_restaurant(char name[10],char type[10],char food[10],char address[10])
{
    struct restaurant* newnode = (struct restaurant*)malloc(sizeof(struct restaurant));
    strcpy(newnode->name,name);
    strcpy(newnode->type,type);
    strcpy(newnode->food,food);
    strcpy(newnode->address,address);
    
    newnode->left   = NULL;
    newnode->right  = NULL;
    newnode->height = 1;
    return newnode;
}

struct restaurant* rightRotate_restaurant(struct restaurant*y)
{
    struct restaurant*x = y->left;
    struct restaurant*T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height_restaurant(y->left),height_restaurant(y->right)) + 1;
    x->height = max(height_restaurant(x->left),height_restaurant(x->right)) + 1;
 
    // Return new root
    return x;
}
 

struct restaurant* leftRotate_restaurant(struct restaurant*x)
{
    struct restaurant*y = x->right;
    struct restaurant*T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height_restaurant(x->left),height_restaurant(x->right)) + 1;
    y->height = max(height_restaurant(y->left),height_restaurant(y->right)) + 1;
 
    // Return new root
    return y;
}

int getBalance_restaurant(struct restaurant* N)
{
    if (N == NULL)
        return 0;
    return height_restaurant(N->left) - height_restaurant(N->right);
}

struct restaurant* insert_restaurant(struct restaurant* node,char name[10],char type[10],char food[10],char address[10])
{
    
    if (node == NULL)
        return create_restaurant(name,type,food,address);//works like a constructor
 
    if (strcmp(name,node->name)<0)
        node->left  = insert_restaurant(node->left,name,type,food,address); 

    else if (strcmp(name,node->name)>0)
        node->right = insert_restaurant(node->right,name,type,food,address);
    else 
        return node;
 
    
    node->height = 1 + max(height_restaurant(node->left),height_restaurant(node->right));
 
    
    int balance = getBalance_restaurant(node);
 
    //Left Left
    if (balance > 1 && strcmp(name ,node->left->name)<0)
        return rightRotate_restaurant(node);
 
    // Right Right Case
    if (balance < -1 && strcmp(name , node->right->name)>0)
        return leftRotate_restaurant(node);
 
    // Left Right Case
    if (balance > 1 && strcmp(name , node->left->name)>0)
    {
        node->left =  leftRotate_restaurant(node->left);
        return rightRotate_restaurant(node);
    }
 
    // Right Left Case
    if (balance < -1 && strcmp(name , node->right->name)<0)
    {
        node->right = rightRotate_restaurant(node->right);
        return leftRotate_restaurant(node);
    }
 
    
    return node;
}



void traversal_restaurant_type(struct restaurant *root,char type[])
{
    if(root != NULL)
    {
        traversal_restaurant_type(root->left,type);
        if(strcmp(root->type,type)==0)
        {
            printf("%s \n",root->name);
        }
        traversal_restaurant_type(root->right,type);
    }
}

void traversal_restaurant_category(struct restaurant *root,char food[])
{
    if(root != NULL)
    {
        traversal_restaurant_category(root->left,food);
        if(strcmp(root->food,food)==0)
        {
            printf("%s \n",root->name);
        }
        traversal_restaurant_category(root->right,food);
    }
}

void traversal_restaurant_area(struct restaurant *root,char address[])
{
    if(root != NULL)
    {
        traversal_restaurant_area(root->left,address);
        if(strcmp(root->address,address)==0)
        {
            printf("%s \n",root->name);
        }
        traversal_restaurant_area(root->right,address);
    }
}








int height_agent(struct agent *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

struct agent* create_agent(int id,char name[10],int phone,char area[])
{
    struct agent* newnode = (struct agent*)malloc(sizeof(struct agent));
    newnode->id=id;
    strcpy(newnode->name,name);
    newnode->phone=phone;
    strcpy(newnode->area,area);
    newnode->commission=0;
    newnode->status=0;

    newnode->left   = NULL;
    newnode->right  = NULL;
    newnode->height = 1;
    return newnode;
}

struct agent* rightRotate_agent(struct agent*y)
{
    struct agent*x = y->left;
    struct agent*T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height_agent(y->left),height_agent(y->right)) + 1;
    x->height = max(height_agent(x->left),height_agent(x->right)) + 1;
 
    // Return new root
    return x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct agent* leftRotate_agent(struct agent*x)
{
    struct agent*y = x->right;
    struct agent*T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height_agent(x->left),height_agent(x->right)) + 1;
    y->height = max(height_agent(y->left),height_agent(y->right)) + 1;
 
    // Return new root
    return y;
}

int getBalance_agent(struct agent* N)
{
    if (N == NULL)
        return 0;
    return height_agent(N->left) - height_agent(N->right);
}

struct agent* insert_agent(struct agent* node,int id,char name[10],int phone,char area[])
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return create_agent(id,name,phone,area);
 
    if (id < node->id)
        node->left  = insert_agent(node->left,id,name,phone,area);
    else if (id > node->id)
        node->right = insert_agent(node->right,id,name,phone,area);
    else 
        return node;
 
    
    node->height = 1 + max(height_agent(node->left),height_agent(node->right));
 
    
    int balance = getBalance_agent(node);
 
    // left left
    if (balance > 1 && id < node->id)
        return rightRotate_agent(node);
 
    // Right Right Case
    if (balance < -1 && id > node->id)
        return leftRotate_agent(node);
 
    // Left Right Case
    if (balance > 1 && id > node->id)
    {
        node->left =  leftRotate_agent(node->left);
        return rightRotate_agent(node);
    }
 
    // Right Left Case
    if (balance < -1 && id < node->id)
    {
        node->right = rightRotate_agent(node->right);
        return leftRotate_agent(node);
    }
 
    
    return node;
}

void traversal_agent(struct agent *root)
{
    if(root != NULL)
    {
        traversal_agent(root->left);
        printf("%d %s %d %s %d %d\n",root->id,root->name,root->phone,root->area,root->commission,root->status);
        traversal_agent(root->right);
    }
}

void find_agent(struct agent* root,struct agent** node)
{
    if(root != NULL)
    {
        find_agent(root->left,node);
        if(root->status==0)
        {
            *node=root;
        }
        find_agent(root->right,node);
    }
}


int height_order(struct order *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

struct order* create_order(int uid,const char name[20],const char address[20],int phone,const char menu[20],const char rest_name[20],int x,int cost)
{
    struct order* newnode = (struct order*)malloc(sizeof(struct order));
    strcpy(newnode->name,name);
    strcpy(newnode->address,address);
    newnode->phone=phone;
    newnode->user_id=uid;
    strcpy(newnode->menu,menu);
    strcpy(newnode->rest_name,rest_name);
    newnode->t = time(NULL);
    newnode->cost=cost;

    newnode->left   = NULL;
    newnode->right  = NULL;
    newnode->height = 1;
    
    struct order* ptr=headorder;
    struct user* p1=headuser;
    struct agent* agentptr=NULL;
    
	find_agent(headagent,&agentptr);
    
    

    if(agentptr==NULL)
    {
        printf("Order failed No agent available\n");
        free(newnode);
        return NULL;
    }

    else
    {
        
        newnode->delivery=agentptr;// delivery is an agent pointer within order strture
        newnode->order_id=x;//x is passed as q
        agentptr->status=1;//agent is assigned(busy)
        printf("\nOrder placed successfully\n");
        printf("Name - %s\n",newnode->name);
        printf("Address - %s\n",newnode->address);
        printf("Phone - %d\n",newnode->phone);
        printf("order id - %d\n",newnode->order_id);
        printf("Menu - %s\n",newnode->menu);
        struct tm* date=localtime(&newnode->t);
        printf("Order placed on %s",asctime(date));
        printf("Delivered by - %s, ID - %d, Phone - %d\n",newnode->delivery->name,newnode->delivery->id,newnode->delivery->phone);
        q++;// order id is incremented

        //Adding to user history
        struct order* nnode = (struct order*)malloc(sizeof(struct order));
        strcpy(nnode->name,name);
        strcpy(nnode->menu,menu);
        strcpy(nnode->rest_name,rest_name);
        strcpy(nnode->address,address);
        nnode->order_id=x;
        nnode->t=newnode->t;
        nnode->right=NULL;
        nnode->left=NULL;
        nnode->height=1;
        nnode->user_id=uid;
        struct user* ptr=headuser;
        while(ptr->id !=uid)
        {
            if(ptr->id > uid)
            {
                ptr=ptr->left;
            }
            else
            {
                ptr=ptr->right;
            }
        }
        ptr->next=insert_user_hisory(ptr->next,nnode);


        return newnode;
    }
}

struct order* rightRotate_order(struct order*y)
{
    struct order* x = y->left;
    struct order* T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height_order(y->left),height_order(y->right)) + 1;
    x->height = max(height_order(x->left),height_order(x->right)) + 1;
 
    // Return new root
    return x;
}
 

struct order* leftRotate_order(struct order*x)
{
    struct order*y = x->right;
    struct order*T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height_order(x->left),height_order(x->right)) + 1;
    y->height = max(height_order(y->left),height_order(y->right)) + 1;
 
    // Return new root
    return y;
}

int getBalance_order(struct order* N)
{
    if (N == NULL)
        return 0;
    return height_order(N->left) - height_order(N->right);
}

struct order* insert_order(struct order* node,int uid,const char name[20],const char address[20],int phone,const char menu[20],const char rest_name[20],int x,int cost)
{
    
    if (node == NULL)
    {
        struct order* temp=create_order(uid,name,address,phone,menu,rest_name,x,cost);
        if(temp==NULL)
        {
            return NULL;//NULL will be returned when ther is no af=gent available
        }
        else
        {
            return temp;
        }
    }
    
    if ( x < node->order_id)
        node->left  = insert_order(node->left,uid,name,address,phone,menu,rest_name,x,cost);
    else if (x > node->order_id)
        node->right = insert_order(node->right,uid,name,address,phone,menu,rest_name,x,cost);
    else // Equal names are not allowed in BST
        return node;
 
    
    node->height = 1 + max(height_order(node->left),height_order(node->right));
 
    
    int balance = getBalance_order(node);
 
    
    if (balance > 1 && x < node->order_id)
        return rightRotate_order(node);
 
    
    if (balance < -1 && x > node->order_id)
        return leftRotate_order(node);
 
    
    if (balance > 1 && x > node->order_id)
    {
        node->left =  leftRotate_order(node->left);
        return rightRotate_order(node);
    }
 
    // Right Left Case
    if (balance < -1 && x < node->order_id)
    {
        node->right = rightRotate_order(node->right);
        return leftRotate_order(node);
    }
 
    /* return the (unchanged) node pointer */
    return node;
}

void traversal_order(struct order *root)
{
    if(root != NULL)
    {
        
        traversal_order(root->left);
        struct tm* date=localtime(&root->t);
        printf("%s %s %d %s %s ",root->name,root->address,root->phone,root->menu,root->rest_name);
        printf("Order placed on %s",asctime(date));
        traversal_order(root->right);
    }
}









int height_user(struct user *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

struct user* create_user(int id,char name[20],char address[20],int phone)
{
    struct user* newnode = (struct user*)malloc(sizeof(struct user));
    strcpy(newnode->name,name);
    strcpy(newnode->address,address);
    newnode->phone=phone;
    newnode->id=id;
    newnode->next=NULL;
        
    newnode->left   = NULL;
    newnode->right  = NULL;
    newnode->height = 1;

    return newnode;
}

struct user* rightRotate_user(struct user*y)
{
    struct user* x = y->left;
    struct user* T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height_user(y->left),height_user(y->right)) + 1;
    x->height = max(height_user(x->left),height_user(x->right)) + 1;
 
    // Return new root
    return x;
}
 

struct user* leftRotate_user(struct user*x)
{
    struct user*y = x->right;
    struct user*T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height_user(x->left),height_user(x->right)) + 1;
    y->height = max(height_user(y->left),height_user(y->right)) + 1;
 
    // Return new root
    return y;
}

int getBalance_user(struct user* N)
{
    if (N == NULL)
        return 0;
    return height_user(N->left) - height_user(N->right);
}

struct user* insert_user(struct user* node,int id,char name[20],char address[20],int phone)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
    {
        return create_user(id,name,address,phone);
    }
    
    if ( id < node->id)
        node->left  = insert_user(node->left,id,name,address,phone);
    else if (id > node->id)
        node->right = insert_user(node->right,id,name,address,phone);
    else 
        return node;
 
    
    node->height = 1 + max(height_user(node->left),height_user(node->right));
 
    
    int balance = getBalance_user(node);
 
    
    if (balance > 1 && id < node->id)
        return rightRotate_user(node);
 
    
    if (balance < -1 && id > node->id)
        return leftRotate_user(node);
 
    
    if (balance > 1 && id > node->id)
    {
        node->left =  leftRotate_user(node->left);
        return rightRotate_user(node);
    }
 
    
    if (balance < -1 && id < node->id)
    {
        node->right = rightRotate_user(node->right);
        return leftRotate_user(node);
    }
 
    
    return node;
}

void traversal_user(struct user *root)
{
    if(root != NULL)
    {
        
        traversal_user(root->left);
        
        printf("\n\n%d %s %s %d\n",root->id,root->name,root->address,root->phone);
        traversal_order(root->next);

        traversal_user(root->right);
    }
}














struct favfood
{
    char food[20];
    int frequency;
    char restaurant[20];
    struct favfood* next;
};
struct favfood* fav_food_head=NULL;
struct favfood* fav_restaurant_head=NULL;
struct favfood* fav_food_accross_restaurant_head=NULL;

struct favfood* findmid(struct favfood* ptr)
{
	struct favfood* fast=ptr->next;
	struct favfood* slow=ptr;

	while(fast!=NULL && fast->next!=NULL)
	{
		fast=fast->next->next;
		slow=slow->next;	
	}
	return slow;
}
struct favfood* merge(struct favfood* head1, struct favfood* head2)  
{ 
    struct favfood* head;
    
    if(head1->frequency < head2->frequency)
    {
        head=head2;
        head2=head2->next;
    }
    else
    {
        head=head1;
        head1=head1->next;
    }
    struct favfood* ptr=head;
    
    while(head1!=NULL && head2!=NULL)
    {
        if(head1->frequency < head2->frequency)
        {
            ptr->next=head2;
            ptr=head2;
            head2=head2->next;
        }
        else
        {
            ptr->next=head1;
            ptr=head1;
            head1=head1->next;
        }
    }
    while(head1!=NULL)
    {
        ptr->next=head1;
        ptr=head1;
        head1=head1->next;
    }
    while(head2!=NULL)
    {
        ptr->next=head2;
        ptr=head2;
        head2=head2->next;
    }
    return head;
}

struct favfood* mergesort(struct favfood* ptr)
{
	if(ptr==NULL || ptr->next==NULL)
	{
		return ptr;
	}

	struct favfood* mid=findmid(ptr);

	struct favfood* low=ptr;
	struct favfood* high=mid->next;
	mid->next=NULL;

	high=mergesort(high);
	low=mergesort(low);
	ptr=merge(high,low);

	return ptr;
}






void additems(char item[],time_t t)
{
    time_t present=time(NULL);
    if(present-90*24*60*60 < t)
    {
        if(fav_food_head==NULL)
        {
            struct favfood* favptr=(struct favfood*)malloc(sizeof(struct favfood));
            strcpy(favptr->food,item);
            favptr->frequency=1;
            favptr->next=NULL;
            fav_food_head=favptr;
            
        }
        else
        {
            struct favfood* ptr=fav_food_head;
            struct favfood* prev=NULL;
            while(ptr!=NULL && strcmp(ptr->food,item) !=0)
            {
                prev=ptr;
                ptr=ptr->next;
            }
            if(ptr==NULL)
            {
                struct favfood* favptr=(struct favfood*)malloc(sizeof(struct favfood));
                strcpy(favptr->food,item);
                favptr->frequency=1;
                favptr->next=NULL;
                prev->next=favptr;
            }
            else
            {
                ptr->frequency ++;
            }
        }
    }
}
void traversal_order_fav_food(struct order* ptr)
{
    if(ptr!=NULL)
    {
        traversal_order_fav_food(ptr->left);
        additems(ptr->menu,ptr->t);
        traversal_order_fav_food(ptr->right);
    }
}
void find_favourite_food_user(int user_id)
{
    struct user* ptr=headuser;
    while(ptr->id!=user_id)
    {
        if(ptr->id>user_id)
        {
            ptr=ptr->left;
        }
        else
        {
            ptr=ptr->right;
        }
    }
    fav_food_head=NULL;
    traversal_order_fav_food(ptr->next);
    struct favfood* ptr2=fav_food_head;
    ptr2=mergesort(ptr2);
    int i=1;
    
    while(ptr2!=NULL && i<=3)
    {
        printf("%d : %s --> %d\n",i,ptr2->food,ptr2->frequency);
        ptr2=ptr2->next;
        i++;
    }
    
    fav_food_head=NULL;
}








void addrestaurant(char item[],time_t t,int n)
{
    time_t present=time(NULL);
    if(present-n*24*60*60 < t)
    {
        if(fav_restaurant_head==NULL)
        {
            struct favfood* favptr=(struct favfood*)malloc(sizeof(struct favfood));
            strcpy(favptr->restaurant,item);
            favptr->frequency=1;
            favptr->next=NULL;
            fav_restaurant_head=favptr;
            
        }
        else
        {
            struct favfood* ptr=fav_restaurant_head;
            struct favfood* prev=NULL;
            while(ptr!=NULL && strcmp(ptr->restaurant,item) !=0)
            {
                prev=ptr;
                ptr=ptr->next;
            }
            if(ptr==NULL)
            {
                struct favfood* favptr=(struct favfood*)malloc(sizeof(struct favfood));
                strcpy(favptr->restaurant,item);
                favptr->frequency=1;
                favptr->next=NULL;
                prev->next=favptr;
            }
            else
            {
                ptr->frequency ++;
            }
        }
    }
}

void traversal_order_fav_restaurant(struct order* ptr,int n)
{
    if(ptr!=NULL)
    {
        traversal_order_fav_restaurant(ptr->left,n);
        addrestaurant(ptr->rest_name,ptr->t,n);
        traversal_order_fav_restaurant(ptr->right,n);
    }
}
void f1(struct user* ptr,int n)
{
    if(ptr!=NULL)
    {
        f1(ptr->left,n);
        traversal_order_fav_restaurant(ptr->next,n);
        f1(ptr->right,n);
    }
}
void find_favourite_restaurant(int n)
{
    struct user* ptr=headuser;
    fav_restaurant_head=NULL;
    f1(ptr,n);
    fav_restaurant_head=mergesort(fav_restaurant_head);
    int i=1;
    
    while(fav_restaurant_head!=NULL && i<=3)
    {
        printf("%d : %s --> %d\n",i,fav_restaurant_head->restaurant,fav_restaurant_head->frequency);
        fav_restaurant_head=fav_restaurant_head->next;
        i++;
    }
    
    fav_restaurant_head=NULL;
}




void add_food_accross_restaurant(char item[],time_t t,int n)
{
    time_t present=time(NULL);
    if(present-n*24*60*60 < t)
    {
        if(fav_food_accross_restaurant_head==NULL)
        {
            struct favfood* favptr=(struct favfood*)malloc(sizeof(struct favfood));
            strcpy(favptr->food,item);
            favptr->frequency=1;
            favptr->next=NULL;
            fav_food_accross_restaurant_head=favptr;
            
        }
        else
        {
            struct favfood* ptr=fav_food_accross_restaurant_head;
            struct favfood* prev=NULL;
            while(ptr!=NULL && strcmp(ptr->food,item) !=0)
            {
                prev=ptr;
                ptr=ptr->next;
            }
            if(ptr==NULL)
            {
                struct favfood* favptr=(struct favfood*)malloc(sizeof(struct favfood));
                strcpy(favptr->food,item);
                favptr->frequency=1;
                favptr->next=NULL;
                prev->next=favptr;
            }
            else
            {
                ptr->frequency ++;
            }
        }
    }
}
void traversal_order_fav_food_accross_restaurant(struct order* ptr,int n)
{
    if(ptr!=NULL)
    {
        traversal_order_fav_food_accross_restaurant(ptr->left,n);
        add_food_accross_restaurant(ptr->menu,ptr->t,n);
        traversal_order_fav_food_accross_restaurant(ptr->right,n);
    }
}
void f2(struct user* ptr,int n)
{
    if(ptr!=NULL)
    {
        f2(ptr->left,n);
        traversal_order_fav_food_accross_restaurant(ptr->next,n);//trsverses user orders
        f2(ptr->right,n);
    }
}
void find_favourite_food_accross_restaurant(int n)
{
    struct user* ptr=headuser;
    fav_food_accross_restaurant_head=NULL;
    f2(ptr,n);// trverses across users 
    fav_food_accross_restaurant_head=mergesort(fav_food_accross_restaurant_head);
    int i=1;
    while(fav_food_accross_restaurant_head!=NULL && i<=3)
    {
        printf("%d : %s --> %d\n",i,fav_food_accross_restaurant_head->food,fav_food_accross_restaurant_head->frequency);
        fav_food_accross_restaurant_head=fav_food_accross_restaurant_head->next;
        i++;
    }
    fav_food_accross_restaurant_head=NULL;
}


void traversal_area_agent(struct agent *root,char area[])
{
    if(root != NULL)
    {
        traversal_area_agent(root->left,area);
        if(strcmp(root->area,area)==0)
        {   
            printf("%d %s %d %s %d %d\n",root->id,root->name,root->phone,root->area,root->commission,root->status);
        }
        traversal_area_agent(root->right,area);
    }
}

void get_restaurant_details(struct restaurant *root,char name[],char address[])
{
    if(root != NULL)
    {
        
        get_restaurant_details(root->left,name,address);
        if(strcmp(root->name,name)==0 && strcmp(root->address,address)==0)
        {
            printf("\n%s %s %s %s", root->name,root->type,root->food,root->address);
        }
        
        get_restaurant_details(root->right,name ,address);
    }
}

int main()
{
    char name[20],type[20],food[20],area[20],address[20];
    int i,id,phone;

    FILE * filePointer;

    filePointer = fopen("restaurant.txt","r");
    for(i=0;i<5;i++)
    {
        fscanf(filePointer, "%s %s %s %s",name,type,food,area);
        headrestaurant=insert_restaurant(headrestaurant,name,type,food,area);
    }
    fclose(filePointer);

    filePointer = fopen("agent.txt","r");
    for(i=0;i<5;i++)
    {
        fscanf(filePointer, "%d %s %d %s",&id,name,&phone,area);
        headagent=insert_agent(headagent,id,name,phone,area);
    }
    fclose(filePointer);

    filePointer = fopen("user.txt","r");
    for(i=0;i<5;i++)
    {
        fscanf(filePointer, "%d %s %s %d",&id,name,address,&phone);
        headuser=insert_user(headuser,id,name,address,phone);
    }
    fclose(filePointer);

    headorder=insert_order(headorder,401,"ram","addr1",45547632,"item1","restaurant1",q,500);//q=101        //line 759
    delivery(101);//278
    headorder=insert_order(headorder,403,"roy","addr3",13453100,"item2","restaurant2",q,4000);//q=102
    headorder=insert_order(headorder,401,"ram","addr1",45547632,"item2","restaurant2",q,300);//q=103
    headorder=insert_order(headorder,401,"ram","addr1",45547632,"item3","restaurant3",q,200);//q=104
    headorder=insert_order(headorder,405,"tom","addr5",68345778,"item3","restaurant4",q,800);//q=105
    //headorder=insert_order(headorder,405,"tom","addr5",68345778,"item3","restaurant4",q,500);
    
    cancel(102);//304
    printf("\n");
    printf("\n");
    printf("\n");
    //traversal_user(headuser);
    //traversal_order(headorder);

    printf("Q 1.a \n");//440
    traversal_restaurant_type(headrestaurant,"cafe");

    printf("Q 1.b \n");//
    traversal_restaurant_category(headrestaurant,"northindian");

    printf("Q 1.c \n");//
    traversal_restaurant_area(headrestaurant,"address3");


    printf("Favourate food of any user : \n");
    find_favourite_food_user(401);//1103       passed user id

    printf("Favourate restaurant : \n");
    find_favourite_restaurant(10);//1252        days

    printf("Favourate food accross the restaurants : \n");
    find_favourite_food_accross_restaurant(9);//q7 1330  

    printf("Current Status of agesnts : \n");  
    traversal_agent(headagent);//624

    printf("Live Orders : \n");
    traversal_order(headorder);//846 live orders from global database


    get_restaurant_details(headrestaurant,"restaurant2","address2");//1329
}
