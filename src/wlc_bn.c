#include "../inc/wlc_bn.h"
#include "../inc/wlc_rand.h"

/**
 * Set zero to a big number.
 * С�˿�ʼһ������ֵ��0
 *
 * @param[out] x			- the big number to assign.
 * @param[in] digs			- the number of digits.
 */
void bn_set_zero(dig_t* x, int digs)
{
	for (int i = 0; i < digs; i++)
	{
		*x++ = 0;
	}
}

/**
*	��С�˵����һ������ֵ�����
* @param[in] x - the big number to assign,here is used to make seed
*/
void bn_rand(dig_t* x, int digs)
{
	//��ʼ��seed
	rand_seed(x, digs);

	for (int i = 0; i < digs; i++)
	{
		*x++ = rand();
	}

}

/**
 * Copy the second big number to the first big number.
 * ���ڶ����������Ƶ���һ������
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number to copy.
 * @param[in] digs			- the number of digits.
 */
void bn_copy(dig_t* r, const dig_t* x, int digs)
{

	for (int i = 0; i < digs; i++)
	{
		r[i] = x[i];
	}
}

/**
 * get the number of bits of a big number.
 * ��һ��������λ��
 *
 * @param[in] x             - the big number to get the number of bits.
 * @param[in] digs          - the number of digits.
 * @return the number of bits of the big number x.
 */
int bn_get_bits(const dig_t* x, int digs)
{
	dig_t* constx = (dig_t*)calloc(digs, sizeof(dig_t));
	if (constx)
	{
		bn_copy(constx, x, digs);
		int bits = 0;

		int cnt = digs - 1;

		while (!constx[cnt])
		{
			cnt--;
		}

		for (; cnt >= 0; cnt--)
		{
			while (constx[cnt] > 0)
			{
				constx[cnt] >>= 1;
				bits ++;
			}
			break;			
		}

		bits += 32 * cnt;
		free(constx);
		return bits;
	}
	else
	{
		return -1;
	}
}


/**
 * Add two big numbers of the same size.
 * Computes r = x + y.
 *
 * @param[out] r			- the result.���
 * @param[in] x				- the first big number to add.��һ������
 * @param[in] y				- the second big number to add.�ڶ�������
 * @param[in] digs			- the number of digits.�ֽ���
 * @return the carry of the last digit addition.
 */
dig_t bn_add(dig_t* r, const dig_t* x, const dig_t* y, int digs)
{
	int i = 0;
	register udi_t carry = 0;
	for (i = 0; i < digs; i++)
	{
		carry += (udi_t)(*x++) + (udi_t)(*y++);
		(*r++) = (dig_t)carry;
		carry >>= 32;
	}
	return (dig_t)carry;
}


/**
 * Subtract a big number from another.
 * ����һ������ȥһ����
 * Compute r = x - y.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number.
 * @param[in] y				- the big number to subtract.
 * @param[in] digs			- the number of digits.
 * @return the carry of the last digit subtraction.�������������1
 */
dig_t bn_sub(dig_t* r, const dig_t* x, const dig_t* y, int digs)
{
	register udi_t temp = 0;//��ʱ�����һλ�Ľ��<<32
	register udi_t carry = 0;//��һλ*256����Ŀǰ��һλ

	dig_t* tempy = (dig_t*)calloc(digs, sizeof(dig_t));//��ʱ�ļ���

	if (tempy)//�ɹ�������
	{
		bn_set_zero(tempy, digs);
		bn_copy(tempy, y, digs);

		carry += (udi_t)x[0];

		short int isoutspan = 0;//�����Ƿ����f��ͷ������
		for (int cnt = 0; cnt < digs - 1; cnt++)//�������һ��
		{
			temp = (udi_t)x[cnt + 1];

			//��һλʵ��û�У���1����������һλ++
			if (temp == 0)
			{
				temp = 1;

				short int yplusdigs = 1;
				while (tempy[cnt + yplusdigs] == 0xffffffff)
				{
					tempy[cnt + yplusdigs] = 0;
					yplusdigs++;
					if (cnt + yplusdigs >= digs)
					{
						tempy[digs - 1] = 0xffffffff;
						isoutspan = 1;
						break;
					}
				}
				tempy[cnt + yplusdigs]++;
			}

			temp <<= 32;
			carry += temp;//�µĴ󱻼���

			carry -= (udi_t)tempy[cnt];
			r[cnt] = (dig_t)(carry);

			carry >>= 32;
		}

		//cnt = digs - 1�ˣ����ϲ�λ
		if (carry >= (udi_t)tempy[digs - 1])
		{
			r[digs - 1] = (dig_t)(carry - (udi_t)tempy[digs - 1]);
			free(tempy);

			if (isoutspan)
			{
				return 1;
			}
			return 0;
		}
		else
		{
			temp = 0x100000000;
			carry += temp;

			r[digs - 1] = (dig_t)(carry - (udi_t)tempy[digs - 1]);

			free(tempy);
			return 1;
		}
	}
	else
	{
		free(tempy);
		return -1;
	}
}


/**
* 32����Ч����λ
* ����������16�ֽڵ���
* @param[in] result - �����ָ�룬һ���preͬ��
* @param[in] pre - ��Ҫ����λ��dig_t���飨������dig_t[8])
* @param[in] line - ��Ҫ��λ32��line��
*/
void bn_32lef(dig_t* result, const dig_t* pre, int line)
{
	if (line == 0)
	{
		return;
	}
	int cnt = 0;
	dig_t* tempy = (dig_t*)calloc(16, sizeof(dig_t));//��ʱ�ļ���
	if(tempy)
	{
		bn_copy(tempy, pre, 16);
		bn_set_zero(result, 16);
		for (cnt = 0; cnt <= 8; cnt++)
		{
			result[cnt + line] = tempy[cnt];
		}
		free(tempy);
	}
	else
	{
		return;
	}
}

/**
* 32����Ч����λ
* ����������16�ֽڵ���
* @param[in] result - �����ָ�룬һ���preͬ��
* @param[in] pre - ��Ҫ����λ��dig_t����
* @param[in] line - ��Ҫ��32λ��line��
*/
void bn_32rig(dig_t* result, const dig_t* pre, int line)
{
	//���ȴ����Ƶ������Ƹ��½��ı���
	dig_t* tempy = (dig_t*)calloc(16, sizeof(dig_t));
	if(tempy)
	{
		bn_set_zero(tempy, 16);
		bn_copy(tempy, pre, 16);
		bn_set_zero(result, 16);

		int cnt = 0;
		while (tempy[cnt] == 0x00000000)
		{
			cnt++;
		}

		for (; cnt < 8; cnt++)
		{
			result[cnt - line] = tempy[cnt];
		}

		free(tempy);
	}
	else
	{
		return;
	}
}

/**
 * Multiply two number x and y to r
 * r = x * y
 * Compute r = x * y
 *
 * @param[out] r             - the result.
 * @param[in] x              - the big number.
 * @param[in] y              - the big number.
 * @param[in] digs           - the number of digits.
 */
void bn_mul(dig_t* r, const dig_t* x, const dig_t* y, int digs)
{
	register udi_t temp = 0;//�洢���ֽڵĽ����׼��������һ�ֽڵ����
	register udi_t temp1 = 0;//�洢

	bn_set_zero(r, 2 * digs);

	//�洢һ�γ˷��е�digs*2�����������digs��
	dig_t** tempr = (dig_t**)calloc(digs, sizeof(dig_t*));
	if (tempr)
	{
		for (int i = 0; i < digs; i++)
		{
			tempr[i] = (dig_t*)calloc(2 * digs, sizeof(dig_t));//Ϊÿ����ָ�����digs*2�����dig_t�����ݵ��ڴ�
			if (!tempr[i])
			{
				return;
			}
			bn_set_zero(tempr[i], 2 * digs);
		}

		int line = 0;//�ڶ��������ĵڼ�������
		int cnt = 0;//��һ�������ĵڼ�������

		for (; line < digs; line++)
		{
			temp = 0;
			for (cnt = 0; cnt < digs; cnt++)
			{
				temp += ((udi_t)x[cnt] * (udi_t)y[line]);
				tempr[line][cnt] = (dig_t)temp;
				temp >>= 32;
			}
			//������������֣���digs + 1��
			tempr[line][digs] = (dig_t)temp;
		}


		for (line = 0; line < digs; line++)
		{
			if (line != 0)
			{
				bn_32lef(tempr[line], tempr[line], line);
			}
			bn_add(r, r, tempr[line], 2 * digs);
		}

		free(tempr);
	}
	else
	{
		return;
	}
}


/**
 * Compares two big numbers.
 *
 * @param[in] x				- the first big number.
 * @param[in] y				- the second big number.
 * @param[in] digs			- the number of digits.
 * @return -1 if x < y, 0 if x == y and 1 if x > y.
 */
int bn_cmp(const dig_t* x, const dig_t* y, int digs)
{
	int cnt = digs - 1;

	for (; cnt >= 0; cnt--)
	{
		if (x[cnt] > y[cnt])
		{
			return 1;
		}
		else if (x[cnt] < y[cnt])
		{
			return -1;
		}
		else
		{
			continue;
		}
	}
	return 0;
}


/**
 * Shifts a big number to the left. Computes r = x << bits.
 * ��������,���Բ�����21��
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number to shift.
 * @param[in] bits			- the number of bits to shift.
 * @param[in] digs			- the number of digits.
 * @return the carry of the overflow bits.
 */
dig_t bn_lsh_low(dig_t* r, const dig_t* x, int bits, int digs)
{
	register udi_t temp = 0;//�洢���ֽڵĽ����׼��������һ�ֽڻ����
	register udi_t temp1 = 0;//�����������������ֵ��������


	int cnt = 0;
	for (; cnt < digs; cnt++)
	{
		temp >>= 32;//�������ֽ�֮���ֵ��û		

		//��һ���ɴ˽���
		temp1 = (udi_t)x[cnt];//���ȸ�ֵ�������Ϊԭ����Ԫ��

		temp1 <<= bits;//��λ
		temp += temp1;

		r[cnt] = (dig_t)temp;//����λ������ֽڵ�ֵ����ԭ����������У�result��λ�õ�ֵ
	}

	temp >>= 32;

	return (dig_t)temp;
}


/**
 * Shifts a big number to the right. Computes r = x >> bits.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number to shift.
 * @param[in] bits			- the number of bits to shift.
 * @param[in] digs			- the number of digits.
 * @return the carry of the overflow bits.
 */
dig_t bn_rsh_low(dig_t* r, const dig_t* x, int bits, int digs)
{
	register udi_t temp = 0;//�洢���ֽڵĽ����׼��������һ�ֽڻ������ǰ32�ǵ�ǰ�ֽڣ���32��׼������һ�ֽڵ�����
	register udi_t temp1 = 0;//�����������������ֵ��������
	register udi_t temp2 = 0;//��result��ֵ�ı���

	int cnt = digs - 1;
	for (; cnt >= 0; cnt--)
	{
		//���帳ֵΪ��ǰ�ֽڵ�ֵ
		temp1 = (udi_t)x[cnt];
		temp1 <<= 32;

		temp1 >>= bits;//��������

		temp += temp1;

		temp2 = temp;
		temp2 >>= 32;
		r[cnt] = (dig_t)temp2;		//��һ�ֽ�result�н���һ���ֽ������ֵ

		temp <<= 32;

	}

	return (dig_t)temp;//�������ڻ������ĺ�32
}

/**
 * Computes r = x + y mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the first big number to subtract.
 * @param[in] y				- the second big number to subtract.
 * @param[in] m				- the modulus.
 * @param[in] digs			- the number of digits.
 */
void bn_mod_add(dig_t* r, const dig_t* x, const dig_t* y, const dig_t* m, int digs)
{
	int carry = bn_add(r, x, y, digs);
	if (carry)//�������(1)
	{
		bn_sub(r, r, m, digs);
	}
}

/**
 * Computes r = x - y mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the first big number to subtract.
 * @param[in] y				- the second big number to subtract.
 * @param[in] m				- the modulus.
 * @param[in] digs			- the number of digits.
 */
#define method1
void bn_mod_sub(dig_t* r, const dig_t* x, const dig_t* y, const dig_t* m, int digs)
{
	if (bn_cmp(x, y, digs) == 1)
	{
		bn_sub(r, x, y, digs);
	}
	else if (bn_cmp(x, y, digs) == -1)
	{
		//�����ǿ��
#ifdef method1
		bn_set_zero(r, digs );
		r[digs] = bn_add(r, m, m, digs);
		r[digs] += bn_add(r, x, r, digs);
		bn_sub(r, r, y, digs);
#endif
#ifdef method2
		//�ٰ�
		bn_sub(r, m, y, digs);
		bn_add(r, r, x, digs);
#endif
	}
	else
	{
		bn_set_zero(r, digs);
		return;
	}
}

/**
 * Halve a big number with modular reduction.
 * Computes r = x / 2 mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the field element to halve.
 * @param[in] m				- the modulus.
 * @param[in] digs			- the number of digits.
 */
void bn_mod_hlv(dig_t* r, const dig_t* x, const dig_t* m, int digs)
{
	if (*x % 2)
	{
		if (bn_cmp(x, m, digs) < 0)
		{
			bn_mod_add(r, x, m, m, digs);
		}
		else
		{
			bn_mod_sub(r, x, m, m, digs);
		}
		bn_rsh_low(r, r, 1, digs);
	}
	else
	{
		bn_rsh_low(r, x, 1, digs);
	}
}

/**
 * A modular reduction of a big number.x
 *
 * @param[out] r			- the result.
 * @param[in] x				- the field element to module.
 * @param[in] m				- the modulus.
 * @param[in] digs          - the number of the big number.
 */
 //����
#if 0
void bn_mod_rdc(dig_t* r, const dig_t* x, const dig_t* m, int xdigs, int mdigs)
{
	if (xdigs > mdigs)
	{
		dig_t* tempy = (dig_t*)calloc(xdigs, sizeof(dig_t));//��ʱ��Լ��
		bn_copy(tempy, m, mdigs);

		if (bn_cmp(tempy, x, xdigs) < 0)
		{
			while (bn_cmp(tempy, x, xdigs) < 0)
			{
				bn_lsh_low(tempy, tempy, 5, xdigs);
			}
			while (bn_cmp(tempy, x, xdigs) >= 0)
			{
				bn_rsh_low(tempy, x, 1, xdigs);
			}
			bn_lsh_low(tempy, x, 1, xdigs);
		}

		bn_sub(r, x, tempy, xdigs);
	}
	else
	{
		//�����������Χ�������·���
		if (bn_cmp(x, m, mdigs) < 0)
		{
			r = x;
		}
		else
		{
			bn_sub(r, x, m, mdigs);
		}
	}
}
#endif
//�İ�
#if 1
void bn_mod_rdc(dig_t* r, const dig_t* x, const dig_t* m, int xdigs, int mdigs)
{
	//����x��m��ֹ�����ı�
	dig_t* constx = (dig_t*)calloc(xdigs, sizeof(dig_t));
	bn_set_zero(constx, xdigs);
	bn_copy(constx, x, xdigs);

	dig_t* constm = (dig_t*)calloc(xdigs, sizeof(dig_t));
	bn_set_zero(constm, xdigs);
	bn_copy(constm, m, mdigs);

	//����Ԥ��16�ֽ���Ϊ�˺��ĵ���λ
	dig_t* tempm = (dig_t*)calloc(xdigs, sizeof(dig_t));
	bn_set_zero(tempm, xdigs);
	bn_copy(tempm, m, mdigs);

	dig_t* tempr = (dig_t*)calloc(xdigs, sizeof(dig_t));
	bn_set_zero(tempr, xdigs);
	bn_set_zero(r, mdigs);

	//���Ȼ��x��m����ʵλ
	int xbits = bn_get_bits(x, xdigs);
	int mbits = bn_get_bits(m, mdigs);

	//���������xbitsС�ڵ���mbits��x��mС
	if (xbits <= mbits)
	{
		if (bn_cmp(x, m, mdigs) < 0)
		{
			bn_copy(r, x, mdigs);
			return;
		}
		else if (bn_cmp(x, m, mdigs) == 0)
		{
			bn_set_zero(r, mdigs);
			return;
		}
	}

	//�����������ʵ����ж��
	int delbits = xbits - mbits;
	bn_32lef(tempm, tempm, delbits/32);

	if (bn_cmp(constx, tempm, xdigs) < 0)
	{
		while (bn_cmp(constx, tempm, xdigs) < 0)
		{
			bn_rsh_low(tempm, tempm, 1, xdigs);//�������ƣ�ֱ���ɱ�
			if (bn_cmp(constx, tempm, xdigs) == 0)
			{
				bn_set_zero(r, mdigs);
				return;
			}
		}
	}
	else if (bn_cmp(constx, tempm, xdigs) == 0)
	{
		bn_set_zero(r, mdigs);
		return;
	}

	bn_sub(tempr, constx, tempm, xdigs);//��һ��������

	//շת���
	short int isdouble = 0;

	while (bn_cmp(tempr, constm, xdigs) > 0)
	{
		isdouble = 0;
		while (bn_cmp(tempr, tempm, xdigs) > 0)
		{
			bn_lsh_low(tempm, tempm, 1, xdigs);
			isdouble = 1;
		}
		if(isdouble == 1)
		{
			bn_rsh_low(tempm, tempm, 1, xdigs);
		}

		if (bn_cmp(tempr, tempm, xdigs) > 0)
		{
			bn_sub(tempr, tempr, tempm, xdigs);
		}
		else if (bn_cmp(tempr, tempm, xdigs) == 0)
		{
			bn_set_zero(r, mdigs);
			return;
		}
		else// < 0
		{
			bn_rsh_low(tempm, tempm, 1, xdigs);
		}
	}

	bn_copy(r, tempr, mdigs);
}
#endif

/**
 * Compute r = x * y mod m.
 *
 * @param[out] r             - the result.
 * @param[in] x              - the big number.
 * @param[in] y              - the big number.
 * @param[in] m				- the modulus.
 * @param[in] digs           - the number of digits.
 */
void bn_mod_mul(dig_t* r, const dig_t* x, const dig_t* y, const dig_t* m, int digs)
{
	dig_t* tempmul = (dig_t*)calloc(2 * digs, sizeof(dig_t));//��ʱ�Ĵ�˻�
	dig_t* tempmod = (dig_t*)calloc(2 * digs, sizeof(dig_t));//��ʱ��ģ��
	dig_t* constx = (dig_t*)calloc(digs, sizeof(dig_t));
	dig_t* tempy = (dig_t*)calloc(digs, sizeof(dig_t));

	bn_set_zero(constx, digs);
	bn_set_zero(tempy, digs);

	bn_mod_rdc(constx, x, m, digs, digs);
	bn_mod_rdc(tempy, y, m, digs, digs);

	bn_set_zero(tempmul, 2 * digs);
	bn_mul(tempmul, constx, tempy, digs);
	bn_set_zero(tempmod, 2 * digs);
	bn_copy(tempmod, m, digs);

	//�������Ҳû�и��죬����ɾ��
#if 0
	for (int cmb = 16; cmb >= 1; cmb /= 2)
	{
		while (bn_cmp(tempmod, tempmul, 2 * digs) <= 0)
		{
			bn_lsh_low(tempmod, tempmod, cmb, 2 * digs);
		}

		bn_rsh_low(tempmod, tempmod, cmb, 2 * digs);

		bn_mod_sub(tempmul, tempmul, tempmod, m, 2 * digs);
		bn_set_zero(tempmod, 2 * digs);
		bn_copy(tempmod, m, digs);
	}
#endif

	bn_mod_rdc(r, tempmul, m, 2 * digs, digs);

	free(tempmul);
	free(tempmod);
	free(constx);
	free(tempy);
}


//hextobin��С��Ԫ��������������һ��Ԫ��
#include<string.h>
void hextobin(char* result, dig_t num)
{
	register int remainder;

	const static char buf[16][5] = { "0000","0001","0010","0011",\
					"0100","0101","0110","0111",\
					"1000","1001","1010","1011",\
					"1100","1101","1110","1111" };

	if (0 == num)
	{
		return;
	}

	remainder = num % 16;
	hextobin(result, num >> 4);
	strcat(result, buf[remainder]);
}

/**
* ��16���ƵĴ�����[8]ת��Ϊ���ֳ�2���Ƶ��ַ���[257]
* @param[in] result - ������
* @param[in] pre - ׼��������Ĵ�������
*/
short int isfirst = 1;//�Ƿ��ǵ�һ��С��Ԫת������
void bighextobin(char* result, dig_t* pre)
{
	//result�ڴ��뺯��ǰ�Ѿ�����
	char tempresult[40] = { 0 };

	register int cnt = 7;
	for (; cnt >= 0; cnt--)
	{
		if (pre[cnt] == 0x00000000)
		{
			continue;
		}
		else
		{
			strcpy(tempresult, "\0");
			hextobin(tempresult, pre[cnt]);
			if (isfirst != 1)
			{
				register int leftlen = strlen(tempresult);
				for (register int cntt = 0; cntt < 32 - leftlen; cntt++)
				{
					strcat(result, "0");
				}
			}
			strcat(result, tempresult);
			isfirst = 0;
		}		
	}
	strcat(result, "\0");
}

/**
 * Computes modular exponentiation: r = x ^ e mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number.
 * @param[in] e				- the exponent.
 * @param[in] m				- the modulus.
 * @param[in] digs          - the number of digits.
 */
void bn_mod_exp(dig_t* r, const dig_t* x, const dig_t* e, const dig_t* m, int digs)
{
	dig_t* constx = (dig_t*)calloc(digs, sizeof(dig_t));//���x������
	bn_set_zero(constx, digs);
	bn_copy(constx, x, digs);

	char arraye[280] = { 0 };//һ�����Դ洢257���ַ����ַ��������洢e�Ķ����ƽ��
	bighextobin(arraye, e);

	//�����Ʒ���
	int cnt = 0;
	while (arraye[cnt] != '1')
	{
		cnt++;
	}

	bn_copy(r, constx, digs);//step1
	cnt++;

	for (; arraye[cnt] != '\0'; cnt++)//step2
	{
		//2a
		bn_mod_mul(r, r, r, m, digs);

		//2b
		if (arraye[cnt] == '1')
		{
			bn_mod_mul(r, r, constx, m, digs);
		}
	}

	free(constx);
}