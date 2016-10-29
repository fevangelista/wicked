#!/usr/bin/env python

import pywicked as pw

opT1 = pw.make_diag_operator("T1",["v"],["c"])
opT2 = pw.make_diag_operator("T2",["v", "v"],["c", "c"])

opR1 = pw.make_diag_operator("R1",["c"],["v"])
opR2 = pw.make_diag_operator("R2",["c", "c"],["v", "v"])

opH1oo = pw.make_diag_operator("f", ["c"], ["c"]);
opH1ov = pw.make_diag_operator("f", ["c"], ["v"]);
opH1vo = pw.make_diag_operator("f", ["v"], ["c"]);
opH1vv = pw.make_diag_operator("f", ["v"], ["v"]);

opH2oooo = pw.make_diag_operator("v", ["c", "c"], ["c", "c"]);
opH2ovoo = pw.make_diag_operator("v", ["c", "v"], ["c", "c"]);
opH2ooov = pw.make_diag_operator("v", ["c", "c"], ["c", "v"]);
opH2ovov = pw.make_diag_operator("v", ["c", "v"], ["c", "v"]);
opH2vvoo = pw.make_diag_operator("v", ["v", "v"], ["c", "c"]);
opH2oovv = pw.make_diag_operator("v", ["c", "c"], ["v", "v"]);
opH2vvov = pw.make_diag_operator("v", ["v", "v"], ["c", "v"]);
opH2ovvv = pw.make_diag_operator("v", ["c", "v"], ["v", "v"]);
opH2vvvv = pw.make_diag_operator("v", ["v", "v"], ["v", "v"]);

T1 = pw.WDiagOperatorSum([ [opT1] ]); # + opT1
T2 = pw.WDiagOperatorSum([ [opT2] ]); # + opT1
F = pw.WDiagOperatorSum([ [opH1oo],[opH1ov],[opH1vo],[opH1vv] ]); # + opH1oo + opH1ov + opH1vo + opH1vv
V = pw.WDiagOperatorSum([ [opH2oooo],[opH2ovoo],[opH2ooov],[opH2ovov],[opH2vvoo],[opH2oovv],[opH2vvov],[opH2ovvv],[opH2vvvv]]); # + opH1oo + opH1ov + opH1vo + opH1vv

T1_squared = pw.WDiagOperatorSum([ [opT1,opT1] ]); # + opT1 * opT1

wdt = pw.WDiagTheorem()

FT2_c = pw.commutator(V, T1);
FT2T2_c = pw.commutator(FT2_c, T1);

#print T1F_c.str()

sum = wdt.contract_sum(pw.rational(1,2),FT2T2_c, 0, 4);

print sum.str()

eqs = sum.to_manybody_equation("R")

for eq in eqs:
    print eq.ambit()

#WDiagOperatorSum T2({{opT2}});
#WDiagOperatorSum R1({{opR1}});
#WDiagOperatorSum R2({{opR2}});
#WDiagOperatorSum F({{opH1oo}, {opH1ov}, {opH1vo}, {opH1vv}});

#WDiagOperatorSum V;
#for (const auto &op : {opH2oooo, opH2ovoo, opH2ooov, opH2ovov, opH2vvoo,
#                       opH2oovv, opH2vvov, opH2ovvv, opH2vvvv}) {
#  V.add({op});
#}



#cout << res << endl;
#for (const auto &eq : res.to_manybody_equation("R")) {
#  cout << "  " << eq.ambit() << endl;
#}

#
#sum = wdt.contract(pw.rational(1),[opR1,opT1],0,0)
#eqs = sum.to_manybody_equation("R")
#for eq in eqs:
#    print eq.ambit()
