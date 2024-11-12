#include <iostream>
#include <vai/vector.hpp>

class foo {
private:
    int x_;
    int y_;

public:
    foo():
        x_(0),
        y_(0) {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
    }

    explicit foo(int x, int y):
        x_(x),
        y_(y) {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
    }

    explicit foo(const foo& obj):
        x_(obj.x_),
        y_(obj.y_) {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
    }

    foo(foo&& obj):
        x_(obj.x_),
        y_(obj.y_) {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
    }

    ~foo() {
        std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
    }
};

int main() {
    vai::vector<foo> v;

    foo foo1;
    foo foo2(2, 2);
    v.push_back(foo1);
    v.emplace_back(7, 7);
    v.emplace_back(9, 9);

    return 0;
}

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
using namespace std;
#include <stack>
#include <vector>

class Solution {
public:
    struct ListNode;

    struct ListNode {
        int val;
        ListNode* next;

        ListNode():
            val(0),
            next(nullptr) {}

        ListNode(int x):
            val(x),
            next(nullptr) {}

        ListNode(int x, ListNode* next):
            val(x),
            next(next) {}
    };

    struct Store {
        int index;
        ListNode* cu;

        // Store(int i): index(i), cu(nullptr){}
        Store(int i, ListNode* temp):
            index(i),
            cu(temp) {}
    };

    std::vector<int> nextLargerNodes(ListNode* head) {
        if (head == nullptr) {
            return vector<int>();
        }
        stack<Store> st;
        ListNode* current = head;
        Store cu_         = Store(0, current);
        st.push(cu_);

        // count the number of bnodes in LL
        int n = 0;
        while (current) {
            ++n;
            current = current->next;
        }
        vector<int> res(n, 0);

        current = head->next;
        int i   = 1;
        while (current) {
            Store temp = st.top();
            if (current->val <= temp.cu->val) {
                st.push(Store(i, current));
                ++i;
                current = current->next;
                continue;
            }
            while (!st.empty() and temp.cu->val < current->val) {
                res[temp.index] = current->val;
                st.pop();
                temp = st.top();
            }
            st.push(Store(i, current));
            ++i;
            current = current->next;
        }
        return res;
    }
};