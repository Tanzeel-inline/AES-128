#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <cstring>
#include <cmath>
using namespace std;
static unsigned char sbox[256] = {
	   0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	   0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	   0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	   0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	   0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	   0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	   0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	   0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	   0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	   0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	   0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	   0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	   0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	   0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	   0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	   0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
const int size_mix_column = 4;
int mix_column[size_mix_column][size_mix_column] =
{
{2,3,1,1},
{1,2,3,1},
{1,1,2,3},
{3,1,1,2}
};
void tranpose(unsigned char* key, const int size)
{
	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size; ++j)
			swap(key[size * i + j], key[size * j + i]);
}
unsigned char mul2(unsigned char value)
{
	unsigned char temp = value;
	value = value << 1;
	if (value >= 256)
	{
		value -= 256;
	}
	if (temp >= 128)
	{
		value ^= 27;
	}
	return value;
}
unsigned char mul3(unsigned char value)
{
	unsigned char temp = value;
	value = value << 1;
	if (value >= 256)
	{
		value -= 256;
	}
	if (temp >= 128)
	{
		value ^= 27;
	}
	value ^= temp;
	return value;
}
void KeyExpansion(unsigned char* cipherkey, unsigned char* expandedkey) {

	for (int i = 0; i < 16; i++)
	{
		expandedkey[i] = cipherkey[i];
	}
	for (int i = 1; i <= 10; i++)
	{
		unsigned char* temp = new unsigned char[4];
		temp[0] = expandedkey[((i - 1) * 16) + 13];
		temp[1] = expandedkey[((i - 1) * 16) + 14];
		temp[2] = expandedkey[((i - 1) * 16) + 15];
		temp[3] = expandedkey[((i - 1) * 16) + 12];
		for (int j = 0; j < 4; j++)
		{
			temp[j] = sbox[temp[j]];
		}
		int k;
		if (i < 9)
		{
			k = pow(2, i - 1);
		}
		else if (i == 9)
		{
			k = 27;
		}
		else
		{
			k = 54;
		}
		cout << "Round Constant:" << hex << k << endl;
		temp[0] ^= k;

		for (int j = 0; j < 16; j++)
		{
			if (j < 4)
			{
				expandedkey[(i * 16) + j] = expandedkey[((i - 1) * 16) + j] ^ temp[j];
				continue;
			}
			expandedkey[(i * 16) + j] = expandedkey[((i - 1) * 16) + j] ^ expandedkey[(i * 16) + (j - 4)];
		}
		for (int j = (i * 16); j < (i + 1) * 16; j++)
		{
			cout << hex << static_cast<int>(expandedkey[j]) << " ";
		}
		cout << endl;
	}
}
void addRoundKey(unsigned char* state, unsigned char* roundKey) {
	//system("cls");
	for (int i = 0; i < 16; i++)
	{
		state[i] ^= roundKey[i];
		cout << hex << static_cast<int>(state[i]) << " ";
	}
	cout << endl;
}
void SubBytes(unsigned char* state) {
	for (int i = 0; i < 16; i++)
	{
		state[i] = sbox[static_cast<int>(state[i])];
		cout << hex << static_cast<int>(state[i]) << " ";
	}
	cout << endl;
}
void MixColumn(unsigned char* state)
{
	unsigned char** temp_array = new unsigned char* [4];
	for (int i = 0; i < 4; i++)
	{
		temp_array[i] = new unsigned char[4];
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			unsigned char product = 0;
			for (int k = 0; k < 4; k++)
			{
				if (k == 0)
				{
					if (mix_column[i][k] == 1)
					{
						product = state[(k * 4) + j];
					}
					else if (mix_column[i][k] == 2)
					{
						product = mul2(state[(k * 4) + j]);
					}
					else
					{
						product = mul3(state[(k * 4) + j]);
					}

				}
				else
				{
					if (mix_column[i][k] == 1)
					{
						product ^= state[(k * 4) + j];
					}
					else if (mix_column[i][k] == 2)
					{
						product ^= mul2(state[(k * 4) + j]);
					}
					else
					{
						product ^= mul3(state[(k * 4) + j]);
					}
				}
			}
			temp_array[i][j] = product;
		}
	}
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			state[(i * 4) + j] = temp_array[i][j];
			cout << hex << static_cast<int> (state[(i * 4) + j]) << " ";
		}
	}
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		delete[] temp_array[i];
	}
	delete[] temp_array;
}
void ShiftRows(unsigned char* state) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < i; j++)
		{
			unsigned char temp = state[0 + (i * 4)];
			state[0 + (i * 4)] = state[1 + (i * 4)];
			state[1 + (i * 4)] = state[2 + (i * 4)];
			state[2 + (i * 4)] = state[3 + (i * 4)];
			state[3 + (i * 4)] = temp;
		}
	}
	for (int i = 0; i < 16; i++)
	{
		cout << hex << static_cast<int>(state[i]) << " ";
	}
	cout << endl;
}
void Encrypt(unsigned char* plaintext, unsigned char* cipherkey, unsigned char* ciphertext) {
	unsigned char* expandedKey = new unsigned char[176];
	for (int i = 0; i < 16; i++)
	{
		ciphertext[i] = plaintext[i];
	}
	tranpose(ciphertext, 4);
	KeyExpansion(cipherkey, expandedKey);
	tranpose(cipherkey, 4);
	addRoundKey(ciphertext, cipherkey);
	for (int i = 1; i <= 10; i++)
	{
		SubBytes(ciphertext);
		ShiftRows(ciphertext);
		if (i != 10)
		{
			MixColumn(ciphertext);
		}
		for (int j = 0; j < 16; j++)
		{
			cipherkey[j] = expandedKey[(i * 16) + j];
		}
		tranpose(cipherkey, 4);
		addRoundKey(ciphertext, cipherkey);


	}
	tranpose(ciphertext, 4);
	for (int i = 0; i < 16; i++)
	{
		cout << hex << static_cast<int>(ciphertext[i]) << " ";
	}
	cout << endl;
}