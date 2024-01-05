#ifndef WCSSKILLS_LINKEDLIST
#define WCSSKILLS_LINKEDLIST
#include <functional>
#include <cassert>
#include <type_traits>
#include "exceptions.hpp"

namespace dataStorage {
    template<typename T>
    class DoubleLinkedListNode;
    
    template<typename T>
    class DoubleLinkedList;
    
    
    template<typename T>
    class DoubleLinkedListNode {
    private:
        template<typename... Args, std::is_constructible<T, Args...> = true>
        explicit DoubleLinkedListNode(Args&&...);
        
        explicit DoubleLinkedListNode(T const&);
        ~DoubleLinkedListNode() = default;
    protected:
        DoubleLinkedListNode<T>* previous;
        DoubleLinkedListNode<T>* next;
        
        DoubleLinkedListNode<T>* insertAfter(DoubleLinkedListNode<T>*);
        DoubleLinkedListNode<T>* insertBefore(DoubleLinkedListNode<T>*);
        DoubleLinkedListNode<T>* insertAfter(T&);
        DoubleLinkedListNode<T>* insertBefore(T&);
        
        DoubleLinkedListNode<T>* eraseReturnNext() noexcept; // Deletes current node
        DoubleLinkedListNode<T>* eraseReturnPrevious() noexcept; // Deletes current node
        void erase() noexcept;  // Deletes current node
    public:
        DoubleLinkedListNode<T>* getNext() const;
        DoubleLinkedListNode<T>* getPrevious() const;
        T data;
        friend DoubleLinkedList<T>;
    };
    
    
    class UnspecifiedBehavior : public CustomException
            { using CustomException::CustomException; };
    
    template<typename T>
    class DoubleLinkedList {
        unsigned int _size = 0;
    public:
        ~DoubleLinkedList();
        
        DoubleLinkedListNode<T>* head = nullptr;
        DoubleLinkedListNode<T>* tail = nullptr;
        
        unsigned int size() noexcept;
        
        DoubleLinkedListNode<T>* findNode(std::function<bool(const T)>) const noexcept;
        DoubleLinkedListNode<T>* findNodeReverse(std::function<bool(const T)>) const noexcept;
        
        void forEach(std::function<void(DoubleLinkedListNode<T>*)>) const noexcept; // Erases forbidden
        void forEachReverse(std::function<void(DoubleLinkedListNode<T>*)>) const noexcept; // Erases forbidden
        void forEach(std::function<void(T)>);
        void forEachReverse(std::function<void(T)>);
        void insertAfter(DoubleLinkedListNode<T>*, T&&);
        void insertBefore(DoubleLinkedListNode<T>*, T&&);
        void insertAfter(DoubleLinkedListNode<T>*, T&);
        void insertBefore(DoubleLinkedListNode<T>*, T&);
        
        void erase(DoubleLinkedListNode<T>*);
    };
    
    template<typename T>
    DoubleLinkedListNode<T>::DoubleLinkedListNode
    (T const& data) : data(data), previous(nullptr), next(nullptr) {}
    
    template<typename T>
    template<typename... Args, std::is_constructible<T, Args...>>
    DoubleLinkedListNode<T>::DoubleLinkedListNode
    (Args&&... args) : data(T(args...)), previous(nullptr), next(nullptr) {}
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::insertAfter
    (DoubleLinkedListNode<T>* new_node) {
        if (next) next->previous = new_node;
        new_node->previous = this;
        new_node->next = next;
        next = new_node;
        return new_node;
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::insertBefore
    (DoubleLinkedListNode<T>* new_node) {
        if (previous) previous->next = new_node;
        new_node->previous = previous;
        new_node->next = this;
        previous = new_node;
        return new_node;
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::insertAfter
    (T& new_node_data) {
        return insertAfter(new DoubleLinkedListNode<T>(new_node_data));
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::insertBefore
    (T& new_node_data) {
        return insertBefore(new DoubleLinkedListNode<T>(new_node_data));
    }
    
    
    template<typename T>
    void DoubleLinkedListNode<T>::erase() noexcept {
        if (previous) previous->next = next;
        if (next) next->previous = previous;
        delete this;
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::eraseReturnNext() noexcept {
        auto* n = next;
        erase();
        return n;
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::eraseReturnPrevious() noexcept {
        auto* n = previous;
        erase();
        return n;
    }


    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::getNext() const {
        return this->next;
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedListNode<T>::getPrevious() const {
        return this->previous;
    }
    
    template<typename T>
    DoubleLinkedList<T>::~DoubleLinkedList() {
        if (!head) return;
        auto* node = head;
        while (node) node = node->eraseReturnNext();
    }
    
    template<typename T>
    unsigned int DoubleLinkedList<T>::size() noexcept {
        return _size;
    }
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedList<T>::findNode
    (std::function<bool(const T)> function) const noexcept {
        auto* current_node = head;
        while (current_node) {
            if (function(current_node->data)) return current_node;
            current_node = current_node->next;
        }
        return nullptr;
    }
    
    
    template<typename T>
    DoubleLinkedListNode<T>* DoubleLinkedList<T>::findNodeReverse
    (std::function<bool(const T)> function) const noexcept  {
        auto* current_node = tail;
        while (current_node) {
            if (function(current_node->data)) return current_node;
            current_node = current_node->previous;
        }
        return nullptr;
    }
    
    
    
    template<typename T>
    void DoubleLinkedList<T>::forEach
    (std::function<void(DoubleLinkedListNode<T>*)> function) const noexcept {
        auto* current_node = head;
        while (current_node) {
            function(current_node);
            current_node = current_node->next;
        }
    }
    
    template<typename T>
    void DoubleLinkedList<T>::forEachReverse
    (std::function<void(DoubleLinkedListNode<T>*)> function) const noexcept {
        auto* current_node = tail;
        while (current_node) {
            function(current_node);
            current_node = current_node->previous;
        }
    }
    
    template<typename T>
    void DoubleLinkedList<T>::forEach
    (std::function<void(T)> function) {
        auto* current_node = head;
        while (current_node) {
            function(current_node->data);
            current_node = current_node->next;
        }
    }
    
    template<typename T>
    void DoubleLinkedList<T>::forEachReverse
    (std::function<void(T)> function) {
        auto* current_node = tail;
        while (current_node) {
            function(current_node->data);
            current_node = current_node->previous;
        }
    }
    
    
    template<typename T>
    void DoubleLinkedList<T>::insertAfter
    (DoubleLinkedListNode<T>* target, T&& _data) {
        if (_size == 0) {
            assert(target == nullptr);
            head = tail = new DoubleLinkedListNode<T>(_data);
        } else if (target==tail) {
            assert(target != nullptr);
            tail = tail->insertAfter(_data);
        } else {
            assert(target != nullptr);
            target->insertAfter(_data);
        }
        _size++;
    }
    
    template<typename T>
    void DoubleLinkedList<T>::insertBefore
    (DoubleLinkedListNode<T>* target, T&& _data) {
        if (_size == 0) {
            assert(target == nullptr);
            head = tail = new DoubleLinkedListNode<T>(_data);
        } else if (target==head) {
            assert(target != nullptr);
            head = head->insertBefore(_data);
        } else {
            assert(target != nullptr);
            target->insertBefore(_data);
        }
        _size++;
    }
    
    
    template<typename T>
    void DoubleLinkedList<T>::insertAfter
    (DoubleLinkedListNode<T>* target, T& _data) {
        if (_size == 0) {
            assert(target == nullptr);
            head = tail = new DoubleLinkedListNode<T>(_data);
        } else if (target==tail) {
            assert(target != nullptr);
            tail = tail->insertAfter(_data);
        } else {
            assert(target != nullptr);
            target->insertAfter(_data);
        }
        _size++;
    }
    
    template<typename T>
    void DoubleLinkedList<T>::insertBefore
    (DoubleLinkedListNode<T>* target, T& _data) {
        if (_size == 0) {
            assert(target == nullptr);
            head = tail = new DoubleLinkedListNode<T>(_data);
        } else if (target==head) {
            assert(target != nullptr);
            head = head->insertBefore(_data);
        } else {
            assert(target != nullptr);
            target->insertBefore(_data);
        }
        _size++;
    }
    
    template<typename T>
    void DoubleLinkedList<T>::erase(DoubleLinkedListNode<T>* target) {
        assert(target != nullptr);
        if (target == head) {
            head = head->eraseReturnNext();
            if (!head)
                tail = nullptr;
        } else if (target == tail) {
            tail = tail->eraseReturnPrevious();
            if (!tail)
                head = nullptr;
        } else {
            target->erase();
        }
        _size--;
    }
}

#endif //WCSSKILLS_LINKEDLIST
