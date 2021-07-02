#include <stdio.h>
#define KEY 21
int main()
{
	FILE *fin = fopen("encrypted.en", "rb");
	FILE *fout = fopen("decrypted.jpg", "wb");
	int mapping[8] = {1, 3, 5, 7, 0, 2, 4, 6};
	unsigned long long buf;
	while (fread(&buf, sizeof(unsigned long long), 1, fin))
	{
		union
		{
			unsigned long long qword;
			unsigned char bytes[8];
		} u;
		u.qword = buf;
		for (int i = 0; i < 8; i++)
			u.bytes[i] ^= KEY;
		buf = u.qword;
		// printf("%llx ", buf);
		unsigned long long ans = 0ull;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
			{
				int source = i * 8 + j;
				int target = mapping[i] + (7 - j) * 8;
				ans |= ((buf >> source) & 1) << (63 - target); // some endian-related problem
			}
		// printf("%llx\n", ans);
		fwrite(&ans, sizeof(unsigned long long), 1, fout);
	}
	fclose(fin);
	fclose(fout);
	return 0;
}
