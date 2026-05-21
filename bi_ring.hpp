#ifndef BI_RING_HPP
#define BI_RING_HPP

#include <iostream>
#include <string>

template <typename Key, typename Info> 
class bi_ring 
{ 
    struct Node
    {
        Key key;
        Info info;
        Node* next;
        Node* back;

        Node(const Key& k, const Info& i, Node* n = nullptr, Node* b = nullptr)
            : key(k), info(i), next(n), back(b) {}
    }; 
    Node* any;
    size_t count;//Tracks number of elements in the list

    void copy_ring(const bi_ring& src);
    void delete_ring();

public: 
//------------First-iterator--------------------------------------------------------------------------------
    class iterator
    { 
    private:
        bi_ring<Key,Info> * owner;
    
        Node* current;
        Node* start_node;//Tracking traversal
        iterator(Node* c, Node* s) : current(c), start_node(s) {}

        public:
        friend class bi_ring;

        using value_type = std::pair<Key, Info>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        std::pair<Key&, Info&> operator*() const
        { 
            if (current == nullptr) 
                throw std::out_of_range("Dereferencing null iterator.");
            return {current->key, current->info}; 
        }
        //Prefix increment
        iterator& operator++()
        {
            if (current == nullptr) 
                return *this;
            current = current->next;
            return *this;
        }
        //Postfix increment
        iterator operator++(int)
        {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        //Prefix decrement
        iterator& operator--()
        {
            if (current != nullptr)
            {
                current = current->back;
            }
            else
            {
                if (start_node)
                {
                    current = start_node->back;
                }
            }
            return *this;
        }
        //Postfix decrement
        iterator operator--(int)
        {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        //Comparison operators
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }

        //Access to the raw node pointer (for bi_ring class methods like erase)
        Node* get_node() const { return current; }
    };
//------------Second-iterator--------------------------------------------------------------------------------
class const_iterator
{ 
    private:
        bi_ring<Key,Info> * owner; //Useful when comparing iterators from different rings
        const Node* current;
        const Node* start_node;
        const_iterator(const Node* c, const Node* s) : current(c), start_node(s) {}

    public:
        friend class bi_ring;
        using value_type = const std::pair<Key, Info>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag; 

        const_iterator() : current(nullptr), start_node(nullptr) {}
        //Conversion from non-const iterator to const
        const_iterator(const iterator& it) : current(it.get_node()), start_node(it.start_node) {}
        
        const std::pair<const Key&, const Info&> operator*() const
        { 
             if (current == nullptr) 
                throw std::out_of_range("Dereferencing null iterator.");
             return {current->key, current->info}; 
        }
        const_iterator& operator++()
        {
            if (current != nullptr) 
                current = current->next;//traversal in ring so it can go infinitely around
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        const_iterator& operator--()
        {
            if(current == start_node || current == nullptr)
            {
                current = start_node->back; 
            }
            /*else if(current == nullptr)
            {
                current = start_node->back;
            }*/
            else
            {
                current = current->back;
            }
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return current != other.current; }
    }; 
//--------------------------------------------------------------------------------
    bi_ring():any(nullptr), count(0) {} 

    bi_ring(const bi_ring& src):any(nullptr), count(0)
    {
        copy_ring(src);
    }

    ~bi_ring()
    { 
        delete_ring();
    } 

    bi_ring& operator=(const bi_ring& src) 
    {
        if (this != &src)
        {
            delete_ring();
            copy_ring(src);
        }
        return *this;
    }

    iterator begin()
    { 
        return iterator(any, any); 
    }
    const_iterator begin() const
    { 
        return const_iterator(any, any); 
    }
    const_iterator cbegin() const
    { 
        return const_iterator(any, any); 
    }

    iterator end()
    {
        return iterator(nullptr, any); 
    }

    const_iterator end() const
    { 
        return const_iterator(nullptr, any); 
    }
    const_iterator cend() const
    { 
        return const_iterator(nullptr, any); 
    }

    size_t size() const { return count; }
    bool empty() const { return count == 0; }

    iterator push_front(const Key& key, const Info& info); 
    iterator pop_front(); 
    iterator push_back(const Key& key, const Info& info); 
    iterator pop_back();

    iterator insert(iterator position, const Key& key, const Info& info); 
    iterator erase(iterator position); 
    size_t erase_all(const Key& target_key);//
    void print_forward() const;
    void print_reverse_recursive(const Node* current, const Node* stop_node)const;
    void print_reverse() const;
};
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
void bi_ring<Key, Info>::delete_ring()
{
    if (count == 0) return;

    Node* current = any;
    Node* next_node = nullptr;
    //Break the ring structure to prevent infinite loop
    current->back->next = nullptr; 

    while (current)
    {
        next_node = current->next;
        delete current;
        current = next_node;
    }
    any = nullptr;
    count = 0;
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
void bi_ring<Key, Info>::copy_ring(const bi_ring& src)
{
    if (src.empty())
    {
        any = nullptr;
        count = 0;
        return;
    }
    Node* src_curr = src.any;//Allocated memory
    Node* new_node = new Node(src_curr->key, src_curr->info);//Assigned values
    
    any = new_node;
    count = 1;
    Node* last_node = new_node;
    src_curr = src_curr->next;

    while (src_curr != src.any)
    {
        Node* next_new_node = new Node(src_curr->key, src_curr->info, nullptr, last_node);
        last_node->next = next_new_node;
        last_node = next_new_node;
        src_curr = src_curr->next;
        count++;
    }
    last_node->next = any;
    any->back = last_node;
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
typename bi_ring<Key, Info>::iterator bi_ring<Key, Info>::push_front(const Key& key, const Info& info)
{
    Node* new_node = new Node(key, info);

    if(empty())
    {
        //If empty, the node points to itself (circular)
        new_node->next = new_node;
        new_node->back = new_node;
        any = new_node;
    }
    else 
    {
        Node* tail = any->back;

        new_node->next = any;
        new_node->back = tail;

        tail->next = new_node;
        any->back = new_node;
        any = new_node;
    }
    count++;
    //Return iterator pointing to the newly inserted element
    return iterator(new_node, new_node); 
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
typename bi_ring<Key, Info>::iterator bi_ring<Key, Info>::push_back(const Key& key, const Info& info)
{
    if (empty())
    {
        return push_front(key, info);
    }

    Node* tail = any->back;
    Node* new_node = new Node(key, info);

    new_node->next = any;
    new_node->back = tail;

    tail->next = new_node;
    any->back = new_node;

    count++;
    return iterator(new_node, any); 
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
typename bi_ring<Key, Info>::iterator bi_ring<Key, Info>::pop_front()
{
    if (empty())
    {
        throw std::out_of_range("pop_front on empty bi_ring");
    }
    Node* old_front = any;
    
    if (count == 1)
    {
        delete old_front;
        any = nullptr;
        count = 0;
        return end(); 
    }

    Node* new_front = old_front->next;
    Node* tail = old_front->back; 

    tail->next = new_front;
    new_front->back = tail;

    any = new_front;

    delete old_front;
    count--;

    return iterator(new_front, new_front); 
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
typename bi_ring<Key, Info>::iterator bi_ring<Key, Info>::pop_back() {
    if (empty())
    {
        throw std::out_of_range("pop_back on empty bi_ring");
    }
    
    if (count == 1)
    {
        return pop_front();
    }

    Node* old_back = any->back;
    Node* new_back = old_back->back;
    Node* front = any;

    new_back->next = front;
    front->back = new_back;

    delete old_back;
    count--;

    return iterator(new_back, front); 
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
typename bi_ring<Key, Info>::iterator bi_ring<Key, Info>::insert(iterator position, const Key& key, const Info& info) 
{
    //Insertion occurs before position!!
    if (empty())
    {
        if (position == end())
        {
            return push_front(key, info);
        }else 
        {
            throw std::invalid_argument("Insert position invalid for empty ring.");
        }
    }

    if (position == begin())
    {
        return push_front(key, info);
    }
    if (position == end())
    {
        return push_back(key, info);
    }

    Node* next_node = position.get_node();
    if (!next_node)
    {
        throw std::out_of_range("Invalid iterator position for insert.");
    }

    Node* prev_node = next_node->back;
    Node* new_node = new Node(key, info, next_node, prev_node);

    prev_node->next = new_node;
    next_node->back = new_node;

    count++;
    return iterator(new_node, any);
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
typename bi_ring<Key, Info>::iterator bi_ring<Key, Info>::erase(iterator position)
{
    if (empty() || position == end())
    {
        throw std::out_of_range("erase on empty bi_ring or invalid iterator");
    }
    
    Node* to_delete = position.get_node();
    Node* next_node = to_delete->next;
 
    bool was_any = (to_delete == any);

    if (count == 1)
    {
        delete to_delete;
        any = nullptr;
        count = 0;
        return end();
    }

    Node* prev_node = to_delete->back;
    prev_node->next = next_node;
    next_node->back = prev_node;

    if (was_any)
    {
        any = next_node;
    }
    delete to_delete;
    count--;

    return iterator(next_node, any);
}
//--------------------------------------------------------------------------------
//Function that erases all ndoes with the same key, returns number of erased elements
template <typename Key, typename Info>
size_t bi_ring<Key, Info>::erase_all(const Key& target_key)
{
    if (empty()) return 0;
    
    size_t erased_count = 0;

    iterator current = begin();
    
    size_t initial_count = count;
    //     ^- to ensure we traverse circular list once
    for (size_t i = 0; i < initial_count; ++i)
    {
        const Key& current_key = (*current).first;
        if (current_key == target_key)
        {
            current = erase(current); 
            erased_count++;
        } 
        else
        {
            ++current;
        }
        if (empty()) break;
    }
    
    return erased_count;
}

//--------------------------------------------------------------------------------
template <typename Key, typename Info>
void bi_ring<Key, Info>::print_forward() const
{
    if (empty())
    {
        std::cout<< "Ring: [EMPTY]"<<std::endl;
        return;
    }

    std::cout<< "Ring (Forward): [";

    const Node* current = any;
    
    do{
        std::cout<<current->key<<":"<<current->info;
        current=current->next;
        if(current != any)
        {
            std::cout<<", ";
        }
    }while(current != any);
    
    std::cout <<"]" <<std::endl;
}

//--------------------------------------------------------------------------------
template <typename Key, typename Info>
void bi_ring<Key, Info>::print_reverse_recursive(const Node* current, const Node* stop_node)const
{
    if (current == stop_node)
    {
        return; 
    }

    print_reverse_recursive(current->next, stop_node);

    const Node* head = any;
    const Node* tail = head->back;

    if (current != tail)
    {
        std::cout << ", ";
    }
    std::cout << current->key << ":" << current->info;
}
//--------------------------------------------------------------------------------
template <typename Key, typename Info>
void bi_ring<Key, Info>::print_reverse() const
{
    if (empty()) {
        std::cout << "Ring: [EMPTY]" << std::endl;
        return;
    }
    std::cout << "Ring (Reverse): [";
   
    const Node* head = any;
    const Node* tail = head->back;
    print_reverse_recursive(head, head);

    std::cout << "]" << std::endl;
}
#endif