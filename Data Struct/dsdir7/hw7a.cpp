#include "bt0.h"
int main(){
    Tree<double> tree;
    double dval;

    cout << "Enter Values: \n";
    while(cin >> dval)
        tree.Insert(dval);
    cout << endl << "Preorder traversal: ";
    tree.Preorder();
    cout << endl << "Inorder traversal: ";
    tree.Inorder();
    cout << endl << "Postorder traversal: ";
    tree.Postorder();
    cout << endl << "Levelorder traversal: ";
    tree.Levelorder();
    cout << endl;
}
