#include <iostream>
#include <list>
using namespace std;

class Problem {
public :
	Problem() : point_a(0), point_b(0) {}
	void run() {
		int tmp;
		while (cin >> tmp) {
			depot.push_back(tmp);
		}

		list<int>::iterator zero = depot.begin();
		list<int>::iterator half = depot.begin();
		std::advance(half, depot.size() / 2);

		for (; half != depot.end(); zero++, half++) {
			if (*zero > *half) {
				point_a++;
			}
			else if (*zero < *half) {
				point_b++;
			}
		}

		cout << point_a << ' ' << point_b << endl;
	}
private :
	list<int> depot;
	int point_a, point_b;
};

int main(void) {

	Problem ct;

	ct.run();

	return 0;
}