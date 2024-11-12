#include <stddef.h>
#include<iostream>
#ifndef LIST_HPP
#define LIST_HPP



namespace vai{
    
    template<typename T>
    class List{

    private:

        struct ListNode{
            T value;
            ListNode* next;
            ListNode* previous;

            ListNode(T& val):
                value(val),
                next(nullptr),
                previous(nullptr){}

            ListNode():
                value(T()),
                next(nullptr),
                previous(nullptr){}

            ListNode(ListNode* oth):
                value(oth ? oth->value : 0),
                next(oth ? oth->next : nullptr),
                previous(oth ? oth->previous : nullptr){}

            template <class... Args>
            ListNode(Args&&... args) : value(std::forward<Args>(args)...) {}

            ~ListNode(){}
        };

        ListNode* head;
        ListNode* tail;
        size_t size_;

    public:
        List():
            head(nullptr),
            tail(nullptr),
            size_(0){}
        


        List& operator=(List& other){
            if(this==&other) {
                return *this;
            }
            clear();

            size_=other.size_;
            if(other.head){
                head= new ListNode(other.head->value);
                ListNode* current= head;
                ListNode* nextCurrent= other.head->next;

                while(nextCurrent){
                    current->next=new ListNode(nextCurrent->value);
                    current->next->previous= current;
                    current=current->next;
                    nextCurrent=nextCurrent->next;
                }
                tail=current;
            }
            else{
                head=tail=nullptr;
            }
            return *this;
        }

        List& operator=(List&& other){
            if(this==&other){
                return *this;
            }
            clear();
            size_=other.size_;
            head=other.head;
            tail=other.tail;
            other.size_=0;
            other.head=nullptr;
            other.tail=nullptr;
            return *this;
        }

        bool operator==( List& other){
            if(this->size_!=other.size_){
                return false;
            }
            ListNode* current=this->head;
            ListNode* otherCurrent=other.head;
            while(current and otherCurrent){
                if(current->value!=otherCurrent->value){
                    return false;
                }
                current=current->next;
                otherCurrent=otherCurrent->next;
            }
            return true;
        }
        
        class Iterator{
            public:
                Iterator(ListNode* node):
                    current(node){}

                T& operator*(){
                    return current->value;
                }

                Iterator& operator ++(){
                    if(current->next==nullptr){
                       std::cout<<"This is the last element";
                        return *this;
                    }
                        this->current=this->current->next;
                        return *this;
                }

                Iterator& operator --(){
                    if(current->previous==nullptr){
                        std::cout<<"This is the first element";
                        return *this;
                    }
                       current = current->previous;
                       return *this;
                }

                ListNode* getNode(){
                    return current;
                }

                bool operator==(const Iterator& other) {
                    if(this->current==other.current){
                        return true;
                    }
                    return false;
                }

                bool operator!=(const Iterator& other){
                    return !(*this==other);
                }


            private:
               typename List<T>:: ListNode* current;
            
        };
        
        void push_back(T& value){
            ListNode* node= new ListNode(value);
            if(head==nullptr){
                head=node;
                tail=node;
            }
            else{
                tail->next=node;
                node->previous=tail;
                tail=node;
            }
            ++size_;
        }

        template< class... Args>
        Iterator emplace(Iterator iter, Args&&... args){
            ListNode* node=iter.getNode();
            ListNode* newNode=new ListNode(std::forward<Args>(args)...);
            ++size_;
            if(node==head){
                newNode->next=node;
                node->previous=newNode;
                head=newNode;
                return Iterator(head);
            }
            else if(node== nullptr){
                if(tail){
                    tail->next=newNode;
                    newNode->previous=tail;
                }
                tail=newNode;
                if(head==nullptr){
                    head=tail;
                }
                return Iterator(tail);
            }
            else{
                ListNode* temp=node->previous;
                temp->next=newNode;
                newNode->previous=temp;
                newNode->next=node;
                node->previous=newNode;
                return Iterator(newNode);
            }
        }

        template<class ... Args>
        void emplace_back(Args&&... args){
            ListNode* node= new ListNode(std::forward<Args>(args)...);
            if(head==nullptr){
                head=node;
                tail=node;
            }
            else{
                tail->next=node;
                node->previous=tail;
                tail=node;
            }
            ++size_;
        }

        template<class ... Args>
        void emplace_front(Args&&... args){
            ListNode* node= new ListNode(std::forward<Args>(args)...);
            if(head==nullptr){
                head=node;
                tail=node;
           }
           else{
               node->next=head;
               head->previous=node;
               head=node;
           }
           ++size_;
        }

        void push_front(T& value){
            ListNode* node= new ListNode(value);
            if(head==nullptr){
                head=node;
                tail=node;
            }
            else{
                node->next=head;
                head->previous=node;
                head=node;
            }
            ++size_;
        }

        void pop_back(){
            if (tail == nullptr) return;
            if (head == tail) {
                delete tail;
                head = tail = nullptr;
            } else {
                ListNode* temp = tail;
                tail = tail->previous;
                tail->next = nullptr;
                delete temp;
            }
            --size_;
        }

        void pop_front(){
            if (head == nullptr) return;
            if (head == tail) {
                delete head;
                head = tail = nullptr;
            } else {
                ListNode* temp = head;
                head = head->next;
                head->previous = nullptr;
                delete temp;
            }
            --size_;
        }

        ListNode* front(){
            if(head==nullptr){
                std::cout<<"List is empty\n";
                return nullptr;
            }
            return head;
        }
        ListNode* back(){
            if(head==nullptr){
                std::cout<<"List is empty\n";
                return nullptr;
            }
            return tail;
        }

        bool empty(){
            return head==nullptr;
        }

        size_t size(){
            return size_;
        }

        void clear(){
           ListNode* current=head;
           while(current){
            ListNode* delete_this=current;
            current=current->next;
            delete delete_this;
           }
           head=nullptr;
           tail=nullptr;
           size_=0;
        }

        

        Iterator insert(Iterator pos, T& data){
            ListNode* node= pos.getNode();
            ListNode* new_node= new ListNode(data);
            ++size_;
            if(node==tail){ 
                new_node->previous=tail;
                new_node->next=nullptr;
                tail->next=new_node;
                tail=new_node;
                return Iterator(tail);
            }
            if(node==head){
                new_node->previous=nullptr;
                new_node->next=head;
                head->previous=new_node;
                head=new_node;
                return Iterator(head);
            }
            ListNode* temp=node->next;
            new_node->next=temp;
            new_node->previous=node;
            node->next=new_node;
            temp->previous=new_node;
            return Iterator(new_node);
        }

        Iterator begin(){
            return Iterator(head);
        }
        Iterator end(){
            return Iterator(nullptr);
        }

        Iterator rbegin(){
            return Iterator(tail);
        }

        Iterator rend(){
            return Iterator(head);
        }

        void erase(Iterator pos){
            ListNode* node=pos.getNode();
            if(node==head){
                head=head->next;
                head->previous=nullptr;
                delete node;
            }
            else if(node==tail){
                tail=tail->previous;
                tail->next=nullptr;
                delete node;
            }
            else{
                ListNode* temp=node->previous;
                temp->next=node->next;
                node->next->previous=temp;
                delete node;
            }
            --size_;
        }

        Iterator find(T& value){
            for(auto iter=begin();iter!=end();++iter){
                if(iter->value == value){
                    return iter;
                }
            }
            return Iterator(end);
        }

        void reverse(){
            ListNode* current = head;
            ListNode* temp = nullptr;
            while (current != nullptr) {
                temp = current->previous;
                current->previous = current->next;
                current->next = temp;
                current = current->previous;
            }
            if (temp != nullptr) {
                tail = head;
                head = temp->previous;
            }
        }
        void merge(List & other){
            if(this==&other){
                return;
            }
            ListNode* thisCurrent = head;
            ListNode* otherCurrent = other.head;
            List<T> result;

            while (thisCurrent && otherCurrent) {
                if (thisCurrent->value < otherCurrent->value) {
                    result.push_back(thisCurrent->value);
                    thisCurrent = thisCurrent->next;
                } else {
                    result.push_back(otherCurrent->value);
                    otherCurrent = otherCurrent->next;
                }
            }
            while (thisCurrent) {
                result.push_back(thisCurrent->value);
                thisCurrent = thisCurrent->next;
            }
            while (otherCurrent) {
                result.push_back(otherCurrent->value);
                otherCurrent = otherCurrent->next;
            }

            *this = std::move(result);
            other.clear();
        }
};

 






}
#endif