#include <iostream>

using namespace std;


class Fake{
	Fake(){
		cout << "It's fake class\n";
	}

};

int main()
{
	Fake f1;
    return 0;
}
