/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

void
_fmprb_poly_evaluate_fmpcb_rectangular(fmpcb_t y, const fmprb_struct * poly,
    long len, const fmpcb_t x, long prec)
{
    long i, j, m, r;
    fmpcb_struct * xs;
    fmpcb_t s, t, c;

    if (len < 3)
    {
        if (len == 0)
        {
            fmpcb_zero(y);
        }
        else if (len == 1)
        {
            fmpcb_set_round_fmprb(y, poly + 0, prec);
        }
        else if (len == 2)
        {
            fmpcb_mul_fmprb(y, x, poly + 1, prec);
            fmpcb_add_fmprb(y, y, poly + 0, prec);
        }
        return;
    }

    m = n_sqrt(len) + 1;
    r = (len + m - 1) / m;

    xs = _fmpcb_vec_init(m + 1);
    fmpcb_init(s);
    fmpcb_init(t);
    fmpcb_init(c);

    for (i = 0; i <= m; i++)
    {
        if (i == 0)
            fmpcb_one(xs + i);
        else if (i == 1)
            fmpcb_set(xs + i, x);
        else if (i % 2 == 0)
            fmpcb_mul(xs + i, xs + i / 2, xs + i / 2, prec);
        else
            fmpcb_mul(xs + i, xs + i - 1, x, prec);
    }

    fmpcb_set_fmprb(y, poly + (r - 1) * m);
    for (j = 1; (r - 1) * m + j < len; j++)
        fmpcb_addmul_fmprb(y, xs + j, poly + (r - 1) * m + j, prec);

    for (i = r - 2; i >= 0; i--)
    {
        fmpcb_set_fmprb(s, poly + i * m);
        for (j = 1; j < m; j++)
            fmpcb_addmul_fmprb(s, xs + j, poly + i * m + j, prec);

        fmpcb_mul(y, y, xs + m, prec);
        fmpcb_add(y, y, s, prec);
    }

    _fmpcb_vec_clear(xs, m + 1);
    fmpcb_clear(s);
    fmpcb_clear(t);
    fmpcb_clear(c);
}

void
fmprb_poly_evaluate_fmpcb_rectangular(fmpcb_t res, const fmprb_poly_t f, const fmpcb_t a, long prec)
{
    _fmprb_poly_evaluate_fmpcb_rectangular(res, f->coeffs, f->length, a, prec);
}

