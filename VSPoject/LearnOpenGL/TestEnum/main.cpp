#include <iostream>
enum  testEnum
{
	A,B,C
};
void test(testEnum e)
{
	if(e== A)
		std::cout << e<< std::endl;
}
void main()
{
	testEnum ee = testEnum::B;
	test(ee);
	test(B);
	
}