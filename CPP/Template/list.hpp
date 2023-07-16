#include <iostream>
// 使用你的名字替换YOUR_NAME
namespace WangChen {
template <class T>
// list存储的节点
// 可以根据你的需要添加适当的成员函数与成员变量
struct node {
    node<T>* prev_;
    node<T>* next_;
    T data_;
    // 构造函数
    node(const T& data) : data_(data), prev_(nullptr), next_(nullptr) {}
    node() : prev_(nullptr), next_(nullptr) {}
    // 析构函数
    ~node() {}
};

template <class T>
// 迭代器类,(类比指针)

struct iterator {
    typedef node<T> node_;
    typedef iterator<T> iterator_;
    node_* node_ptr_;
    iterator(node_* node_ptr) : node_ptr_(node_ptr) {}
    ~iterator() {}
    // 迭代到下一个节点
    //++it
    iterator_& operator++() {
        node_ptr_ = node_ptr_->next_;
        return *this;
    }
    // 迭代到前一个节点
    //--it
    iterator_& operator--() {
        node - ptr_ = node_ptr_->prev_;
        return *this;
    }
    // it++
    iterator operator++(int) {
        iterator_ temp = *this;
        ++(*this);
        return temp;
    }
    // it--
    iterator operator--(int) {
        iterator_ temp = *this;
        --(*this);
        return temp;
    }
    // 指向迭代器中被访问的成员值
    T& operator*() { return node_ptr_->data_; }

    // 指向迭代器中被访问的成员指针
    T* operator->() { return &this->node_ptr_->data_; }
    // 重载==
    bool operator==(const iterator_& t) { return node_ptr_ == t.node_ptr_; }
    // 重载！=
    bool operator!=(const iterator_& t) { return node_ptr_ != t.node_ptr_; }

    //**可以添加需要的成员变量与成员函数**
};

template <class T>
class list {
   public:
    typedef node<T> node_;
    typedef iterator<T> Iterator;

   private:
    // 可以添加你需要的成员变量
    node_* head_;  // 头节点,哨兵（不存储有效数据）

   public:
    // 构造函数
    list() {
        head_ = new node_*;
        head_->next_ = head_;
        head_.prev_ = head_;
    }
    // 拷贝构造，要求实现深拷贝
    list(const list<T>& lt) {
        head_ = new node_;
        head_->next_ = head_;
        head_.prev_ = head_;

        for (auto e : lt) {
            push_back(e);
        }
    }
    template <class inputIterator>
    // 迭代器构造
    list(inputIterator begin, inputIterator end) {
        head_ = new node_;
        head_->next_ = head_;
        head_.prev_ = head_;
        while (begin != end) {
            push_back(*begin);
            ++begin;
        }
    }

    ~list() {}
    // 拷贝赋值
    list<T>& operator=(const list<T>& lt) {
        if (this == &lt) {
            return *this;
        }
        clear();
        for (auto it = lt.begin(); it != lt.end(); ++it) {
            push_back(*it);
        }
        return *this;
    }
    // 清空list中的数据
    void clear() {
        if (empty()) {
            return;
        }

        for (Iterator it = this->front(); it != this->end();) {
            erase(it++);
        }
    }
    // 返回容器中存储的有效节点个数
    size_t size() const {
        if (head_->next_ = head_) {
            return 0;
        }
        node_* p = head_->next_;
        size_t ret;
        while (p != head_) {
            ret++;
            p = p->next_;
        }
        return ret;
    }
    // 判断是否为空
    bool empty() const { return head_->next_ == head_; }
    // 尾插
    bool push_back(const T& data) {
        // new 一个new_node
        node_* new_node = new node_;
        if (new_node == NULL) {
            return false;
        }
        new_node->data_ = data;

        node_* tail_node = head_->prev_;
        new_node->next_ = head_;
        head_->prev_ = new_node;
        new_node->prev_ = tail_node;
        tail_node->next_ = new_node;
        return true;
    }

    // 头插
    bool push_front(const T& data) {
        node_* new_node = new node_;
        if (new_node == NULL) {
            return false;
        }
        new_node->data_ = data;

        node_* next_node = head_->next_;
        new_node->next_ = next_node;
        next_node->prev_ = new_node;
        new_node->prev_ = head_;
        head_->next_ = new_node;
        return true;
    }
    // 尾删
    bool pop_back() {
        if (head_->next_ = head_) {
            return false;
        }
        node_* tail_node = head_->prev_;
        node_* new_tail = tail_node->prev_;
        new_tail->next_ = head_;
        head_->prev_ = new_tail;
        delete tail_node;
        return true;
    }
    // 头删
    bool pop_front() {
        if (head_->prev_ = head_) {
            return false;
        }
        node_* head_node = head_->next_;
        node_* new_head = head_node->next_;

        new_head->prev_ = head_;
        head_->next_ = new_head;
        delete head_node;
        return true;
    }
    // 默认新数据添加到pos迭代器的后面,根据back的方向决定插入在pos的前面还是后面
    bool insert(Iterator pos, const T& data, bool back = true) {
        node_* new_node = new node_;
        new_node->data_ = data;
        if (back) {
            node_* next_node = pos.node_ptr_->next_;
            new_node->next_ = next_node;
            next_node->prev_ = new_node;

            pos.node_ptr_->next_ = new_node;
            new_node->prev_ = pos.node_ptr_;
        } else {
            node_* pre_node = pos.node_ptr_->prev_;
            new_node->next_ = pos.node_ptr_;
            pos.node_ptr_->prev_ = new_node;

            pre_node->next_ = new_node;
            new_node->prev_ = pre_node;
        }
    }
    // 删除pos位置的元素
    bool erase(Iterator pos) {
        node_* del_node = pos.node_ptr_;
        node_* next_node = del_node->next_;
        node_* pre_node = del_node->prev_;

        if (!empty() && del_node != head_) {
            pre_node->next_ = next_node;
            next_node->prev_ = pre_node;
            delete del_node;
            return true;
        }
        return false;
    }

    // 获得list第一个有效节点的迭代器
    Iterator begin() const { return head_->next_; }

    // 获得list最后一个节点的下一个位置
    Iterator end() const { return head_; }
    // 查找data对应的迭代器
    Iterator find(const T& data) const {
        if (emty())
            return NULL;
        node_* p = head_->next_;
        while (p != head_) {
            if (p->data_ == data) {
                return p;
            }
            p = p->next_;
        }
    }
    // 获得第一个有效节点元素值
    T front() const {
        if (empty()) {
            return NULL;
        }
        return head_->next_->data_;
    }
    // 获得最后一个有效节点元素值
    T back() const {
        if (empty()) {
            return NULL;
        }
        return head_->prev_->data_;
    }

   private:
    // 可以添加你需要的成员函数
};
};  // namespace WangChen