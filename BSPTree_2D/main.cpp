#include "bsp.h"
#include <stdlib.h>

int main()
{
	BSPTree* t = new BSPTree(1,33,1,33,3);
	t->add(4,10,4,16);
	t->add(10,24,4,9);
	t->add(7,19,23,27);
	t->add(22,28,13,24);
	t->build();
	t->print();
	printf("\n");
	t->levelOrder();
	system("pause");
}