/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
   class Key,
   class T,
   class Compare = std::less <Key>
   > class map {
  public:
   typedef pair<const Key, T> value_type;

  private:
   enum Color { RED, BLACK };

   struct Node {
       value_type *data;
       Node *left;
       Node *right;
       Node *parent;
       Color color;

       Node(const value_type &val, Node *p = nullptr)
           : left(nullptr), right(nullptr), parent(p), color(RED) {
           data = new value_type(val);
       }

       Node() : data(nullptr), left(nullptr), right(nullptr), parent(nullptr), color(BLACK) {}

       ~Node() {
           if (data) delete data;
       }
   };

   Node *root;
   Node *nil;  // sentinel node for end iterator
   Node *minNode;  // cache for minimum node
   Node *maxNode;  // cache for maximum node
   size_t count_size;
   Compare comp;

   void rotateLeft(Node *x) {
       Node *y = x->right;
       x->right = y->left;
       if (y->left) y->left->parent = x;
       y->parent = x->parent;
       if (!x->parent) {
           root = y;
       } else if (x == x->parent->left) {
           x->parent->left = y;
       } else {
           x->parent->right = y;
       }
       y->left = x;
       x->parent = y;
   }

   void rotateRight(Node *x) {
       Node *y = x->left;
       x->left = y->right;
       if (y->right) y->right->parent = x;
       y->parent = x->parent;
       if (!x->parent) {
           root = y;
       } else if (x == x->parent->right) {
           x->parent->right = y;
       } else {
           x->parent->left = y;
       }
       y->right = x;
       x->parent = y;
   }

   void insertFixup(Node *z) {
       while (z->parent && z->parent->color == RED) {
           if (z->parent == z->parent->parent->left) {
               Node *y = z->parent->parent->right;
               if (y && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->right) {
                       z = z->parent;
                       rotateLeft(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotateRight(z->parent->parent);
               }
           } else {
               Node *y = z->parent->parent->left;
               if (y && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->left) {
                       z = z->parent;
                       rotateRight(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotateLeft(z->parent->parent);
               }
           }
       }
       root->color = BLACK;
   }

   void transplant(Node *u, Node *v) {
       if (!u->parent) {
           root = v;
       } else if (u == u->parent->left) {
           u->parent->left = v;
       } else {
           u->parent->right = v;
       }
       if (v) v->parent = u->parent;
   }

   Node* minimum(Node *x) const {
       if (!x) return nil;
       while (x->left) x = x->left;
       return x;
   }

   Node* maximum(Node *x) const {
       if (!x) return nil;
       while (x->right) x = x->right;
       return x;
   }

   void deleteFixup(Node *x, Node *xParent) {
       while (x != root && (!x || x->color == BLACK)) {
           if (x == (xParent ? xParent->left : nullptr)) {
               Node *w = xParent->right;
               if (w && w->color == RED) {
                   w->color = BLACK;
                   xParent->color = RED;
                   rotateLeft(xParent);
                   w = xParent->right;
               }
               if ((!w->left || w->left->color == BLACK) &&
                   (!w->right || w->right->color == BLACK)) {
                   w->color = RED;
                   x = xParent;
                   xParent = x ? x->parent : nullptr;
               } else {
                   if (!w->right || w->right->color == BLACK) {
                       if (w->left) w->left->color = BLACK;
                       w->color = RED;
                       rotateRight(w);
                       w = xParent->right;
                   }
                   w->color = xParent->color;
                   xParent->color = BLACK;
                   if (w->right) w->right->color = BLACK;
                   rotateLeft(xParent);
                   x = root;
               }
           } else {
               Node *w = xParent->left;
               if (w && w->color == RED) {
                   w->color = BLACK;
                   xParent->color = RED;
                   rotateRight(xParent);
                   w = xParent->left;
               }
               if ((!w->right || w->right->color == BLACK) &&
                   (!w->left || w->left->color == BLACK)) {
                   w->color = RED;
                   x = xParent;
                   xParent = x ? x->parent : nullptr;
               } else {
                   if (!w->left || w->left->color == BLACK) {
                       if (w->right) w->right->color = BLACK;
                       w->color = RED;
                       rotateLeft(w);
                       w = xParent->left;
                   }
                   w->color = xParent->color;
                   xParent->color = BLACK;
                   if (w->left) w->left->color = BLACK;
                   rotateRight(xParent);
                   x = root;
               }
           }
       }
       if (x) x->color = BLACK;
   }

   void deleteNode(Node *z) {
       Node *y = z;
       Node *x;
       Node *xParent;
       Color y_original_color = y->color;

       if (!z->left) {
           x = z->right;
           xParent = z->parent;
           transplant(z, z->right);
       } else if (!z->right) {
           x = z->left;
           xParent = z->parent;
           transplant(z, z->left);
       } else {
           y = minimum(z->right);
           y_original_color = y->color;
           x = y->right;
           xParent = y->parent;
           if (y->parent == z) {
               xParent = y;
           } else {
               transplant(y, y->right);
               y->right = z->right;
               y->right->parent = y;
           }
           transplant(z, y);
           y->left = z->left;
           y->left->parent = y;
           y->color = z->color;
       }

       delete z;

       if (y_original_color == BLACK) {
           deleteFixup(x, xParent);
       }
   }

   Node* copyTree(Node *node, Node *parent) {
       if (!node) return nullptr;
       Node *newNode = new Node(*(node->data), parent);
       newNode->color = node->color;
       newNode->left = copyTree(node->left, newNode);
       newNode->right = copyTree(node->right, newNode);
       return newNode;
   }

   void clearTree(Node *node) {
       if (!node) return;
       clearTree(node->left);
       clearTree(node->right);
       delete node;
   }

   Node* findNode(const Key &key) const {
       Node *current = root;
       while (current) {
           if (comp(key, current->data->first)) {
               current = current->left;
           } else if (comp(current->data->first, key)) {
               current = current->right;
           } else {
               return current;
           }
       }
       return nullptr;
   }

  public:
   /**
  * see BidirectionalIterator at CppReference for help.
  *
  * if there is anything wrong throw invalid_iterator.
  *     like it = map.begin(); --it;
  *       or it = map.end(); ++end();
    */
   class const_iterator;
   class iterator {
       friend class map;
       friend class const_iterator;
      private:
       map *mp;
       Node *node;

       iterator(map *m, Node *n) : mp(m), node(n) {}

      public:
       iterator() : mp(nullptr), node(nullptr) {}

       iterator(const iterator &other) : mp(other.mp), node(other.node) {}

       /**
    * iter++
        */
       iterator operator++(int) {
           if (!node || node == mp->nil) {
               throw invalid_iterator();
           }
           iterator tmp = *this;
           if (node->right) {
               node = mp->minimum(node->right);
           } else {
               Node *p = node->parent;
               while (p && node == p->right) {
                   node = p;
                   p = p->parent;
               }
               node = p ? p : mp->nil;
           }
           return tmp;
       }

       /**
    * ++iter
        */
       iterator &operator++() {
           if (!node || node == mp->nil) {
               throw invalid_iterator();
           }
           if (node->right) {
               node = mp->minimum(node->right);
           } else {
               Node *p = node->parent;
               while (p && node == p->right) {
                   node = p;
                   p = p->parent;
               }
               node = p ? p : mp->nil;
           }
           return *this;
       }

       /**
    * iter--
        */
       iterator operator--(int) {
           iterator tmp = *this;
           if (node == mp->nil) {
               node = mp->maximum(mp->root);
               if (!node) throw invalid_iterator();
           } else if (!node) {
               throw invalid_iterator();
           } else {
               if (node->left) {
                   node = mp->maximum(node->left);
               } else {
                   Node *p = node->parent;
                   while (p && node == p->left) {
                       node = p;
                       p = p->parent;
                   }
                   if (!p) throw invalid_iterator();
                   node = p;
               }
           }
           return tmp;
       }

       /**
    * --iter
        */
       iterator &operator--() {
           if (node == mp->nil) {
               node = mp->maximum(mp->root);
               if (!node) throw invalid_iterator();
           } else if (!node) {
               throw invalid_iterator();
           } else {
               if (node->left) {
                   node = mp->maximum(node->left);
               } else {
                   Node *p = node->parent;
                   while (p && node == p->left) {
                       node = p;
                       p = p->parent;
                   }
                   if (!p) throw invalid_iterator();
                   node = p;
               }
           }
           return *this;
       }

       /**
    * a operator to check whether two iterators are same (pointing to the same memory).
        */
       value_type &operator*() const {
           if (!node || node == mp->nil) {
               throw invalid_iterator();
           }
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return mp == rhs.mp && node == rhs.node;
       }

       bool operator==(const const_iterator &rhs) const {
           return mp == rhs.mp && node == rhs.node;
       }

       /**
    * some other operator for iterator.
        */
       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       /**
    * for the support of it->first.
    * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
        */
       value_type *operator->() const noexcept {
           return node->data;
       }
   };

   class const_iterator {
       friend class map;
      private:
       const map *mp;
       Node *node;

       const_iterator(const map *m, Node *n) : mp(m), node(n) {}

      public:
       const_iterator() : mp(nullptr), node(nullptr) {}

       const_iterator(const const_iterator &other) : mp(other.mp), node(other.node) {}

       const_iterator(const iterator &other) : mp(other.mp), node(other.node) {}

       const_iterator operator++(int) {
           if (!node || node == mp->nil) {
               throw invalid_iterator();
           }
           const_iterator tmp = *this;
           if (node->right) {
               node = mp->minimum(node->right);
           } else {
               Node *p = node->parent;
               while (p && node == p->right) {
                   node = p;
                   p = p->parent;
               }
               node = p ? p : mp->nil;
           }
           return tmp;
       }

       const_iterator &operator++() {
           if (!node || node == mp->nil) {
               throw invalid_iterator();
           }
           if (node->right) {
               node = mp->minimum(node->right);
           } else {
               Node *p = node->parent;
               while (p && node == p->right) {
                   node = p;
                   p = p->parent;
               }
               node = p ? p : mp->nil;
           }
           return *this;
       }

       const_iterator operator--(int) {
           const_iterator tmp = *this;
           if (node == mp->nil) {
               node = mp->maximum(mp->root);
               if (!node) throw invalid_iterator();
           } else if (!node) {
               throw invalid_iterator();
           } else {
               if (node->left) {
                   node = mp->maximum(node->left);
               } else {
                   Node *p = node->parent;
                   while (p && node == p->left) {
                       node = p;
                       p = p->parent;
                   }
                   if (!p) throw invalid_iterator();
                   node = p;
               }
           }
           return tmp;
       }

       const_iterator &operator--() {
           if (node == mp->nil) {
               node = mp->maximum(mp->root);
               if (!node) throw invalid_iterator();
           } else if (!node) {
               throw invalid_iterator();
           } else {
               if (node->left) {
                   node = mp->maximum(node->left);
               } else {
                   Node *p = node->parent;
                   while (p && node == p->left) {
                       node = p;
                       p = p->parent;
                   }
                   if (!p) throw invalid_iterator();
                   node = p;
               }
           }
           return *this;
       }

       const value_type &operator*() const {
           if (!node || node == mp->nil) {
               throw invalid_iterator();
           }
           return *(node->data);
       }

       bool operator==(const iterator &rhs) const {
           return mp == rhs.mp && node == rhs.node;
       }

       bool operator==(const const_iterator &rhs) const {
           return mp == rhs.mp && node == rhs.node;
       }

       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       const value_type *operator->() const noexcept {
           return node->data;
       }
   };

   /**
  * two constructors
    */
   map() : root(nullptr), minNode(nullptr), maxNode(nullptr), count_size(0) {
       nil = new Node();
   }

   map(const map &other) : root(nullptr), count_size(other.count_size) {
       nil = new Node();
       root = copyTree(other.root, nullptr);
       minNode = minimum(root);
       maxNode = maximum(root);
   }

   /**
  * assignment operator
    */
   map &operator=(const map &other) {
       if (this == &other) return *this;
       clearTree(root);
       root = copyTree(other.root, nullptr);
       count_size = other.count_size;
       minNode = minimum(root);
       maxNode = maximum(root);
       return *this;
   }

   /**
  * Destructors
    */
   ~map() {
       clearTree(root);
       delete nil;
   }

   /**
  * access specified element with bounds checking
  * Returns a reference to the mapped value of the element with key equivalent to key.
  * If no such element exists, an exception of type `index_out_of_bound'
    */
   T &at(const Key &key) {
       Node *node = findNode(key);
       if (!node) throw index_out_of_bound();
       return node->data->second;
   }

   const T &at(const Key &key) const {
       Node *node = findNode(key);
       if (!node) throw index_out_of_bound();
       return node->data->second;
   }

   /**
  * access specified element
  * Returns a reference to the value that is mapped to a key equivalent to key,
  *   performing an insertion if such key does not already exist.
    */
   T &operator[](const Key &key) {
       Node *node = findNode(key);
       if (node) return node->data->second;

       auto result = insert(value_type(key, T()));
       return result.first.node->data->second;
   }

   /**
  * behave like at() throw index_out_of_bound if such key does not exist.
    */
   const T &operator[](const Key &key) const {
       Node *node = findNode(key);
       if (!node) throw index_out_of_bound();
       return node->data->second;
   }

   /**
  * return a iterator to the beginning
    */
   iterator begin() {
       return iterator(this, minNode);
   }

   const_iterator cbegin() const {
       return const_iterator(this, minNode);
   }

   /**
  * return a iterator to the end
  * in fact, it returns past-the-end.
    */
   iterator end() {
       return iterator(this, nil);
   }

   const_iterator cend() const {
       return const_iterator(this, nil);
   }

   /**
  * checks whether the container is empty
  * return true if empty, otherwise false.
    */
   bool empty() const {
       return count_size == 0;
   }

   /**
  * returns the number of elements.
    */
   size_t size() const {
       return count_size;
   }

   /**
  * clears the contents
    */
   void clear() {
       clearTree(root);
       root = nullptr;
       minNode = nullptr;
       maxNode = nullptr;
       count_size = 0;
   }

   /**
  * insert an element.
  * return a pair, the first of the pair is
  *   the iterator to the new element (or the element that prevented the insertion),
  *   the second one is true if insert successfully, or false.
    */
   pair<iterator, bool> insert(const value_type &value) {
       if (!root) {
           root = new Node(value);
           root->color = BLACK;
           minNode = maxNode = root;
           count_size = 1;
           return pair<iterator, bool>(iterator(this, root), true);
       }

       Node *current = root;
       Node *parent = nullptr;

       while (current) {
           parent = current;
           if (comp(value.first, current->data->first)) {
               current = current->left;
           } else if (comp(current->data->first, value.first)) {
               current = current->right;
           } else {
               return pair<iterator, bool>(iterator(this, current), false);
           }
       }

       Node *newNode = new Node(value, parent);
       if (comp(value.first, parent->data->first)) {
           parent->left = newNode;
       } else {
           parent->right = newNode;
       }

       // Update min/max cache
       if (!minNode || comp(value.first, minNode->data->first)) {
           minNode = newNode;
       }
       if (!maxNode || comp(maxNode->data->first, value.first)) {
           maxNode = newNode;
       }

       count_size++;
       insertFixup(newNode);

       return pair<iterator, bool>(iterator(this, newNode), true);
   }

   /**
  * erase the element at pos.
  *
  * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
    */
   void erase(iterator pos) {
       if (pos.mp != this || !pos.node || pos.node == nil) {
           throw invalid_iterator();
       }

       // Update min/max cache if necessary
       bool updateMin = (pos.node == minNode);
       bool updateMax = (pos.node == maxNode);

       Node *nextMin = nullptr;
       Node *nextMax = nullptr;

       if (updateMin && count_size > 1) {
           // Find successor of minNode
           if (pos.node->right) {
               nextMin = minimum(pos.node->right);
           } else {
               Node *p = pos.node->parent;
               Node *curr = pos.node;
               while (p && curr == p->right) {
                   curr = p;
                   p = p->parent;
               }
               nextMin = p;
           }
       }

       if (updateMax && count_size > 1) {
           // Find predecessor of maxNode
           if (pos.node->left) {
               nextMax = maximum(pos.node->left);
           } else {
               Node *p = pos.node->parent;
               Node *curr = pos.node;
               while (p && curr == p->left) {
                   curr = p;
                   p = p->parent;
               }
               nextMax = p;
           }
       }

       deleteNode(pos.node);
       count_size--;

       // Update caches
       if (count_size == 0) {
           minNode = maxNode = nullptr;
       } else {
           if (updateMin) minNode = nextMin;
           if (updateMax) maxNode = nextMax;
       }
   }

   /**
  * Returns the number of elements with key
  *   that compares equivalent to the specified argument,
  *   which is either 1 or 0
  *     since this container does not allow duplicates.
  * The default method of check the equivalence is !(a < b || b > a)
    */
   size_t count(const Key &key) const {
       return findNode(key) ? 1 : 0;
   }

   /**
  * Finds an element with key equivalent to key.
  * key value of the element to search for.
  * Iterator to an element with key equivalent to key.
  *   If no such element is found, past-the-end (see end()) iterator is returned.
    */
   iterator find(const Key &key) {
       Node *node = findNode(key);
       return node ? iterator(this, node) : end();
   }

   const_iterator find(const Key &key) const {
       Node *node = findNode(key);
       return node ? const_iterator(this, node) : cend();
   }
};

}

#endif
