#!/usr/bin/env python

import pywicked as pw

pw.osi.reset()
pw.osi.add_space("v",pw.rdmtype.unoccupied,["a","b"])
pw.osi.add_space("o",pw.rdmtype.occupied,["i","j"])

T1 = pw.make_operator("T1",["o->v"])
T2 = pw.make_operator("T2",["oo->vv"])
T3 = pw.make_operator("T3",["ooo->vvv"])
T4 = pw.make_operator("T4",["oooo->vvvv"])

T = pw.make_operator("T",["o->v","oo->vv","ooo->vvv","oooo->vvvv"])
F = pw.make_operator("F", ["o->o","o->v","v->o","v->v"]);
V = pw.make_operator("V", ["oo->oo", "oo->ov", "oo->vv",
                           "ov->oo", "ov->ov", "ov->vv",
                           "vv->oo", "vv->ov", "vv->vv"]);
H = pw.make_operator("H", ["o->o"  , "o->v"  , "v->o"  , "v->v",
                           "oo->oo", "oo->ov", "oo->vv",
                           "ov->oo", "ov->ov", "ov->vv",
                           "vv->oo", "vv->ov", "vv->vv"]);
wdt = pw.WDiagTheorem()
#FT2_c = pw.commutator(V, T1);
#FT2T2_c = pw.commutator(FT2_c, T1);

#VT2_c = pw.commutator(V, T2);
#VT2T2_c = pw.commutator(VT2_c, T2);

Hbar = pw.bch_series(H,T,4)

sum = wdt.contract_sum(pw.rational(1,1),Hbar, 0, 8);

eqs = sum.to_manybody_equation("R")

for eq in eqs:
    print eq.ambit()

#A = pw.make_operator("A",["o->o"])
#B = pw.make_operator("B",["o->o"])
#print pw.bch_series(A,B,4).str()
