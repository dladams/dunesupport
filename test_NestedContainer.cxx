// test_NestedContainer.cxx

#include "NestedContainer.h"
#include <string>
#include <iostream>
#include <iomanip>

using std::string;
using std::cout;
using std::endl;
using std::setw;

namespace {

  using Index = unsigned int;

  template<typename T1, typename T2>
  Index check(const T1& val, const T2& exp, string msg) {
    Index chk = val != exp;
    if ( msg.size() ) {
      cout << setw(30) << msg << ": " << val << " == " << exp;
      if ( chk ) cout << " FAILED!";
      cout << endl;
    }
    return chk;
  }

  int test1() {
    string myname = "test1: ";
    Index err = 0;
    cout << myname << "Construct container." << endl;
    using Con = NestedContainer<vector<vector<int>>>;
    Con vals;
    err += check(vals.size(), 0, "empty size");
    vals.container().push_back({1});
    err += check(vals.size(), 1, "size 1");
    vals.container().push_back({11, 12});
    err += check(vals.size(), 3, "size 2");
    vals.container().push_back({21, 22, 23});
    err += check(vals.size(), 6, "size 3");
    cout << myname << "Check values" << endl;
    vector<int> chkvals = {1, 11, 12, 21, 22, 23};
    err += check(chkvals.size(), 6, "check");
    Index ichk = 0;
    cout << myname << "Begin/end iteration" << endl;
    for ( Con::const_iterator ival=vals.begin(); ival!=vals.end(); ++ival ) {
      check(*ival, chkvals[ichk], "check value");
      ++ichk;
    }
    check(ichk, chkvals.size(), "check count");
    cout << myname << "Range base iteration" << endl;
    ichk = 0;
    for ( int val : vals ) {
      check(val, chkvals[ichk], "check value");
      ++ichk;
    }
    check(ichk, chkvals.size(), "check count");
    return err;
  }

}

int test_NestedContainer() {
  Index err = 0;
  err += test1();
  return err;
}
