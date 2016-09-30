/*
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

void
_acb_dirichlet_dft_cyc_init_z_fac(acb_dirichlet_dft_cyc_t t, n_factor_t fac, slong dv, acb_ptr z, slong dz, slong len, slong prec)
{
    slong i, j, num;
    t->n = len;
    num = 0;
    for (i = 0; i < fac.num; i++)
        num += fac.exp[i];
    t->num = num;
    t->cyc = flint_malloc(num * sizeof(acb_dirichlet_dft_step_struct));

    if (z == NULL)
    {
        z = _acb_vec_init(t->n);
        acb_dirichlet_vec_nth_roots(z, t->n, prec);
        dz = 1;
        t->zclear = 1;
    }
    else
    {
        if (DFT_VERB)
            flint_printf("dft_cyc: roots of order %wu already computed\n", t->n);
        t->zclear = 0;
    }

    t->z = z;

    num = 0;
    for (i = 0; i < fac.num; i++)
    {
        for (j = 0; j < fac.exp[i]; j++)
        {
            slong m, M;
            m = fac.p[i];
            M = (len /= m);
            t->cyc[num].m = m;
            t->cyc[num].M = M;
            t->cyc[num].dv = dv;
            t->cyc[num].z = z;
            t->cyc[num].dz = dz;
            /* TODO: ugly, reorder should solve this */
            if (num == t->num - 1)
                _acb_dirichlet_dft_precomp_init(t->cyc[num].pre, dv, z, dz, m, prec);
            else
                _acb_dirichlet_dft_precomp_init(t->cyc[num].pre, M, z, dz * M, m, prec);
            dv *= m;
            dz *= m;
            num++;
        }
    }
}

void
_acb_dirichlet_dft_cyc_init(acb_dirichlet_dft_cyc_t t, slong dv, slong len, slong prec)
{
    n_factor_t fac;
    n_factor_init(&fac);
    n_factor(&fac, len, 0);
    _acb_dirichlet_dft_cyc_init_z_fac(t, fac, dv, NULL, 0, len, prec);
}

void
acb_dirichlet_dft_cyc_clear(acb_dirichlet_dft_cyc_t t)
{
    slong i;
    for (i = 0; i < t->num; i++)
        acb_dirichlet_dft_precomp_clear(t->cyc[i].pre);
    if (t->zclear)
        _acb_vec_clear(t->z, t->n);
    flint_free(t->cyc);
}

void
acb_dirichlet_dft_cyc(acb_ptr w, acb_srcptr v, slong len, slong prec)
{
    acb_dirichlet_dft_cyc_t cyc;
    acb_dirichlet_dft_cyc_init(cyc, len, prec);
    acb_dirichlet_dft_cyc_precomp(w, v, cyc, prec);
    acb_dirichlet_dft_cyc_clear(cyc);
}
