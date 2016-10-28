#!/usr/bin/env python

import pywicked

opT1 = pywicked.make_diag_operator("T1",["v"],["c"])
opR1 = pywicked.make_diag_operator("R1",["c"],["v"])

wdt = pywicked.WDiagTheorem()
sum = wdt.contract(pywicked.rational(1),[opR1,opT1],0,0)
eqs = sum.to_manybody_equation("R")
for eq in eqs:
    print eq.ambit()
