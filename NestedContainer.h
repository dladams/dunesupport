// NestedContainer.h

// David Adams
// February 2018

// This is a template that nests one container inside another and
// provides iteration over the elements in the inner containers.
// Construct from a nested conainer C@<c1> E.g.:
//   using MyBag = NestedContainer<std::map<std::vector<int>>>;

#ifndef NestedContainer_H
#define NestedContainer_H

//----------------------------------------------------------------------

// ContainerWrap wraps stl containers to give them a more uniform interface.
// For simple containers like vector<T>, the key, value and element are the same
// and have type T.
// Mapped containers like map<K,T> map use Keys of type K to map values of type T
// and have elements of type std::pair<K,T>.

template<typename C>
class ContainerWrap {
public:
  using Container = C;
  using Element = typename Container::value_type;
  using Key = Element;
  using Value = Element;
  static const Key& key(const Value& val) { return val; }
  static const Value& value(const Value& val) { return val; }
  static const Element& element(const Value& val) { return val; }
};

template<typename K, typename T>
class ContainerWrap<std::map<K,T>> {
public:
  using Container = std::map<K,T>;
  using Key = K;
  using Value = T;
  using Element = typename Container::value_type;
  static const Key& key(const Element& ele) { return ele.first; }
  static const Value& value(const Element& ele) { return ele.second; }
  static const Element& element(const Value& ele) { return ele; }
};

//----------------------------------------------------------------------

// Nested container C1<C2>
//
// Use C::const_iterator ival to iterate over all values in the nested container
// and the following to access elements:
//  icon.value() is the value in the inner container
//  icon.key1() is the key for the outer container.
//  icon.key2() is the key for the inner container
template<typename C>
class NestedContainer {

public:
  using C1 = C;                        // Outer container
  using WC1 = ContainerWrap<C1>;
  using C2 = typename WC1::Value;      // Inner container
  using WC2 = ContainerWrap<C2>;
  using Key1 = typename WC1::Key;       // Key type for outer container
  using Key2 = typename WC2::Key;       // Key type inner container
  using Value = typename WC2::Value;
  using value_type = typename C2::value_type;
  using size_type = typename C1::size_type;

  class const_iterator {
  public:
    const_iterator(const NestedContainer& con, typename C1::const_iterator i1, typename C2::const_iterator i2)
    : m_pcon(&con), m_iter1(i1), m_iter2(i2) { }
    const NestedContainer& parent() const { return *m_pcon; }
    const C1& container() const { return parent().container(); };
    const typename C1::const_iterator& i1() const { return m_iter1; }
    const typename C2::const_iterator& i2() const { return m_iter2; }
    const Key1& key1() const { return WC1::key(*m_iter1); }
    const Key2& key2() const { return WC2::key(*m_iter2); }
    const Value& value() const { return WC2::value(*m_iter2); }
    bool operator==(const const_iterator& rhs) const {
      return i1() == rhs.i1() && i2() == rhs.i2();
    }
    bool operator!=(const const_iterator& rhs) const {
      return i1() != rhs.i1() || i2() != rhs.i2();
    }
    const const_iterator& operator++() {
      ++m_iter2;
      const C2& c2 = WC1::value(*m_iter1);
      if ( m_iter2 == c2.end() ) {
        while ( ++m_iter1 != container().end() ) {
          const C2& c2 = WC1::value(*m_iter1);
          if ( c2.size() ) {
            m_iter2 = c2.begin();
            break;
          }
        }
      }
      if ( m_iter1 == container().end() ) *this = parent().end();
      return *this;
    }
    const value_type& operator*() const {
      return *m_iter2;
    }
  private:
    const NestedContainer* m_pcon;
    typename C1::const_iterator m_iter1;
    typename C2::const_iterator m_iter2;
  };

  // Return the internal (outer) container.
  C1& container() { return m_c; }
  const C1& container() const { return m_c; }

  const size_type size() const {
    size_type sz = 0;
    for ( const typename C1::value_type& e1 : container() ) sz += WC1::value(e1).size();
    return sz;
  }

  const_iterator begin() const {
    typename C1::const_iterator i1 = container().begin();
    typename C2::const_iterator i2 = m_empty.begin();
    for ( ; i1 != container().end(); ++i1 ) {
      const C2& c2 = WC1::value(*i1);
      if ( c2.size() ) {
        i2 = c2.begin();
        break;
      }
    }
    return const_iterator(*this, i1, i2);
  }

  const_iterator end() const {
    typename C1::const_iterator i1 = container().end();
    typename C2::const_iterator i2 = m_empty.begin();
    return const_iterator(*this, i1, i2);
  }

private:

  C1 m_c;
  C2 m_empty;

};

#endif

  
