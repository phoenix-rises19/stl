#pragma once

#include <stack>
#include <stdexcept>
#include <utility>

namespace vai {

    template <typename KeyType, typename ValueType>
    class Map {
        public:
        struct Node {
        public:
            KeyType key_;
            ValueType value_;
            Node* left_;
            Node* right_;
            Node* parent_;
            size_t height_;

            Node(KeyType k, ValueType v):
                key_(k),
                value_(v),
                left_(nullptr),
                right_(nullptr),
                parent_(nullptr),
                height_(1) {}
        };
    private:
        Node* root_;
        size_t size_;

    public:

        class Iterator {
            Node* current;

        public:

            //TODO copy assign operator for iterator
            // TODO erase both via key and iterator
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

    private:

        int getHeight(Node* node) {
            if (node == nullptr)
                return 0;
            return node->height_;
        }

        int getBalance(Node* node) {
            return getHeight(node->left_) - getHeight(node->right_);
        }

        void rightRotate(Node* node) {
            Node* newRoot = node->left_;
            Node* subTree = newRoot->right_;
            Node* oldParent=node->parent_;

            newRoot->right_ = node;
            node->left_     = subTree;

            node->parent_    = newRoot;
            newRoot->parent_ = oldParent;
            if (subTree != nullptr) {
                subTree->parent_ = node;
            }

            if (oldParent) {
                if (oldParent->left_ == node) {
                    oldParent->left_ = newRoot;
                } else {
                    oldParent->right_ = newRoot;
                }
            }
            else {
                root_=newRoot;
            }



            newRoot->height_ = 1 + std::max(getHeight(newRoot->left_), getHeight(newRoot->right_));
            node->height_    = 1 + std::max(getHeight(node->left_), getHeight(node->right_));
        }

        void leftRotate(Node* node) {
            Node* newRoot = node->right_;
            Node* subTree = newRoot->left_;
            Node* oldParent=node->parent_;


            newRoot->left_ = node;
            node->right_   = subTree;

            node->parent_    = newRoot;
            newRoot->parent_ = oldParent;
            if (subTree != nullptr) {
                subTree->parent_ = node;
            }

            if (oldParent) {
                if (oldParent->left_ == node) {
                    oldParent->left_ = newRoot;
                } else {
                    oldParent->right_ = newRoot;
                }
            }
            else {
                root_=newRoot;
            }

            newRoot->height_ = 1 + std::max(getHeight(newRoot->left_), getHeight(newRoot->right_));
            node->height_    = 1 + std::max(getHeight(node->left_), getHeight(node->right_));
        }

        std::pair<Iterator,bool> insert_(const KeyType& k, const ValueType& v) {
            Node* current=root_;
            Node* parent=nullptr;


            bool flag=false;
            while(current) {
                if (current->key_ == k) {
                    current->value_ = v;
                    return std::make_pair(Iterator(current),false);
                }
                else if (current->key_ > k) {
                    parent= current;
                    current=current->left_;
                    flag=false;
                }
                else if(current->key_ < k) {
                    parent= current;
                    current=current->right_;
                    flag=true;
                }
            }
            Node* newNode = new Node(k,v);
            if(root_==nullptr) {
                root_=newNode;
                return std::make_pair(Iterator(newNode),true);
            }
            newNode->parent_ = parent;
            if (flag) {
                parent->right_ = newNode;
            } else {
                parent->left_ = newNode;
            }

            flag=true;
            while(parent and flag) {
                parent->height_ = 1 + std::max(getHeight(parent->left_), getHeight(parent->right_));
                int balance   = getBalance(parent);

                if(abs(balance)>1) {
                    flag=false;
                    if (balance > 1 and k < parent->left_->key_) {
                        rightRotate(parent);
                    }
                    else if (balance < -1 and k > parent->right_->key_) {
                        leftRotate(parent);
                    }
                    else if (balance < -1 and k < parent->right_->key_) {
                        rightRotate(parent->right_);
                        leftRotate(parent);
                    }
                    else if (balance > 1 and k > parent->left_->key_) {
                        leftRotate(parent->left_);
                        rightRotate(parent);
                    }
                }
                parent=parent->parent_;
            }
            return std::make_pair(Iterator(newNode),true);
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
            Node* current = node;
            while(current) {
                if (k < node->key_) {
                    current=current->left_;
                } else if (k > node->key_) {
                    current=current->right_;
                } else {
                    return node;
                }
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

        std::pair<Iterator,bool> insert(KeyType k, ValueType v) {
            ++size_;
            return insert_(k, v);
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
