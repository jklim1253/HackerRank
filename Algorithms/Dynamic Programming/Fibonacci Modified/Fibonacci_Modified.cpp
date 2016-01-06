#include <iostream>
#include <algorithm>
#include <iomanip>
#include <list>
using namespace std;

class Number {
	static const unsigned base = 10000U;		// 0000 ~ 9999
	list<unsigned> value;
public :
	Number(unsigned v = 0U) {
		while (v >= base) {
			value.push_back(v%base);
			v /= base;
		}
		value.push_back(v);
	}
	template<typename Iter>
	Number(Iter Last, Iter First) : value(Last, First) {}
	friend ostream& operator << (ostream& os, const Number& obj) {
		auto it = obj.value.crbegin();
		os << *it++;
		while (it != obj.value.crend()) {
			os << setw(4) << setfill('0') << *it++;
		}
		return os;
	}
	friend Number operator + (const Number& lhs, const Number& rhs) {
		list<unsigned> r;
		unsigned tmp = 0U, carry = 0U;
		auto lit = lhs.value.cbegin();
		auto rit = rhs.value.cbegin();
		while (lit != lhs.value.cend() || rit != rhs.value.cend() || carry != 0U) {
			tmp = carry;
			if (lit != lhs.value.cend()) {
				tmp += *lit;
				++lit;
			}
			if (rit != rhs.value.cend()) {
				tmp += *rit;
				++rit;
			}
			r.push_back(tmp%base);
			carry = tmp/base;
		}
		return Number(r.begin(), r.end());
	}
	friend Number operator * (const Number& lhs, const Number& rhs) {
		list<unsigned> unit(1,0U);

		auto lit = lhs.value.cbegin();
		while (lit != lhs.value.cend()) {
			auto target = unit.begin();
			auto diff = std::distance(lhs.value.cbegin(), lit);
			while (diff--) {
				++target;
			}
			unsigned tmp = 0U, carry = 0U;
			auto rit = rhs.value.cbegin();
			while (rit != rhs.value.cend() || carry != 0U) {
				if (target == unit.end()) {
					unit.push_back(0U);
					target = std::next(unit.end(), -1);
//					cout << "need expand" << unit.size() << endl;
				}
				tmp = carry + *target;
//				cout << "tmp : " << tmp << ", carry : " << carry << ",target : " << *target << endl;
				if (rit != rhs.value.cend()) {
					tmp += (*lit) * (*rit);
					++rit;
				}
				*target = tmp%base;
				//unit.push_back(tmp%base);
				carry = tmp/base;
//				cout << "tmp : " << tmp << ", carry : " << carry << ",target : " << *target << endl;
				++target;
			}
			++lit;
		}
		return Number(unit.begin(), unit.end());
	}
	Number squared(void) {
		// very big number : A = A_high*base^n + A_low
		// A*A => A_high*A_high*base^2n + A_low*A_low + 2*A_high*A_low*base^n
		if (value.size() < 10);
		return Number();
	}
};

int main(void) {
//	unsigned a, b, n;
//	cin >> a >> b >> n;
//
//	Number fibo[2] = {a, b};
//
//	cout << "0 : " << fibo[0] << endl;
//	cout << "1 : " << fibo[1] << endl;
//
//	unsigned i = 2U;
//	while (i < n) {
//		fibo[i%2] = fibo[(i-1)%2].squared() + fibo[(i-2)%2];
//		cout << i << " : " << fibo[i%2] << endl;
//
//		++i;
//	}
//
//	cout << fibo[(n-1)%2] << endl;

	// Number class operator Test
	Number a = 2;

	for (int i = 0; i < 10; ++i) {
		cout << a << endl;
		a = a * a;
	}
	return 0;
}
