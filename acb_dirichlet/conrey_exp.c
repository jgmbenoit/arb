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

    Copyright (C) 2016 Pascal Molin

******************************************************************************/

#include "acb_dirichlet.h"

ulong
acb_dirichlet_conrey_exp(acb_dirichlet_conrey_t x, const acb_dirichlet_group_t G)
{
    ulong k, n = 1;
    for (k = G->neven; k < G->num; k++)
        n = nmod_mul(n, nmod_pow_ui(G->generators[k], x->log[k], G->mod), G->mod);
    x->n = n;
    return n;
}