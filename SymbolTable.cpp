#include <iostream>
#include <bits/stdc++.h>
#include <regex>

using namespace std;

typedef enum Type {INT, FLOAT, DOUBLE, BOOL} Type;
typedef enum Kind {VARIABLE, FUNCTION, PARAMETER} Kind;

typedef struct _Node Node;
typedef Node SymbolTable;

typedef struct Entry
{
    string name;
    string type;
    string kind; 
} Entry;

typedef struct _Node
{
    vector<Entry*> entries;
    vector<Node*> childs;
    Node *parent;
} Node;

void print_help()
{
    cout<< "1 " <<"Enter a new Entry"<<endl;
    cout<< "2 " <<"Lookup an Entry"<<endl;
    cout<< "3 " <<"Enter new scope"<<endl;
    cout<< "4 " <<"Exit scope"<<endl;
    cout<< "5 " <<"Exit"<<endl;;
}

int get_choice()
{
    int choice;
    cout<<"Enter your choice ";
    cin>>choice;
    
    return choice;
}

void insert (Node  *node)
{
    Entry *entry;
    string name1,type1,kind1;
    
    entry = (Entry*) malloc(sizeof(Entry));
    
    cout<< "Enter name: ";
    cin>>name1;
    entry->name = name1;
    
    cout<< "Enter type: ";
    cin>>type1;
    entry->type = type1;
    
    cout<< "Enter kind: ";
    cin>>kind1;
    entry->kind = kind1;
    
    node->entries.push_back(entry);
}

Node* enter_scope(Node *node)
{
    Node *scope = (Node*) malloc(sizeof(Node));
        
    scope->parent = node;
    node->childs.push_back(scope);
    
    return scope;
}

Node* exit_scope(Node *node)
{
    if (!node || !node->parent)
        return NULL;
    
    return node->parent;
}

int lookup (SymbolTable *table,
             string       key)
{
    int sz;
    Node *parent;
    
    if (table == NULL)
        return 0;
        
    parent = table->parent;
    
    if (key == "")
    {
        cout<<"Enter name of entry to be searched: ";
        cin>>key;
    }   
    
    sz = table->entries.size();
    
    for (int i = 0; i < sz; i++)
    {
        Entry *entry;
        entry = table->entries[i];
        
        //cout<<"name: "<<entry->name<<" type: "<<entry->type<<" kind "<<entry->kind<<endl;
        if (entry->name == key)
        {
            cout<<"name: "<<key<<" type: "<<entry->type<<" kind "<<entry->kind<<endl;;
            return 1;
        }
    }
    
    for (int i = 0; i <  parent->childs.size(); i++)
    {
        if (lookup (parent->childs[i], key))
            return 1;
    }
    
    return lookup(table->parent, key);
}

int main()
{
    SymbolTable *root;
    Node *curr_scope;
    int choice;
    
    root = (SymbolTable*) malloc(sizeof(SymbolTable));
    curr_scope = root;
    
    choice = -1;
    print_help();
    while (1)
    {
        choice = get_choice();
        if (choice == 5)
        {
            cout<<"quit"<<endl;
            break;
        }
        
        switch (choice)
        {
            case 1:
                insert (curr_scope);
                break;
                
            case 2:
                lookup(curr_scope, "");
                break;
                
            case 3:
            
                curr_scope = enter_scope (curr_scope);
                break;
            
            case 4:
                curr_scope = exit_scope (curr_scope);
                
                if (curr_scope == NULL)
                    curr_scope = root;
                    
                break;
        }
    }
}
