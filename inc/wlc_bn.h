#ifndef _WLC_BN_H_
#define _WLC_BN_H_


#include "wlc_types.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define ENABLE_BN_MEM
#define ENABLE_BN_ADD
#define ENABLE_BN_CMP
#define ENABLE_BN_SHIFT
#define ENABLE_BN_MOD
#define ENABLE_BN_EXTRA

/*============================================================================*/
/* Constant definitions          
*///常量定义
/*============================================================================*/

#define MAX_BN_BITS     2112

#define MAX_BN_BYTES     (MAX_BN_BITS/8)

#define MAX_BN_DIGS     (MAX_BN_BITS / WBITS)

/*============================================================================*/
/* Macro definitions    
*///宏定义
/*============================================================================*/

/**
 * Tests if a big number is even.
 * 测试一个大数是否为偶数。
 *
 * @param[in] a				- the big number to test.
 * @return 1 if the argument is even, 0 otherwise.
 */
#define BN_IS_EVEN(a, w) (((a)[0] & 1) ^ 1)

 /*============================================================================*/
 /* Function definitions 
 *///功能定义
 /*============================================================================*/

#ifdef __cplusplus
 extern "C" {	/* start of __cplusplus */
#endif



#ifdef  ENABLE_BN_MEM
/**
* 挨个从地址靠后的位置向前,从而由高位数向小位数，打印大整数表示的数值
* @param[in] x - 指向等待被打印的大整数的指针
* @param[in] digs - 存储大整数数组的真实元素个数
*/
void bn_print(const dig_t* x, int digs);

/**
 * Set zero to a big number.
 * 将一个大数清零
 *
 * @param[out] x			- the big number to assign.
 * @param[in] digs			- the number of digits.
 */
void bn_set_zero(dig_t* x, int digs);

/**
 * Randomize a big number.
 * 随机化一个大数
 *
 * @param[out] x			- the big number to randomize.
 * @param[in] digs			- the number of digits.
 */
void bn_rand(dig_t* x, int digs);

/**
 * Copy the second big number to the first big number.
 * 将第二个大数复制到第一个大数
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number to copy.
 * @param[in] digs			- the number of digits.
 */
void bn_copy(dig_t* r, const dig_t* x, int digs);

/**
 * get the number of bits of a big number.
 * 求一个大数的位数
 * 
 * @param[in] x             - the big number to get the number of bits.
 * @param[in] digs          - the number of digits.
 * @return the number of bits of the big number x.
 */
int bn_get_bits(const dig_t* x, int digs);

#endif

#ifdef  ENABLE_BN_ADD

/**
 * Add two big numbers of the same size. 
 * 对两个大数同一位进行加法
 * 
 * Computes r = x + y.
 * 计算 r = x + y
 *
 * @param[out] r			- the result.
 * @param[in] x				- the first big number to add.
 * @param[in] y				- the second big number to add.
 * @param[in] digs			- the number of digits.
 * @return the carry of the last digit addition.
 */
dig_t bn_add(dig_t* r, const dig_t* x, const dig_t* y, int digs);

/**
 * Subtract a big number from another.
 * 用另一个数减去一个数
 * Compute r = x - y.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number.
 * @param[in] y				- the big number to subtract.
 * @param[in] digs			- the number of digits.
 * @return the carry of the last digit subtraction.
 */
dig_t bn_sub(dig_t* r, const dig_t* x, const dig_t* y, int digs);

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
void bn_mul(dig_t* r, const dig_t* x, const dig_t* y, int digs);

#endif /* ENABLE_BN_ADD */

#ifdef  ENABLE_BN_CMP 

/**
 * Compares two big numbers.
 *
 * @param[in] x				- the first big number.
 * @param[in] y				- the second big number.
 * @param[in] digs			- the number of digits.
 * @return -1 if x < y, 0 if x == y and 1 if x > y.
 */
int bn_cmp(const dig_t* x, const dig_t* y, int digs);

#endif /* ENABLE_BN_CMP */

#ifdef  ENABLE_BN_SHIFT

/**
 * Shifts a big number to the left. Computes r = x << bits.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number to shift.
 * @param[in] bits			- the number of bits to shift.
 * @param[in] digs			- the number of digits.
 * @return the carry of the overflow bits.
 */
dig_t bn_lsh_low(dig_t* r, const dig_t* x, int bits, int digs);

/**
 * Shifts a big number to the right. Computes r = x >> bits.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number to shift.
 * @param[in] bits			- the number of bits to shift.
 * @param[in] digs			- the number of digits.
 * @return the carry of the overflow bits.
 */
dig_t bn_rsh_low(dig_t* r, const dig_t* x, int bits, int digs);

#endif

#ifdef  ENABLE_BN_MOD

/**
 * Computes r = x + y mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the first big number to subtract.
 * @param[in] y				- the second big number to subtract.
 * @param[in] m				- the modulus.
 * @param[in] digs			- the number of digits.
 */
void bn_mod_add(dig_t* r, const dig_t* x, const dig_t* y, const dig_t* m, int digs);

/**
 * Computes r = x - y mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the first big number to subtract.
 * @param[in] y				- the second big number to subtract.
 * @param[in] m				- the modulus.
 * @param[in] digs			- the number of digits.
 */
void bn_mod_sub(dig_t* r, const dig_t* x, const dig_t* y, const dig_t* m, int digs);

/**
 * Halve a big number with modular reduction.
 * Computes r = x / 2 mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the field element to halve.
 * @param[in] m				- the modulus.
 * @param[in] digs			- the number of digits.
 */
void bn_mod_hlv(dig_t* r, const dig_t* x, const dig_t* m, int digs);


/**
 * A modular reduction of a big number.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the field element to module.
 * @param[in] m				- the modulus.
 * @param[in] digs          - the number of the big number.
 */
void bn_mod_rdc(dig_t* r, const dig_t* x, const dig_t* m, int xdigs, int mdigs);

#endif
#ifdef ENABLE_BN_EXTRA
/**
 * Compute r = x * y mod m.
 *
 * @param[out] r             - the result.
 * @param[in] x              - the big number.
 * @param[in] y              - the big number.
 * @param[in] m				- the modulus.
 * @param[in] digs           - the number of digits.
 */
void bn_mod_mul(dig_t* r, const dig_t* x, const dig_t* y, const dig_t* m, int digs);

/**
 * Computes modular exponentiation: r = x ^ e mod m.
 *
 * @param[out] r			- the result.
 * @param[in] x				- the big number.
 * @param[in] e				- the exponent.
 * @param[in] m				- the modulus.
 * @param[in] digs          - the number of digits.
 */
void bn_mod_exp(dig_t* r, const dig_t* x, const dig_t* e, const dig_t* m, int digs);

#endif


#ifdef __cplusplus
 }			/* end of __cplusplus */
#endif

#endif  /* !_WLC_BN_H_ */