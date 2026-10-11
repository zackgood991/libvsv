/* simple VSV example - 42 * 2^i */

#include <cstdio>
#include <vector>

int main(int argc, char** argv)
{
	int i;
	int topush = 42;
	int gotten;
	std::vector<int> vec = {};

	for(i = 0; i < 10; i++)
	{
		vec.push_back(topush);
		if (i == 0)
			printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());

		topush *= 2;
	}
	vec.insert(vec.begin() + 10, 3, topush);

loop:
	for(i = 0; i < vec.size(); i++)
	{
		gotten = vec[i];
		printf("vec[%4d/%lu]: %d\n", i, vec.size(), gotten);
	}
	vec.pop_back();
	if(vec.size() > 0)
		goto loop;


	printf("...\n");

	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());
	vec.resize(56, topush);
	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());
	for(i = 0; i < vec.size(); i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vec.size(), vec[i]);
		if(i >= 10) vec[i] += i;
	}
	vec.resize(58);
	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());
	for(i = 0; i < vec.size(); i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vec.size(), vec[i]);
	}
	vec.erase(vec.begin()+55, vec.begin()+55+2);
	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());
	for(i = 0; i < vec.size(); i++)
	{
		printf("vec[%4d/%lu]: %d\n", i, vec.size(), vec[i]);
	}
	vec.clear();
	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());
	vec.push_back(topush);
	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());
	vec.shrink_to_fit();
	printf("size: %lu, cap: %lu\n", vec.size(), vec.capacity());

	return 0;
}
