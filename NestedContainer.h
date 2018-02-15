// NestedContainer.h

// David Adams
// February 2018

// This is a template that nests one container inside another and
// provides iteration over the elements in the inner containers.
// Construct from a nested conainer C@<c1> E.g.:
//   using MyBag = NestedContainer<std::map<std::vector<int>>>;

#ifndef NestedContainer_H
#define NestedContainer_H

template<typename C>
class NestedContainer {

public:
  using C2 = C;                        // Outer container
  using C1 = typename C2::value_type;  // Iner container
  using value_type = typename C1::value_type;
  using size_type = typename C2::size_type;

  class const_iterator {
  public:
    const_iterator(const NestedContainer& con, typename C2::const_iterator i2, typename C1::const_iterator i1)
    : m_pcon(&con), m_iter2(i2), m_iter1(i1) { }
    const NestedContainer& parent() const { return *m_pcon; }
    const C2& container() const { return parent().container(); };
    const typename C2::const_iterator& i2() const { return m_iter2; }
    const typename C1::const_iterator& i1() const { return m_iter1; }
    bool operator==(const const_iterator& rhs) const {
      return i2() == rhs.i2() && i1() == rhs.i1();
    }
    bool operator!=(const const_iterator& rhs) const {
      return i2() != rhs.i2() || i1() != rhs.i1();
    }
    const const_iterator& operator++() {
      ++m_iter1;
      if ( m_iter1 == m_iter2->end() ) {
        while ( ++m_iter2 != container().end() ) {
          if ( m_iter2->size() ) {
            m_iter1 = m_iter2->begin();
            break;
          }
        }
      }
      if ( m_iter2 == container().end() ) *this = parent().end();
      return *this;
    }
    const value_type& operator*() const {
      return *m_iter1;
    }
  private:
    const NestedContainer* m_pcon;
    typename C2::const_iterator m_iter2;
    typename C1::const_iterator m_iter1;
  };

  // Return the internal (outer) container.
  C2& container() { return m_c; }
  const C2& container() const { return m_c; }

  const size_type size() const {
    size_type sz = 0;
    for ( const C1& c1 : container() ) sz += c1.size();
    return sz;
  }

  const_iterator begin() const {
    typename C2::const_iterator i2 = container().begin();
    typename C1::const_iterator i1 = m_empty.begin();
    for ( ; i2 != container().end(); ++i2 ) {
      if ( i2->size() ) {
        i1 = i2->begin();
        break;
      }
    }
    return const_iterator(*this, i2, i1);
  }

  const_iterator end() const {
    typename C2::const_iterator i2 = container().end();
    typename C1::const_iterator i1 = m_empty.begin();
    return const_iterator(*this, i2, i1);
  }

private:

  C2 m_c;
  C1 m_empty;

};

#endif

  
