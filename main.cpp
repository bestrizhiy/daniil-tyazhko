#include"RBTree.h"
#include<iostream>
#include<vector>
#include <fstream>

using namespace std;


int main() {

    RBTree<int> tree;

    while (true) {
        cout << "1.Insert\n2.Remove\n3.Find\n4.Print\n9.Quit\n";
        string s;
        getline(cin, s);
        switch (s[0]) {
            case '1':
                cout << "1. Console\n2.File\n";
                getline(cin, s);
                if (s[0] == '1') {
                    getline(cin, s);
                    int a = 0;
                    try {
                        a = std::stoi(s);
                    } catch (...) {
                        cout << "Number is required" << endl;
                        break;
                    }
                    tree.insert(a);
                } else {
                    cout << "Filename:\n";
                    getline(cin, s);
                    ifstream ifs(s);
                    if (ifs.is_open()) {
                        ifs >> tree;
                        ifs.close();
                    } else {
                        cout << "Cant read from this file\n";
                    }
                }
                break;
            case '2': {
                getline(cin, s);
                int a;
                try {
                    a = std::stoi(s);
                } catch (...) {
                    cout << "Number is required" << endl;
                    break;
                }
                tree.remove(a);
            }
                break;
            case '3': {
                getline(cin, s);
                int a;
                try {
                    a = std::stoi(s);
                } catch (...) {
                    cout << "Number is required" << endl;
                    break;
                }
                auto n = tree.search(a);
                if (n) {
                    cout << "Found\n";
                    if (n->parent)
                        cout << "parent: " << n->parent->key << " ";
                    if (n->left)
                        cout << "left: " << n->left->key << " ";
                    if (n->right)
                        cout << "right: " << n->right->key;
                    cout << "\n";
                }
                break;
            }
            case '4':
                cout << "1. Console\n2.File\n";
                getline(cin, s);
                if (s[0] == '1') {
                    cout << tree << endl;
                } else {
                    cout << "Filename:\n";
                    getline(cin, s);
                    ofstream ofs(s);
                    if (ofs.is_open()) {
                        ofs << tree;
                        ofs.close();
                    } else {
                        cout << "Cant write to this file\n";
                    }
                }
                break;
            case '9':
                return 0;
            default:
                break;
        }
    }

//    string line;
//    ifstream ifs("in.txt");
//    ifs >> tree;
//    ifs.close();
//
//    ofstream ofs("out.txt");
//    ofs << tree;
//    ofs.close();
//
//    cout << tree << endl;
//
//    auto t = tree.search(4);
//    cout << "4th parent: " << t->parent << endl;
//
//    tree.remove(4);
//
//    cout << tree << endl;
//
//    RBTree<std::string> tree2;
//
//    tree2.insert("1");
//    tree2.insert("2");
//    tree2.insert("3");
//
//    tree2.inOrder();

    return 0;
}