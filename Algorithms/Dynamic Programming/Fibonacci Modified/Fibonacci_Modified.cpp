#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
using namespace std;

class Number {
	static const unsigned base = 10000U;
	std::list<unsigned> value;

	// used for inside of Number class.
	template<typename Iter>
	Number(Iter Lower, Iter Upper)
		: value(Lower, Upper) {}
public :
	Number(unsigned v = 0U);
	friend inline std::ostream& operator << (std::ostream& os, const Number& obj);
	friend inline Number operator + (const Number& lhs, const Number& rhs);
	friend inline Number operator * (const Number& lhs, const Number& rhs);
	friend inline bool operator == (const Number& lhs, const Number& rhs);
private :
	inline Number& shift(size_t n) {
		while (n--) {
			value.push_front(0U);
		}
		return *this;
	}
};

#include <chrono>
template<typename Task>
std::chrono::duration<double, std::milli> time_checker(Task f) {
	std::chrono::steady_clock::time_point tick = std::chrono::steady_clock::now();

	f();

	return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - tick);
}

int main(void) {
	unsigned a, b, n;
	cin >> a >> b >> n;

//	auto time_span = time_checker([&](){

		Number fibo[2] = {a, b};

//		cout << "0 : " << fibo[0] << endl;
//		cout << "1 : " << fibo[1] << endl;

		unsigned i = 2U;
		while (i < n) {
			fibo[i%2] = fibo[(i-1)%2]*fibo[(i-1)%2] + fibo[(i-2)%2];
//			cout << i << " : " << fibo[i%2] << endl;
//			cout << i << endl;
			++i;
		}

		cout << fibo[(n-1)%2] << endl;
//	});
//	cout << "elapsed time : " << time_span.count() << " milliseconds" << endl;

	return 0;
}

Number::Number(unsigned v/*=0U*/) {
	while (v >= base) {
		value.push_back(v%base);
		v /= base;
	}
	value.push_back(v%base);
}
inline Number operator + (const Number& lhs, const Number& rhs) {
	auto simpleadd = [&](const Number& longer, const Number& shorter) -> Number {
		std::list<unsigned> result(longer.value.cbegin(), longer.value.cend());
		auto rit = result.begin();
		auto sit = shorter.value.cbegin();
		unsigned carry = 0U;
		while (sit != shorter.value.cend() || carry != 0U) {
			if (rit == result.end()) {
				result.push_back(0U);
				rit = std::prev(result.end(), 1);
			}
			*rit += carry;
			if (sit != shorter.value.cend()) {
				*rit += *sit;
				++sit;
			}
			carry = *rit / Number::base;
			*rit = *rit % Number::base;
			++rit;
		}
		return Number(result.begin(), result.end());
	};

	if (lhs.value.size() > rhs.value.size())
		return simpleadd(lhs, rhs);
	else
		return simpleadd(rhs, lhs);
}
inline Number operator * (const Number& lhs, const Number& rhs) {
	if (lhs.value.size() == 1 && rhs.value.size() == 1) {
		return Number(lhs.value.front()*rhs.value.front());
	}
	else if (lhs == rhs) {
		size_t size = lhs.value.size();
		size_t n = size/2;

		auto First = lhs.value.cbegin();
		auto Last = lhs.value.cend();

		Number High(std::next(First, n), Last);
		Number Low(First, std::next(First, n));

		Number A(High*High);
		Number B(Low*Low);
		Number C(2*High*Low);

		return A.shift(2*n) + B + C.shift(n);
	}
	else if (lhs.value.size() != rhs.value.size()) {
		auto simple = [&](const Number& longer, const Number& shorter) -> Number {
			std::list<unsigned> result(lhs.value.size()+rhs.value.size()-1,0U);
			auto sit = shorter.value.cbegin();
			while (sit != shorter.value.cend()) {
				auto target = result.begin();
				auto diff = std::distance(shorter.value.cbegin(), sit);
				while (diff--) {
					++target;
				}
				unsigned carry = 0U;
				auto lit = longer.value.cbegin();
				while (lit != longer.value.cend() || carry != 0U) {
					if (target == result.end()) {
						result.push_back(0U);
						target = std::prev(result.end(), 1);
					}
					*target += carry;
					if (lit != longer.value.cend()) {
						*target += (*lit) * (*sit);
						++lit;
					}
					carry = *target / Number::base;
					*target = *target % Number::base;
					++target;
				}
				++sit;
			}
			return Number(result.begin(), result.end());
		};

		if (rhs.value.size() > lhs.value.size())
			return simple(rhs, lhs);
		else
			return simple(lhs, rhs);
	}

	size_t max_size = std::max(lhs.value.size(), rhs.value.size());
	size_t n = max_size/2;

	auto Left_First = lhs.value.cbegin();
	auto Left_Last = lhs.value.cend();

	auto Right_First = rhs.value.cbegin();
	auto Right_Last = rhs.value.cend();

	Number Left_High(std::next(Left_First, n), Left_Last);
	Number Left_Low(Left_First, std::next(Left_First, n));

	Number Right_High(std::next(Right_First, n), Right_Last);
	Number Right_Low(Right_First, std::next(Right_First, n));

	Number A = Left_High*Right_High;
	Number B = Left_Low*Right_Low;
	Number C = Left_High*Right_Low + Left_Low*Right_High;

	Number R = A.shift(2*n) + B + C.shift(n);

	return R;
}
inline std::ostream& operator << (std::ostream& os, const Number& obj) {
	auto it = obj.value.crbegin();
	os << *it++;
	while (it != obj.value.crend()) {
		os << std::setw(4) << std::setfill('0') << *it;
		++it;
	}
	return os;
}
inline bool operator == (const Number& lhs, const Number& rhs) {
	if (lhs.value.size() != rhs.value.size())
		return false;
	auto lit = lhs.value.cbegin();
	auto rit = rhs.value.cbegin();
	while (lit != lhs.value.cend()) {
		if (*lit != *rit)
			return false;

		++lit;
		++rit;
	}
	return true;
}

