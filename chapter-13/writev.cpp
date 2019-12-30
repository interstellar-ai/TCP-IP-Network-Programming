#include <iostream>
#include <sys/uio.h>

using namespace std;

int main(int argc, char const *argv[])
{
	struct iovec vec[2];
	char buf1[] = "ABCDEFG";
	char buf2[] = "123456789";

	int str_len;

	vec[0].iov_base = buf1;
	vec[0].iov_len = sizeof(buf1);

	vec[1].iov_base = buf2;
	vec[1].iov_len = sizeof(buf2);

	str_len = writev(1, vec, 2);
	cout << endl;
	cout << "str_len: " << str_len << endl;
	return 0;
}