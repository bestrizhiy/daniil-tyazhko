#ifndef UNTITLED61_RBTREE_H
#define UNTITLED61_RBTREE_H

#include"Node.h"
#include <iomanip>
#include <iostream>

using namespace std;

template<class T>
class RBTree {
public:
    RBTree();

    ~RBTree();

    void insert(T key);

    void remove(T key);

    Node<T> *search(T key);

    void inOrder() const;

    friend ostream &operator<<(ostream &os, const RBTree<T> &t) {
        t.inOrder(t.root, os);
        return os;
    }

    friend istream &operator>>(istream &is, RBTree<T> &t) {
        std::string line;
        T a;
        while (is >> a) {
            t.insert(a);
        }
        return is;
    }

private:
    void leftRotate(Node<T> *&root, Node<T> *x);

    void rightRotate(Node<T> *&node, Node<T> *y);

    void insert(Node<T> *&root, Node<T> *node);

    void insertFix(Node<T> *&root, Node<T> *node);

    void destory(Node<T> *&node);

    void remove(Node<T> *&root, Node<T> *node);

    void removeFix(Node<T> *&root, Node<T> *node, Node<T> *parent);

    Node<T> *search(Node<T> *node, T key) const;

    void inOrder(Node<T> *tree, ostream &os = cout) const;

private:
    Node<T> *root;
};

template<class T>
RBTree<T>::RBTree() :root(nullptr) {
    root = nullptr;
}

template<class T>
RBTree<T>::~RBTree() {
    destory(root);
}

template<class T>
void RBTree<T>::leftRotate(Node<T> *&node, Node<T> *x) {

    Node<T> *y = x->right;

    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        node = y;
    else {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
};

template<class T>
void RBTree<T>::rightRotate(Node<T> *&node, Node<T> *y) {
    Node<T> *x = y->left;
    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == nullptr)
        node = x;
    else {
        if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
};

template<class T>
void RBTree<T>::insert(T key) {
    auto *a = new Node<T>(key, RED, nullptr, nullptr, nullptr);
    insert(root, a);
};

template<class T>
void RBTree<T>::insert(Node<T> *&root, Node<T> *node) {
    Node<T> *x = root;
    Node<T> *y = nullptr;
    //Поиск нужной вершины для вставки
    while (x != nullptr) {
        y = x;
        if (node->key > x->key)
            x = x->right;
        else
            x = x->left;
    }
    //вставка
    node->parent = y;
    if (y != nullptr) {
        if (node->key > y->key)
            y->right = node;
        else
            y->left = node;
    } else
        root = node;
    node->color = RED;
    //балансировка
    insertFix(root, node);
};

template<class T>
void RBTree<T>::insertFix(Node<T> *&root, Node<T> *node) {
    Node<T> *parent;
    parent = node->parent;
    //Если отец черный - то норм т.к новый элемент красный
    while (node != RBTree::root && parent->color == RED) {
        Node<T> *gparent = parent->parent;
        //Рассмотрим два вариатна: отец - левый ребенок и отец - правый ребенок
        //Они отличаются направлением поворота
        if (gparent->left == parent) {
            Node<T> *uncle = gparent->right;
            //Если отец и дядя - красные - перекрашиваем и проверяем снова (в цикле)
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                node = gparent;
                parent = node->parent;
            } else {
                //Иначе - делаем новый элемент левым ребенком (если он не был им)
                if (parent->right == node) {
                    leftRotate(root, parent);
                    swap(node, parent);
                }
                //И выполняем поворот
                rightRotate(root, gparent);
                gparent->color = RED;
                parent->color = BLACK;
                break;
            }
        } else {
            //Аналогично
            Node<T> *uncle = gparent->left;
            if (uncle != nullptr && uncle->color == RED) {
                gparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = gparent;
                parent = node->parent;
            } else {
                if (parent->left == node) {
                    rightRotate(root, parent);
                    swap(parent, node);
                }
                leftRotate(root, gparent);
                parent->color = BLACK;
                gparent->color = RED;
                break;
            }
        }
    }
    root->color = BLACK;
}

template<class T>
void RBTree<T>::destory(Node<T> *&node) {
    //Рекурсивное удаление дерева
    if (node == nullptr)
        return;
    destory(node->left);
    destory(node->right);
    delete node;
    node = nullptr;
}

template<class T>
void RBTree<T>::remove(T key) {
    //Удаление врещины с заданным значением
    Node<T> *deletenode = search(root, key);
    if (deletenode != nullptr)
        remove(root, deletenode);
}

template<class T>
void RBTree<T>::remove(Node<T> *&root, Node<T> *node) {
    Node<T> *child, *parent;
    Color color;
    //Если удаляется не лист
    if (node->left != nullptr && node->right != nullptr) {
        Node<T> *replace = node;
        //Удаляемая вершина будет заменена на меньшую вершину из правого поддерева,
        //найдем её
        replace = node->right;
        while (replace->left != nullptr) {
            replace = replace->left;
        }
        //Если есть родитель - переопределим
        if (node->parent != nullptr) {
            if (node->parent->left == node)
                node->parent->left = replace;
            else
                node->parent->right = replace;
        }
            //если нет родителя - переопределим корень
        else
            root = replace;

        parent = replace->parent;
        color = replace->color;

        //Единственный возможный ребенок этой новой вершины (replace) это правый, запомним его
        child = replace->right;


        if (parent == node)
            parent = replace;
        else {
            //Если ребенок у replace все же существует, переопределяем ему родителя
            if (child != nullptr)
                child->parent = parent;

            parent->left = child;
            replace->right = node->right;
            node->right->parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if (color == BLACK)
            removeFix(root, child, parent);

        delete node;
        return;
    }
    //Здесь у удаляемой верщины осталось не больше одного ребенка
    //Ставим ребенка на место удаляемой верщины
    if (node->left != nullptr)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    color = node->color;
    if (child) {
        child->parent = parent;
    }
    //Устанавливаем родителю этого нового ребенка
    if (parent) {
        if (node == parent->left)
            parent->left = child;
        else
            parent->right = child;
    }
        //Или если родителя нет - значит ребенок - новый корень
    else
        RBTree::root = child;

    if (color == BLACK) {
        removeFix(root, child, parent);
    }
    delete node;

}

template<class T>
void RBTree<T>::removeFix(Node<T> *&root, Node<T> *node, Node<T> *parent) {
    Node<T> *uncle;
    while ((!node) || (node->color == BLACK && node != RBTree::root)) {
        //Делим возможную ситуацию на 2 части - новая вершина это левый ребенок или правый ребенок
        //Отличаются эти случаи направлением поворота
        //Если левый
        if (parent->left == node) {
            uncle = parent->right;
            //Теперь смотрим на цвет дяди (он правый ребенок). Если он красный - перекрашиваем его и родителя в черный
            // (новый элемент всегда красный) и делаем левый поворот
            if (uncle->color == RED) {
                uncle->color = BLACK;
                parent->color = RED;
                leftRotate(root, parent);
                uncle = parent->right;
            }
            if ((!uncle->left || uncle->left->color == BLACK) && (!uncle->right || uncle->right->color == BLACK)) {
                uncle->color = RED;
                node = parent;
            } else {
                //Если дядя черный
                //Если у дяди нет правого ребенка или он черный
                //Перекрашиваем дядю и ребенка и делаем правый поворот вокруг дяди и предка
                if (!(uncle->right) || uncle->right->color == BLACK) {
                    uncle->left->color = BLACK;
                    uncle->color = RED;
                    rightRotate(root, uncle);
                    uncle = parent->right;
                }
                //Тут у дяди правый ребенок - красный
                //Перекрашиваем его в цвет отца, остальных - в черный
                //Поворот и алгоритм завершен
                uncle->color = parent->color;
                parent->color = BLACK;
                uncle->right->color = BLACK;
                leftRotate(root, parent);
                node = root;
                break;
            }
        } else {
            uncle = parent->left;
            if (uncle->color == RED) {
                uncle->color = BLACK;
                parent->color = RED;
                rightRotate(root, parent);
                uncle = parent->left;
            }
            if ((!uncle->left || uncle->left->color == BLACK) &&
                (!uncle->right || uncle->right->color == BLACK)) {
                uncle->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                if (!(uncle->left) || uncle->left->color == BLACK) {
                    uncle->right->color = BLACK;
                    uncle->color = RED;
                    leftRotate(root, uncle);
                    uncle = parent->left;
                }
                uncle->color = parent->color;
                parent->color = BLACK;
                uncle->left->color = BLACK;
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    //Наконец только что вставленное значение является корнем поддерева (или дерева)
    //Красим в черный
    if (node)
        node->color = BLACK;
}

template<class T>
Node<T> *RBTree<T>::search(T key) {
    return search(root, key);
}

template<class T>
Node<T> *RBTree<T>::search(Node<T> *node, T key) const {
    //Если значение найдено или достигнут null (т.е оно не существует) аозвращаем вершину
    if (node == nullptr || node->key == key)
        return node;
        //Если искомое значение больше - ищем в правом поддереве
    else if (key > node->key)
        return search(node->right, key);
        //Иначе - в левом
    else
        return search(node->left, key);
}

template<class T>
void RBTree<T>::inOrder() const {
    if (root == nullptr)
        cout << "Дерево не существует" << endl;
    else
        inOrder(root, cout);
};

template<class T>
void RBTree<T>::inOrder(Node<T> *tree, ostream &os) const {
    if (tree != nullptr) {
        inOrder(tree->left, os);
        os << tree->key << " ";
        inOrder(tree->right, os);
    }
}

#endif
