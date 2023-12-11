#include <iostream>
using namespace std;

#define pos(x,y) (((y)<<3)|(x))

int main()
{
	cout<<pos(2,0)<<endl;
	cout<<pos(2,1)<<endl;
	return 0;
}
