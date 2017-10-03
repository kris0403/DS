#include<iostream>
using namespace std;

class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);

private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();

private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s
	node* n = new node(s);
	if(root == NULL){
		root = n;
		head->right = root;
		tail->left = root;	
		root->left= head;
		root->right = tail;	
	}
	else {
		bool check=true;
		node* temp = root;
		while(check){
			if(temp->number < s && temp->is_threadr == 0) temp = temp->right;
			else if(temp->number > s && temp->is_threadl == 0)temp = temp->left;
			else check = false;
		}
		if(temp->number < s){
			if(temp == tail->left){
				tail->left = n;
			}
			n->right = temp->right;	//temp->right must be used  
			temp->right = n ;
			temp->is_threadr = 0;
			n->is_threadl = 1;
			n->is_threadr = 1;
			n->left = temp;			
		}
		else{
			if(temp == head->right){
				head->right = n;
			}
			n->left = temp->left;
			temp->left = n;
			temp->is_threadl = 0;
			n->is_threadl = 1;
			n->is_threadr = 1;
			n->right = temp;
		}	
	}
	num++;
}

void op_tree_totum::deletion(int s){
	//TODO: fill in the code to do the deletion of the node with number s
	if(root == NULL) return;
		bool check=true;
		node* temp = root;
		node* previous = NULL;
		while(check){
			if(temp->number < s && temp->is_threadr == 0){
				previous = temp;
				temp = temp->right;
			}
			else if(temp->number > s && temp->is_threadl == 0){
				previous = temp;
				temp = temp->left;
			}
			else if(temp->number == s)check = false;
			else check = false;
		}		
		if(temp->number != s)return ;

		if(temp->is_threadr == 1 && temp->is_threadl == 1){ //the root has no child, ok
			if(temp == root){
				
				tail->left = head;
				head->right= tail;
				root = NULL;
				delete temp;
			}
			else{
				
				if(previous->right == temp){
		
					previous->right = temp->right;
					previous->is_threadr = 1;
					//if(tail->left == temp) tail->left = previous;
				}
				else{
			
					previous->left = temp->left;
					previous->is_threadl = 1;
					//if(head->right == temp) head->right = previous;
				}
	
				node* smallest = root;
				while(smallest->is_threadl == 0){
					smallest = smallest->left;
				}
				head->right = smallest;
				
				node* biggest = root;
				while(biggest->is_threadr == 0){
					biggest = biggest->right;
				}
				tail->left = biggest;
				delete temp;
			}
		}
		else if(temp->is_threadr == 0 && temp->is_threadl == 1){ // the child grow right, ok
			if(temp == root){
				root = temp->right;
				node* small = root;
				while(small->is_threadl == 0){
					small = small->left;
				}
				head->right = small;
				small->left = head;
				delete temp;
			}
			else{
				node* small = temp->right;
				while(small->is_threadl==0){
					small = small->left;
				}
				small->left = previous;
				if(previous->right == temp){
					previous->right = temp->right;
				}
				else if(previous->left == temp){
					previous->left = temp->right;
					node* smallest = root;
					while(smallest->is_threadl == 0) smallest = smallest->left;
					head->right = smallest;
					smallest->left = head;
				}
				delete temp;
			}
		}
		else if(temp->is_threadr == 1 && temp->is_threadl == 0){ //the child grow left, ok
			if(temp == root){
				root = temp->left;
				node* big = root;
				while(big->is_threadr == 0){
					big = big->right;
				}
				tail->left = big;
				big->right = tail;
				delete temp;
			}
			else{
				node* big = temp->left;
				while(big->is_threadr == 0){
					big = big->right;
				}
				big->right = previous;
				if(previous->left == temp){
					previous->left = temp->left;
				}
				else if(previous->right == temp){
					previous->right = temp->right;
					node* biggest = root;
					while(biggest->is_threadr == 0){
						biggest = biggest->right;
					}
					tail->left = biggest;
					biggest->right = tail;
				}
				delete temp;
			}
		}
		else if(temp->is_threadr == 0 && temp->is_threadl == 0){ //the root have two children
			node* small = temp->right;
			node* small_pre = temp;
			while(small->is_threadl == 0){
				small_pre = small;
				small = small->left;
			}
			temp->number = small->number;
			if(small->is_threadr == 1){ //small has no child
				if(small_pre->right == small){
					small_pre->right = small->right;
					small_pre->is_threadr = 1;
				}
				else{
					small_pre->left = small->left;
					small_pre->is_threadl = 1;
				}
				delete small;
			}
			else{ //small has right child
				node* small_small = small->right;
				while(small_small->is_threadl == 0) small_small = small_small->left;
				small_small->left = small_pre;
				small_pre->left = small->right;
				delete small;
			}
			node* smallnew = root;
			while(smallnew->is_threadl != 1){
				smallnew = smallnew->left;
			}
			head->right = smallnew;
			smallnew->left = head;
			node* big = root;
			while(big->is_threadr != 1){
				big = big->right;
			}
			tail->left = big;
			big->right = tail;
		}
		num--;
		
}

void op_tree_totum::inorder_run(){
	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recurtion 
	if(head->right == tail){
		return ;
	}
	node *n = head->right;
	
	while(n != tail->left){
		cout<<n->number<<" ";
		if(n->is_threadr == 1){
			n = n->right;
		}
		else if(n->is_threadr == 0){
			n = n->right;
			while(n->is_threadl != 1){
				n = n->left;
			}
		}		
	}
	cout<<n->number;	
}

void op_tree_totum::reverseorder_run(){
	//TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recurtion 
	if(head->right == tail){
		return ;
	}
	node *n = tail->left;
	
	while(n != head->right){
		cout<<n->number<<" ";
		if(n->is_threadl == 1){
			n = n->left;
		}
		else if(n->is_threadl == 0){
			n = n->left;
			while(n->is_threadr != 1){
				n = n->right;
			}
		}		
	}
	cout<<n->number;		
}

int op_tree_totum::size(){
	return num;
}
