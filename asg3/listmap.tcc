// $Id: listmap.tcc,v 1.9 2016-07-20 21:00:33-07 - - $

#include "listmap.h"
#include "trace.h"
//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                            const value_type& value):
         link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', reinterpret_cast<const void*> (this));
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   iterator iter;
   Less less;

   iter = begin();
   while ( iter != end() and less(iter->first, pair.first) ){
      ++iter;
   }

   if (iter != end() and not less(pair.first, iter->first)) {
      iter->second = pair.second;
      return iter;
   }
   node* n = new node(iter.where, iter.where->prev, pair);
      iter.where->prev->next = n;
      iter.where->prev = n;
      return iterator();
   }

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   TRACE ('l', that);
   iterator iter;
   Less less;

   iter = begin();
   while ( iter != end() and less(iter->first, that) ){
      ++iter;
    }

    if (iter != end() and  not less(that, iter->first)) {
      cout << *iter << endl;
       return iter;
    }
    else{
       cerr << that << ": key not found" << endl;

    }

    return end();
  }

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);
   //Hold on to the next node to return
   iterator iter = position.where->next;
   //Set previous node's next pointer to the node after position.
   //Set next node's previous pointer to the node before position.
   position.where->prev->next = position.where->next;
   position.where->next->prev = position.where->prev;
   //Remove position from listmap.
   delete position.where;
   return iter;
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
         (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
         (const iterator& that) const {
   return this->where != that.where;
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::printVal (const mapped_type& that) {
   for(iterator itor = begin(); itor != end(); ++itor) {
      if (that == itor->second) cout << *itor << endl;
   }
}

template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::print () {
   for(iterator itor = begin(); itor != end(); ++itor) cout << *itor << endl;
}

//
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::remove (const key_type& that) {
   TRACE ('l', that);
   iterator itor = find(that);
   Less less;
   if (itor != end() and not less(that, itor->first)) erase(itor);
   else cerr << that << ": key not found" << endl;
}
