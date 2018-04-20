/* ===-- multi3.c - Implement __multi3 -------------------------------------===
 *
 *                     The LLVM Compiler Infrastructure
 *
 * This file is dual licensed under the MIT and the University of Illinois Open
 * Source Licenses. See LICENSE.TXT for details.
 *
 * ===----------------------------------------------------------------------===

 * This file implements __multi3 for the compiler_rt library.
 *
 * ===----------------------------------------------------------------------===
 */

#include "int_lib.h"

/* Returns: a * b */

static void __mulddi3( __int128* ret, du_int a, du_int b)
{
    twords r;
    const int bits_in_dword_2 = (int)(sizeof(di_int) * CHAR_BIT) / 2;
    const du_int lower_mask = (du_int)~0 >> bits_in_dword_2;
    r.s.low = (a & lower_mask) * (b & lower_mask);
    du_int t = r.s.low >> bits_in_dword_2;
    r.s.low &= lower_mask;
    t += (a >> bits_in_dword_2) * (b & lower_mask);
    r.s.low += (t & lower_mask) << bits_in_dword_2;
    r.s.high = t >> bits_in_dword_2;
    t = r.s.low >> bits_in_dword_2;
    r.s.low &= lower_mask;
    t += (b >> bits_in_dword_2) * (a & lower_mask);
    r.s.low += (t & lower_mask) << bits_in_dword_2;
    r.s.high += t >> bits_in_dword_2;
    r.s.high += (a >> bits_in_dword_2) * (b >> bits_in_dword_2);
    *ret = r.all;
}

/* Returns: a * b */

void __multi3(__int128* ret, uint64_t al, uint64_t ah, uint64_t bl, uint64_t bh)
{
    twords a;
    a.s.high = ah;
    a.s.low  = al;
    twords b;
    b.s.high = bh;
    b.s.low  = bl;
    twords x;
    x.all = a.all;
    twords y;
    y.all = b.all;
    twords r;
    __mulddi3(&r.all, x.s.low, y.s.low);
    r.s.high += x.s.high * y.s.low + x.s.low * y.s.high;
    *ret = r.all;
}
