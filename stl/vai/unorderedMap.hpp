#ifndef UNORDEREDMAP_HPP
#define UNORDEREDMAP_HPP

#pragma once
#include <utility>
#include <vector>

namespace vai {
    template <typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
    class unordered_map {
    private:
        struct Node {
            KeyType key_;
            ValueType value_;
            Node* next_;

            Node():
                next_(nullptr) {}

            Node(KeyType key, ValueType value):
                key_(key),
                value_(value),
                next_(nullptr) {}
        };

        size_t bucketSize_;
        std::vector<Node*> buckets_;
        size_t size_;
        float loadFactor_;
        Hash hashFunction_;

        size_t getBucketIndex(const KeyType& key) const {

            return hashFunction_(key) & (bucketSize_-1);
        }

        void resizeBucket() {
            // here you have to resize the bucket. you will need to get more memory space and then
            size_t newBucketSize_ = 2 * bucketSize_;
            std::vector<Node*> newBucket(newBucketSize_, nullptr);
            for (size_t i = 0; i < bucketSize_; i++) {
                Node* head = buckets_[i];

                while (head) {
                    Node* next       = head->next_;
                    size_t index     = hashFunction_(head->key_) & (newBucketSize_-1);
                    head->next_      = newBucket[index];
                    newBucket[index] = head;
                    head             = next;
                }
            }
            bucketSize_ = newBucketSize_;
            buckets_    = std::move(newBucket);
        }

    public:
        unordered_map():
            bucketSize_(1024),
            buckets_(bucketSize_, nullptr),
            size_(0),
            loadFactor_(1) {}

        unordered_map(size_t bucketSize):
            bucketSize_(bucketSize),
            buckets_(bucketSize_, nullptr),
            size_(0),
            loadFactor_(1) {
            if (bucketSize_ < 0 or bucketSize_ % 2 != 0) {
                throw std::invalid_argument("Bucket size must be positive and even");
            }
        }

        ~unordered_map() {
            // here delete all the nodes in all the linked list's you have at your disposal.
            clear();
        }

        class Iterator {
            Node* current;

        public:
            Iterator():
                current() {}

            Iterator(Node* node):
                current(node) {}
        };

        Iterator begin() const {
            for (size_t i = 0; i < bucketSize_; i++) {
                if (buckets_[i] != nullptr) {
                    return Iterator(buckets_[i]);
                }
            }
            return Iterator();
        }

        Iterator end() const {
            return Iterator();
        }

        void clear() {
            for (size_t i = 0; i < buckets_.size(); ++i) {
                Node* head = buckets_[i];
                while (head) {
                    Node* temp = head;
                    head       = temp->next_;
                    delete temp;
                }
            }
        }

        size_t size() const {
            return size_;
        }

        bool insert(const KeyType& key, const ValueType& value) {
            size_t index = getBucketIndex(key);
            Node* head   = buckets_[index];
            while (head) {
                if (head->key_ == key) {
                    return false;
                }
                head = head->next_;
            }
            Node* node      = new Node(key, value);
            node->next_     = buckets_[index];
            buckets_[index] = node;
            ++size_;

            if (float(size_) / bucketSize_ >= loadFactor_) {
                resizeBucket();
            }
            return true;
        }

        auto find(const KeyType& key) {
            size_t index = getBucketIndex(key);
            Node* head   = buckets_[index];
            while (head) {
                if (head->key_ == key) {
                    return Iterator(head);
                }
                head = head->next_;
            }
            return Iterator();
        }

        size_t erase(const KeyType& key) {
            size_t index = getBucketIndex(key);
            Node* head   = buckets_[index];
            if (head == nullptr) {
                return 0;
            }
            if (head->key_ == key) {
                Node* temp      = head->next_;
                buckets_[index] = temp;
                delete head;
                --size_;
                return 1;
            }
            while (head->next_) {
                if (head->next_->key_ == key) {
                    Node* found = head->next_;
                    head->next_ = found->next_;
                    --size_;
                    delete found;
                    return 1;
                }
                head = head->next_;
            }
            return 0;
        }
    };
}

#endif
