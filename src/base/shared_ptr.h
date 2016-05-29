//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/scoped_ptr.htm for documentation.
//

//  scoped_ptr mimics a built-in pointer except that it guarantees deletion
//  of the object pointed to, either on destruction of the scoped_ptr or via
//  an explicit reset(). scoped_ptr is a simple solution for simple needs;
//  use shared_ptr or std::auto_ptr if your needs are more complex.

//  scoped_ptr_malloc added in by Google.  When one of
//  these goes out of scope, instead of doing a delete or delete[], it
//  calls free().  scoped_ptr_malloc<char> is likely to see much more
//  use than any other specializations.

//  release() added in by Google. Use this to conditionally
//  transfer ownership of a heap-allocated object to the caller, usually on
//  method success.
#ifndef VZBASE_BASE_SHARED_PTR_H__
#define VZBASE_BASE_SHARED_PTR_H__

#include <algorithm>      // std::swap
#include <cstddef>        // for std::ptrdiff_t
#include <stdlib.h>       // for free() decl

#include "base/common.h"  // for ASSERT

#ifdef _WIN32
namespace std {
using ::ptrdiff_t;
};
#endif // _WIN32

namespace vzsdk {
/**
* implementation of reference counter for the following minimal smart pointer.
*
* shared_ptr_count is a container for the allocated pn reference counter.
*/
class shared_ptr_count {
 public:
  shared_ptr_count() :
    pn(NULL) {
  }
  shared_ptr_count(const shared_ptr_count& count) :
    pn(count.pn) {
  }
  // Swap method for the copy-and-swap idiom (copy constructor and swap method)
  void swap(shared_ptr_count& lhs) throw() { // never throws
    std::swap(pn, lhs.pn);
  }
  // getter of the underlying reference counter
  long use_count(void) const throw() { // never throws
    long count = 0;
    if (NULL != pn) {
      count = *pn;
    }
    return count;
  }
  // Acquire/share the ownership of the pointer,
  // initializing the reference counter
  template<class U>
  void acquire(U* p) { // may throw std::bad_alloc
    if (NULL != p) {
      if (NULL == pn) {
        try {
          pn = new long(1); // may throw std::bad_alloc
        } catch (std::bad_alloc&) {
          delete p;
          throw; // rethrow the std::bad_alloc
        }
      } else {
        ++(*pn);
      }
    }
  }
  // release the ownership of the px pointer,
  // destroying the object when appropriate
  template<class U>
  void release(U* p) throw() { // never throws
    if (NULL != pn) {
      --(*pn);
      if (0 == *pn) {
        delete p;
        delete pn;
      }
      pn = NULL;
    }
  }

 public:
  long*   pn; //!< Reference counter
};


/**
 * minimal implementation of smart pointer, a subset of the C++11 s
 * td::shared_ptr or boost::shared_ptr.
 *
 * shared_ptr is a smart pointer retaining ownership of an object through a
 * provided pointer, and sharing this ownership with a reference counter.
 * It destroys the object when the last shared pointer pointing to it is
 * destroyed or reset.
 */
template<class T>
class shared_ptr {
 public:
  /// The type of the managed object, aliased as member type
  typedef T element_type;

  // Default constructor
  shared_ptr(void) throw() : // never throws
    px(NULL),
    pn() {
  }
  // Constructor with the provided pointer to manage
  explicit shared_ptr(T* p) : // may throw std::bad_alloc
    // px(p), would be unsafe as acquire() may throw, which would call release()
    // in destructor
    pn() {
    acquire(p);   // may throw std::bad_alloc
  }
  // Constructor to share ownership. Warning : to be used for pointer_cast
  // only ! (does not manage two separate <T> and <U> pointers)
  template <class U>
  shared_ptr(const shared_ptr<U>& ptr, T* p) :
    // px(p), would be unsafe as acquire() may throw,
    // which would call release() in destructor
    pn(ptr.pn) {
    acquire(p);   // may throw std::bad_alloc
  }
  // Copy constructor to convert from another pointer type
  // never throws (see comment below)
  template <class U>
  shared_ptr(const shared_ptr<U>& ptr) throw() :
    //px(ptr.px),
    pn(ptr.pn) {
    // must be coherent : no allocation allowed in this path
    ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count()));
    // will never throw std::bad_alloc
    acquire(static_cast<typename shared_ptr<T>::element_type*>(ptr.px));
  }
  // Copy constructor (used by the copy-and-swap idiom)
  // never throws (see comment below)
  shared_ptr(const shared_ptr& ptr) throw() :
    //px(ptr.px),
    pn(ptr.pn) {
    // must be coh¨¦rent : no allocation allowed in this path
    ASSERT((NULL == ptr.px) || (0 != ptr.pn.use_count()));
    acquire(ptr.px);   // will never throw std::bad_alloc
  }
  // Assignment operator using the copy-and-swap idiom
  // (copy constructor and swap method)
  // never throws
  shared_ptr& operator=(shared_ptr ptr) throw() {
    swap(ptr);
    return *this;
  }
  // the destructor releases its ownership
  // never throws
  inline ~shared_ptr(void) throw() {
    release();
  }
  // this reset releases its ownership
  inline void reset(void) throw() { // never throws
    release();
  }
  // this reset release its ownership and re-acquire another one
  // may throw std::bad_alloc
  void reset(T* p) {
    // auto-reset not allowed
    ASSERT((NULL == p) || (px != p));
    release();
    // may throw std::bad_alloc
    acquire(p);
  }

  // Swap method for the copy-and-swap idiom (copy constructor and swap method)
  // never throws
  void swap(shared_ptr& lhs) throw() {
    std::swap(px, lhs.px);
    pn.swap(lhs.pn);
  }

  // reference counter operations :
  // never throws
  inline operator bool() const throw() {
    return (0 < pn.use_count());
  }
  // never throws
  inline bool unique(void)  const throw() {
    return (1 == pn.use_count());
  }
  // never throws
  long use_count(void)  const throw() {
    return pn.use_count();
  }

  // underlying pointer operations :
  // never throws
  inline T& operator*()  const throw() {
    ASSERT(NULL != px);
    return *px;
  }
  // never throws
  inline T* operator->() const throw() {
    ASSERT(NULL != px);
    return px;
  }
  // never throws
  inline T* get(void)  const throw() {
    // no assert, can return NULL
    return px;
  }

 private:
  // acquire/share the ownership of the px pointer,
  // initializing the reference counter
  // may throw std::bad_alloc
  inline void acquire(T* p) {
    // may throw std::bad_alloc
    pn.acquire(p);
    // here it is safe to acquire the ownership of the provided raw pointer,
    // where exception cannot be thrown any more
    px = p;
  }

  // release the ownership of the px pointer,
  // destroying the object when appropriate
  inline void release(void) throw() { // never throws
    pn.release(px);
    px = NULL;
  }

 private:
  // This allow pointer_cast functions to share the reference counter
  // between different shared_ptr types
  template<class U>
  friend class shared_ptr;

 private:
  T*                  px; //!< Native pointer
  shared_ptr_count    pn; //!< Reference counter
};


// comparaison operators
// never throws
template<class T, class U>
inline bool operator==(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() {
  return (l.get() == r.get());
}
// never throws
template<class T, class U>
inline bool operator!=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() {
  return (l.get() != r.get());
}
// never throws
template<class T, class U>
inline bool operator<=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() {
  return (l.get() <= r.get());
}
// never throws
template<class T, class U>
inline bool operator<(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() {
  return (l.get() < r.get());
}
// never throws
template<class T, class U>
inline bool operator>=(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() {
  return (l.get() >= r.get());
}
// never throws
template<class T, class U>
inline bool operator>(const shared_ptr<T>& l, const shared_ptr<U>& r) throw() {
  return (l.get() > r.get());
}



// static cast of shared_ptr
// never throws
template<class T, class U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& ptr) {
  return shared_ptr<T>(ptr, static_cast<
                       typename shared_ptr<T>::element_type*>(ptr.get()));
}

// dynamic cast of shared_ptr
// never throws
template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& ptr) {
  T* p = dynamic_cast<typename shared_ptr<T>::element_type*>(ptr.get());
  if (NULL != p) {
    return shared_ptr<T>(ptr, p);
  } else {
    return shared_ptr<T>();
  }
}
} // namespace vzsdk

#endif  // #ifndef VZBASE_BASE_SHARED_PTR_H__
