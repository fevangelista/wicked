#!/usr/bin/env python

import pywicked as pw

pw.osi.reset()
pw.osi.add_space("v",pw.DMStructure.Unoccupied,["a","b"])
pw.osi.add_space("o",pw.DMStructure.DoublyOccupied,["i","j"])

T1 = pw.make_operator("T1",["o->v"])
T2 = pw.make_operator("T2",["oo->vv"])

F = pw.make_operator("F", ["o->o","o->v","v->o","v->v"]);
V = pw.make_operator("V", ["oo->oo", "oo->ov", "oo->vv",
                           "ov->oo", "ov->ov", "ov->vv",
                           "vv->oo", "vv->ov", "vv->vv"]);

wdt = pw.WDiagTheorem()
FT2_c = pw.commutator(V, T1);
FT2T2_c = pw.commutator(FT2_c, T1);

sum = wdt.contract_sum(pw.rational(1,2),FT2T2_c, 0, 4);

eqs = sum.to_manybody_equation("R")

for eq in eqs:
    print eq.ambit()
