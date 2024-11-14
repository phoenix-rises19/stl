#pragma once

#include <stack>
#include <stdexcept>

namespace vai {

    template <typename KeyType, typename ValueType>
    class Map {
    private:
        struct Node {
            KeyType key_;
            ValueType value_;
            Node* left_;
            Node* right_;
            Node* parent_;
            size_t height_;

            Node(KeyType& k, ValueType& v):
                key_(k),
                value_(v),
                left_(nullptr),
                right_(nullptr),
                parent_(nullptr),
                height_(1) {}
        };

        Node* root_;
        size_t size_;

        int getHeight(Node* node) {
            if (node == nullptr)
                return 0;
            return node->height_;
        }

        int getBalance(Node* node) {
            return getHeight(node->left_) - getHeight(node->right_);
        }

        Node* rightRotate(Node* node) {
            Node* newRoot = node->left_;
            Node* subTree = newRoot->right_;

            node->left_     = subTree;
            newRoot->right_ = node;

            newRoot->height_ = 1 + std::max(getHeight(newRoot->left_), getHeight(newRoot->right_));
            node->height_    = 1 + std::max(getHeight(node->left_), getHeight(node->right_));

            newRoot->parent_ = node->parent_;
            node->parent_    = newRoot;
            if (subTree != nullptr) {
                subTree->parent_ = node;
            }

            return newRoot;
        }

        Node* leftRotate(Node* node) {
            Node* newRoot = node->right_;
            Node* subTree = newRoot->left_;

            newRoot->left_ = node;
            node->right_   = subTree;

            newRoot->height_ = 1 + std::max(getHeight(newRoot->left_), getHeight(newRoot->right_));
            node->height_    = 1 + std::max(getHeight(node->left_), getHeight(node->right_));

            newRoot->parent_ = node->parent_;
            node->parent_    = newRoot;
            if (subTree != nullptr) {
                subTree->parent_ = node;
            }

            return newRoot;
        }

        Node* insert(Node* node, KeyType k, ValueType v, Node* parent = nullptr) {
            if (node == nullptr) {
                Node* newNode    = new Node(k, v);
                newNode->parent_ = parent;
                return newNode;
            }
            if (node->key_ > k) {
                node->left_ = insert(node->left_, k, v, node);
            } else if (node->key_ < k) {
                node->right_ = insert(node->right_, k, v, node);
            } else {
                node->value_ = v;
                return node;
            }
            node->height_ = 1 + std::max(getHeight(node->left_), getHeight(node->right_));
            int balance   = getBalance(node);

            if (balance > 1 and k < node->left_->key_) {
                return rightRotate(node);
            }
            if (balance < -1 and k > node->right_->key_) {
                return leftRotate(node);
            }
            if (balance < -1 and k < node->right_->key_) {
                node->right_ = rightRotate(node->right_);
                return leftRotate(node);
            }
            if (balance > 1 and k > node->left_->key_) {
                node->left_ = leftRotate(node->left_);
                return rightRotate(node);
            }
            return node;
        }

        Node* minValueNode(Node* node) {
            Node* current = node;
            while (current->left_) {
                current = current->left_;
            }
            return current;
        }

        Node* find(Node* node, const KeyType& k) {
            if (!node)
                return nullptr;
            if (k < node->key_) {
                return find(node->left_, k);
            } else if (k > node->key_) {
                return find(node->right_, k);
            } else {
                return node;
            }
        }

    public:
        Map():
            root_(nullptr),
            size_(0) {}

        ~Map() {
            // TODO: destroy nodes
        }

        Map(KeyType& k, ValueType& v):
            root_(new Node(k, v)) {}

        void insert(KeyType k, ValueType v) {
            ++size_;
            root_ = insert(root_, k, v);
        }

        bool empty() {
            return size_ == 0;
        }

        void erase(const KeyType& k) {
            root_ = erase(root_, k);
        }

        size_t size() {
            return size_;
        }

        ValueType& at(const KeyType& k) {
            Node* p = find(root_, k);
            if (!p) {
                throw std::out_of_range("Key not found");
            }
            return p->value_;
        }

        bool find(const KeyType& k) {
            Node* node = find(root_, k);
            if (node) {
                return true;
            }
            return false;
        }

        bool isBalanced(Node* node) {
            if (!node)
                return true;
            int balance = getBalance(node);
            return abs(balance) <= 1 and isBalanced(node->left_) and isBalanced(node->right_);
        }

        bool isBalance() {
            return isBalanced(root_);
        }

        class Iterator {
        private:
            Node* current;

        public:

            //TODO copy assign operator for iterator
            // TODO erase both via key and iterator  
            // TODO find and insert do via while loop.
            Iterator():
                current(nullptr) {}

            Iterator(Node* node):
                current(node) {}

            ValueType& operator*() const {
                return current->value_;
            }

            bool operator==(const Iterator& other) const {
                return current == other.current;
            }

            Iterator& operator++() {
                if (current->right_) {
                    current = current->right_;
                    while (current->left_) {
                        current = current->left_;
                    }
                } else {

                    Node* parent = current->parent_;
                    while (parent and current == parent->right_) {
                        current = parent;
                        parent  = parent->parent_;
                    }
                    current = parent;
                }
                return *this;
            }

            auto* operator->() {
                return new std::pair<const KeyType, ValueType>(current->key_, current->value_);
            }
        };

        Iterator begin() {
            Node* current = root_;
            while (current->left_) {
                current = current->left_;
            }
            return Iterator(current);
        }

        Iterator end() {
            // Node* current = root_;
            // while(current->right_) {
            //     current=current->right_;
            // }
            return Iterator();
        }

        Iterator rbegin() {
            return end();
        }

        Iterator rend() {
            return begin();
        }
    };
}
