#include<iostream>
#include<string.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define MAX_STR 257


typedef struct _Node {
	struct _Node* right;
	struct _Node* left;
	unsigned long long value;
	int priority;
	char* key;
} node;

typedef pair<node*, node*> Pair;


size_t MyStrlen(const char *s) 
{
    size_t len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}

void MyStrcpy(char *dst, const char *src) 
{
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}

node* Create_tree(char* key, unsigned long long value)
{
	node* tree = (node*)malloc(sizeof(node));
	int len;
	len = MyStrlen(key);
	tree->key = (char*)malloc((len+1)*sizeof(char));
	strcpy(tree->key, key);
	tree->value = value;
	tree->right = NULL;
	tree->left = NULL;
	tree->priority = rand()%1000;
	return tree;

}


node* Search_min(node* tree)
{
	if (tree->left != NULL) {
		tree = Search_min(tree->left);
	}
	return tree;
}
node* Search_max(node* tree)
{
	if (tree->right != NULL) {
		tree = Search_max(tree->right);
	}
	return tree;
}

void Search(node* tree, char* key) {
	if (strcmp(key, tree->key) == 0) {
		cout<<"OK: "<<tree->value<<endl;
		return;
	}
	if (strcmp(key, tree->key) > 0 && tree->right == NULL) {
		cout<<"NoSuchWord"<<endl;
		return;
	}
	if (strcmp(key, tree->key) < 0 && tree->left == NULL) {
		cout<<"NoSuchWord"<<endl;
		return;
	}
	if (strcmp(key, tree->key) > 0 && tree->right != NULL) {
		Search(tree->right, key);
		return;
	}
	if (strcmp(key, tree->key) < 0 && tree->left != NULL) {
		Search(tree->left, key);
		return;
	}

}

Pair TreeSplit(node* tree, char* key)
{
	if (tree == NULL) {
		return {NULL, NULL};
	}
	if (strcmp(key, tree->key) >= 0) {
		Pair res = TreeSplit(tree->right, key);
		tree->right = res.first;
		return {tree, res.second};
	} else {
		Pair res = TreeSplit(tree->left, key);
		tree->left = res.second;
		return {res.first, tree};
	}
}

node* TreeMerge(node* left, node* right)
{
	if (left == NULL) {
		return right;
	}
	if (right == NULL) {
		return left;
	}

	if (left->priority > right->priority) {
		left->right = TreeMerge(left->right, right);
		return left;
	} else {
		right->left = TreeMerge(left, right->left);
		return right;
	}
}

int SearchNoPrint(node* tree, char* key) {
	if (strcmp(key, tree->key) == 0) {
		return 1;
	}
	if (strcmp(key, tree->key) > 0 && tree->right == NULL) {
		return 0;
	}
	if (strcmp(key, tree->key) < 0 && tree->left == NULL) {
		return 0;
	}
	if (strcmp(key, tree->key) > 0 && tree->right != NULL) {
		if (SearchNoPrint(tree->right, key)) {
			return 1;
		}
		return 0;
	}
	if (strcmp(key, tree->key) < 0 && tree->left != NULL) {
		if (SearchNoPrint(tree->left, key)) {
			return 1;
		}
		return 0;
	}
	return 0;
}

void TreeInsert(node* &tree, char* key, unsigned long long value)
{
	//node* ttree = tree;
	Pair save = TreeSplit(tree, key);
	node* new_tree = Create_tree(key, value);
	tree = TreeMerge(TreeMerge(save.first, new_tree), save.second);
}


void TreeDelete(node* &tree, char* key)
{
	if (tree == NULL) {
		return;
	}
	if (strcmp(key, tree->key) == 0) {
		node* save;
		save = tree;
		tree = TreeMerge(tree->left, tree->right);
		free(save->key);
		free(save);
		cout<<"OK"<<endl;
		return;
	}
	if (strcmp(key, tree->key) > 0 && tree->right != NULL) {
		TreeDelete(tree->right, key);
		return;
	}
	if (strcmp(key, tree->key) < 0 && tree->left != NULL) {
		TreeDelete(tree->left, key);
		return;
	}
}

char ToLow(char c)
{
	if (c >= 'A' && c <= 'Z') {
		return (char)(c + 32);
	}
	return c;
}


void Tree_print(node* tree, int n)
{
    if (tree != NULL)
    {
        Tree_print(tree->right, n + 1);
        for (int i = 0; i < n; i++)
            printf("\t");
        printf("%s %d\n", tree->key, tree->priority);
        Tree_print(tree->left, n + 1);
    }

}

int main()
{
	//ios::sync_with_stdio(false);
	unsigned long long number;
	int created = 0;
	char c;
	int end = 0;
	node* my_tree = NULL;
	while((c = getchar()) != EOF) {
		if (end == 1) {
			break;
		}
		char word[258] = {0};
		char word_search[258] = {0};
		unsigned long long number;
		int len;
		int cnt = 0;
		//cout<<"curcom: "<<c<<endl;
		switch(c){
			case '+':
				cin>>word>>number;
				len = MyStrlen(word);
				for (int i = 0; i < len; ++i) {
					word[i] = ToLow(word[i]);
				}
				if (created == 0) {
					created = 1;
					my_tree = Create_tree(word, number);
					cout<<"OK"<<endl;
				} else {
					if (SearchNoPrint(my_tree, word) == 1) {
						cout<<"Exist"<<endl;
					} else {
						TreeInsert(my_tree, word, number);
						cout<<"OK"<<endl;
					}
				}
				//Tree_print(my_tree,0);
				c = getchar();
				break;
			case '-':
				cin>>word;
				len = MyStrlen(word);
				for (int i = 0; i < len; ++i) {
					word[i] = ToLow(word[i]);
				}
				if (created == 0) {
					cout<<"NoSuchWord"<<endl;
					c = getchar();
					break;
				}
				if (my_tree->right == NULL && my_tree->left == NULL && (strcmp(my_tree->key,word) == 0)) {
					created = 0;
					cout<<"OK"<<endl;
					free(my_tree->key);
					free(my_tree);
					my_tree = NULL;
					c = getchar();
					break;
				}
				if (created == 0) {
					cout<<"NoSuchWord"<<endl;
					c = getchar();
					break;
				}
				if (SearchNoPrint(my_tree, word) == 0) {
					cout<<"NoSuchWord"<<endl;
					c = getchar();
					break;
				}
				TreeDelete(my_tree, word);
				c = getchar();
				//Tree_print(my_tree,0);
				break;
			// case '*':
			// 	end = 1;
			// 	break;
			default:
				if (c != '+' && c != '-') {
					word_search[cnt] = ToLow(c);
					char b;
					while((b = getchar())) {
						if (b == '\n' || b == EOF) {
							break;
						}
						cnt++;
						word_search[cnt] = ToLow(b);
					}
					if (created != 0) {
						Search(my_tree, word_search);
					} else {
						cout<<"NoSuchWord"<<endl;
					}
					
					break;
				}
		}
	}
	// free(my_tree);
}