//
// Created by vaibhav on 11/8/24.
//

#ifndef MAP_HPP
#define MAP_HPP
#include <stdexcept>

namespace vai {
    template<typename KeyType, typename ValueType>
    class Map {
    public:
        Map():
            root_(nullptr){}

        Map(const Map<KeyType, ValueType>& other): root_(nullptr) {
            root_=copyNodes(other.root_);
        }

        void clear() {
            clearNodes(root_);
        }

        Map& operator=(const Map<KeyType, ValueType>& other) {
            if(this==&other) {
                return *this;
            }
            clear();
            root_=copyNodes(other.root_);
            return *this;
        }

        Map(KeyType& k,ValueType& v):
            root_(new Node(k,v)){}

        Node* insert(Node* node,const KeyType& k,const ValueType& v) {
            if(node==nullptr)   return new Node(k,v);
            if(node->key_<k) insert(node->right_, k, v);
            else if(node->key_>k) insert(node->left_, k, v);
            else {
                return node;
            }

            node->height_=1+std::max(getHeight(node->left_), getHeight(node->right_));

            int balance= getBalance(node);

            if(balance>1 and k<node->left_->key_) {
                return rightRotate(node);
            }
            else if(balance<-1 and k>node->right_->key_) {
                return leftRotate(node);
            }
            else if(balance>1 and k>node->left_->key_) {
                node->left_=leftRotate(node->left_);
                return rightRotate(node);
            }
            else if(balance<-1 and k<node->right_->key_) {
                node->right_=rightRotate(node->right_);
                return leftRotate(node);
            }
            return node;
        }

        ValueType& find(const KeyType& k) {
            Node* current=root_;
            while(current) {
                if(current->key_==k) {
                    return (current->value_);
                }
                else if(current->key_<k) {
                    current=current->right_;
                }
                else {
                    current=current->left_;
                }
            }
            throw std::runtime_error("Key not found");
        }
        void erase(const KeyType& k) {
            Node* current=root_;
            Node* nodeToDelete=nullptr;
            while(current) {
                if(current->key_==k){
                    nodeToDelete=current;
                    break;
                }
                else if(current->key_<k) {
                    current=current->right_;
                }
                else {
                    current=current->left_;
                }
            }
            if(nodeToDelete==nullptr) {
                throw std::runtime_error("Key not found");
                return;
            }

            // No children
            if(!nodeToDelete->left and !nodeToDelete->right) {
                if(nodeToDelete==root_) {
                    root_=nullptr;
                }
                else if(nodeToDelete->parent_->left_==nodeToDelete) {
                    nodeToDelete->parent_->left_=nullptr;
                }
                else if(nodeToDelete->parent_->right_==nodeToDelete) {
                    nodeToDelete->parent_->right_=nullptr;
                }
                delete nodeToDelete;
            }

            // One child
            if(nodeToDelete->left_!=nullptr or nodeToDelete->right_!=nullptr) {
                if(nodeToDelete->right_) {
                    if(nodeToDelete->parent_->left_==nodeToDelete) {
                        nodeToDelete->parent_->left_=nodeToDelete->right_;
                    }
                    else {
                        nodeToDelete->parent_->right_=nodeToDelete->right_;
                    }
                    nodeToDelete->right_->parent_=nodeToDelete->parent_;
                }
                else {
                    if(nodeToDelete->parent_->left_==nodeToDelete) {
                        nodeToDelete->parent_->left_=nodeToDelete->left_;
                    }
                    else {
                        nodeToDelete->parent_->right_=nodeToDelete->left_;
                    }
                    nodeToDelete->left_->parent_=nodeToDelete->parent_;
                }
                delete nodeToDelete;
            }

            // Two Children
            if(nodeToDelete->left_!=nullptr and nodeToDelete->right_!=nullptr) {

            }
        }

        class Iterator {
            private:
            Node* current;

            public:
            Iterator():
                current(nullptr){}
            Iterator():
            ValueType& operator*() const {
                return current->value_;
            }
            bool operator==(const Iterator& other) const {
                return current==other.current;
            }
        };

        Iterator begin() {
            return Iterator(root_);
        }
        Iterator end() {
            return Iterator();
        }
        Iterator rbegin() {
            return Iterator();
        }
        Iterator rend() {
            return Iterator(root_);
        }




    private:
        struct Node {
            KeyType key_;
            ValueType value_;
            Node *prev_;
            Node *next_;
            Node *parent_;
            size_t height_;

            Node(KeyType&k, ValueType&v):
                key_(k),
                value_(v),
                prev_(nullptr),
                next_(nullptr),
                parent_(nullptr),
                height_(1){}
        };
        Node * root_;

        Node* copyNodes(const Node* otherNode, Node* parent = nullptr) {
            if (!otherNode) return nullptr;

            Node* newNode = new Node(otherNode->key_, otherNode->value_);
            newNode->parent_ = parent;
            newNode->left_ = copyNodes(otherNode->left_, newNode);
            newNode->right_ = copyNodes(otherNode->right_, newNode);
            return newNode;
        }

        void clearNodes(Node* node) {
            if(node==nullptr) return;
            clearNodes(node->left_);
            clearNodes(node->right_);
            delete node;
        }

        int getHeight(Node* node) {
            if(node==nullptr) return 0;
            return node->height_;
        }

        int getBalance(Node* node) {
            return getHeight(node->left_)-getHeight(node->right_);
        }

        Node* rightRotate(Node* node) {
            Node* newRoot=node->left_;
            Node* subTree= newRoot->right_;

            node->left_=subTree;
            newRoot->parent_=node->parent_;
            node->parent_=newRoot;
            newRoot->right=node;

            if(subTree){
                subTree->parent_=node;
            }

            if(newRoot->parent_) {
                if(newRoot->parent_->left_==node) {
                    newRoot->parent_->left_=newRoot;
                }
                else {
                    newRoot->parent_->right_=newRoot;
                }
            }

            newRoot->height_=1+std::max(getHeight(newRoot->left_), getHeight(newRoot->right_));
            node->height_=1+std::max(getHeight(node->left_), getHeight(node->right_));

            return newRoot;
        }

        Node* leftRotate(Node* node) {
            Node* newRoot=node->right_;
            Node* subTree= newRoot->left_;

            newRoot->left_=node;
            node->right_=subTree;

            newRoot->parent_=node->parent_;
            node->parent_=newRoot;

            if(subTree) {
                subTree->parent_=node;
            }
            if(newRoot->parent_) {
                if(newRoot->parent_->left_==node) {
                    newRoot->parent_->left_=newRoot;
                }
                else {
                    newRoot->parent_->right_=newRoot;
                }
            }

            newRoot->height_=1+std::max(getHeight(newRoot->left_), getHeight(newRoot->right_));
            node->height_=1+std::max(getHeight(node->left_), getHeight(node->right_));
            return newRoot;
        }


    };
}




#endif //MAP_HPP
