//
// Created by Dmitry on 26.01.2021.
//

#ifndef UNTITLED61_NODE_H
#define UNTITLED61_NODE_H

enum Color {
    BLACK, RED
};

template<class KeyType>
struct Node {
    KeyType key;
    Color color;
    Node<KeyType> *left;
    Node<KeyType> *right;
    Node<KeyType> *parent;

    Node(KeyType k, Color c, Node *p, Node *l, Node *r) :
            key(k), color(c), parent(p), left(l), right(r) {};
};


#endif //UNTITLED61_NODE_H
