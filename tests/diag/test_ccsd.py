import wicked as w

w.reset_space()
w.add_space("o", "occupied", ["i", "j", "k", "l", "m", "n"])
w.add_space("v", "unoccupied", ["a", "b", "c", "d", "e", "f"])

T1 = w.diag_operator("t", ["o->v"])
T2 = w.diag_operator("t", ["oo->vv"])
Foo = w.diag_operator("f", ["o->o"])
Fov = w.diag_operator("f", ["v->o"])
Fvo = w.diag_operator("f", ["o->v"])
Fvv = w.diag_operator("f", ["v->v"])
Voovv = w.diag_operator("v", ["vv->oo"])
Vovov = w.diag_operator("v", ["ov->ov"])
Vovvv = w.diag_operator("v", ["vv->ov"])
Vooov = w.diag_operator("v", ["ov->oo"])

wt = w.WickTheorem()


def test_print(val, val_test):
    print(f"Result: {val}")
    print(f"Test:   {val_test}")


def test_energy1():
    """CCSD Energy <F T1> (1)"""
    val = wt.contract(w.rational(1), Fov @ T1, 0, 0)
    val_test = w.string_to_expr("f^{v_0}_{o_0} t^{o_0}_{v_0}")
    test_print(val, val_test)
    assert val == val_test


def test_energy2():
    """CCSD Energy <V T2> (2)"""
    val = wt.contract(w.rational(1), Voovv @ T2, 0, 0)
    val_test = w.string_to_expr("1/4 t^{o_0,o_1}_{v_0,v_1} v^{v_0,v_1}_{o_0,o_1}")
    test_print(val, val_test)
    assert val == val_test


def test_energy3():
    """CCSD Energy 1/2 <V T1 T1> (3)"""
    val = wt.contract(w.rational(1, 2), Voovv @ T1 @ T1, 0, 0)
    val_test = w.string_to_expr("1/2 t^{o0}_{v0} t^{o1}_{v1} v^{v0,v1}_{o0,o1}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_1():
    """CCSD T1 Residual Fov (1)"""
    sum = wt.contract(w.rational(1), Fvo, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("f^{o0}_{v0}").canonicalize()
    test_print(val, val_test)
    assert val == val_test


def test_r1_2():
    """CCSD T1 Residual [Fvv,T1] (2)"""
    sum = wt.contract(w.rational(1), Fvv @ T1, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("f^{v1}_{v0} t^{o0}_{v1}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_3():
    """CCSD T1 Residual [Foo,T1] (3)"""
    sum = wt.contract(w.rational(1), Foo @ T1, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1 f^{o0}_{o1} t^{o1}_{v0}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_4():
    """CCSD T1 Residual [Vovov,T1] (4)"""
    sum = wt.contract(w.rational(1), Vovov @ T1, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1 t^{o1}_{v1} v^{o0,v1}_{o1,v0}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_5():
    """CCSD T1 Residual [Fvo,T2] (5)"""
    sum = wt.contract(w.rational(1), Fov @ T2, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("1 f^{v1}_{o1} t^{o0,o1}_{v0,v1}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_6():
    """CCSD T1 Residual [Vovvv,T2] (6)"""
    sum = wt.contract(w.rational(1), Vovvv @ T2, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1/2 t^{o0,o1}_{v1,v2} v^{v1,v2}_{o1,v0}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_7():
    """CCSD T1 Residual [Vooov,T2] (7)"""
    sum = wt.contract(w.rational(1), Vooov @ T2, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1/2 t^{o1,o2}_{v0,v1} v^{o0,v1}_{o1,o2}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_8():
    """CCSD T1 Residual 1/2 [[Fov,T1],T1] (8)"""
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Fov, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1 f^{v1}_{o1} t^{o1}_{v0} t^{o0}_{v1}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_9():
    """CCSD T1 Residual 1/2 [[Vooov,T1],T1] (9)"""
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vooov, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1 t^{o1}_{v0} t^{o2}_{v1} v^{o0,v1}_{o1,o2}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_10():
    """CCSD T1 Residual 1/2 [[Vovvv,T1],T1] (10)"""
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vovvv, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr("-1 t^{o0}_{v1} t^{o1}_{v2} v^{v1,v2}_{o1,v0}")
    test_print(val, val_test)
    assert val == val_test


def test_r1_11():
    """CCSD T1 Residual 1/6 [[[Voovv,T1],T1],T1] (11)"""
    sum = wt.contract(
        w.rational(1, 6),
        w.commutator(w.commutator(w.commutator(Voovv, T1), T1), T1),
        2,
        2,
    )
    val = sum.to_manybody_equation("r")[2]
    val_test = w.string_to_expr(
        "-1 t^{o1}_{v0} t^{o0}_{v1} t^{o2}_{v2} v^{v1,v2}_{o1,o2}"
    )
    test_print(val, val_test)
    assert val == val_test


def test_r1_12_14():
    """CCSD T1 Residual [[Voovv,T1],T2] (12-14)"""
    sum = wt.contract(
        w.rational(1),
        w.commutator(w.commutator(Voovv, T1), T2),
        2,
        2,
    )
    val = sum.to_manybody_equation("r")[2]
    val_test = (
        w.string_to_expr("1 t^{o1}_{v1} t^{o0,o2}_{v0,v2} v^{v1,v2}_{o1,o2}")
        + w.string_to_expr("-1/2 t^{o0}_{v1} t^{o1,o2}_{v0,v2} v^{v1,v2}_{o1,o2}")
        + w.string_to_expr("-1/2 t^{o1}_{v0} t^{o0,o2}_{v1,v2} v^{v1,v2}_{o1,o2}")
    )
    test_print(val, val_test)
    assert val == val_test


if __name__ == "__main__":
    test_energy1()
    test_energy2()
    test_energy3()
    test_r1_1()
    test_r1_2()
    test_r1_3()
    test_r1_4()
    test_r1_5()
    test_r1_6()
    test_r1_7()
    test_r1_8()
    test_r1_9()
    test_r1_10()
    test_r1_11()
    test_r1_12_14()

# test_return_t test_r2_1() {
#   auto Vvvoo = make_operator("v", {"oo->vv"})

#   // Vvvoo
#   auto sum = wdt.contract_sum(1, Vvvoo, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("1 v^{o0,o1}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual Vvvoo (1)"})
# }

# test_return_t test_r2_2() {
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Fvv = make_operator("f", {"v->v"})

#   // [Fvv,T2]
#   auto sum = wdt.contract_sum(1, Fvv @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("-2 f^{v2}_{v0} t^{o0,o1}_{v1,v2}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Fvv,T2] (2)"})
# }

# test_return_t test_r2_3() {
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Foo = make_operator("f", {"o->o"})

#   // [Foo,T2]
#   auto sum = wdt.contract_sum(1, Foo @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("2 f^{o0}_{o2} t^{o1,o2}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Foo,T2] (3)"})
# }

# test_return_t test_r2_4() {
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Voooo = make_operator("v", {"oo->oo"})

#   // [Voooo,T2]
#   auto sum = wdt.contract_sum(1, Voooo @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("1/2 t^{o2,o3}_{v0,v1} v^{o0,o1}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Voooo,T2] (4)"})
# }

# test_return_t test_r2_5() {
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Vvvvv = make_operator("v", {"vv->vv"})

#   // [Vvvvv,T2]
#   auto sum = wdt.contract_sum(1, Vvvvv @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("1/2 t^{o0,o1}_{v2,v3} v^{v2,v3}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vvvvv,T2] (5)"})
# }

# test_return_t test_r2_6() {
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Vovov = make_operator("v", {"ov->ov"})

#   // [Vovov,T2]
#   auto sum = wdt.contract_sum(1, Vovov @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("-4 t^{o0,o2}_{v0,v2} v^{o1,v2}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vovov,T2] (6)"})
# }

# test_return_t test_r2_7() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Vvvov = make_operator("v", {"ov->vv"})

#   // [Vvvov,T2]
#   auto sum = wdt.contract_sum(1, Vvvov @ T1, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("-2 t^{o0}_{v2} v^{o1,v2}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vvvov,T2] (7)"})
# }

# test_return_t test_r2_8() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Vovoo = make_operator("v", {"oo->ov"})

#   // [Vovoo,T2]
#   auto sum = wdt.contract_sum(1, Vovoo @ T1, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("-2 t^{o2}_{v0} v^{o0,o1}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vovoo,T2] (8)"})
# }

# test_return_t test_r2_9_12() {
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Voovv = make_operator("v", {"vv->oo"})

#   // 1/2 [[Voovv,T2],T2]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Voovv, T2), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test =
#       string_to_sum("2 t^{o0,o2}_{v0,v2} t^{o1,o3}_{v1,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "-1 t^{o0,o1}_{v0,v2} t^{o2,o3}_{v1,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "-1 t^{o0,o2}_{v0,v1} t^{o1,o3}_{v2,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "1/4 t^{o2,o3}_{v0,v1} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Voovv,T2],T2] (9-12)"})
# }

# test_return_t test_r2_13() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Voooo = make_operator("v", {"oo->oo"})

#   // 1/2 [[Voooo,T1],T1]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Voooo, T1), T1), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("1 t^{o2}_{v0} t^{o3}_{v1} v^{o0,o1}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Voooo,T1],T1] (13)"})
# }

# test_return_t test_r2_14() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Vvvvv = make_operator("v", {"vv->vv"})

#   // 1/2 [[Vvvvv,T1],T1]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Vvvvv, T1), T1), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Vvvvv,T1],T1] (14)"})
# }

# test_return_t test_r2_15() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Vovov = make_operator("v", {"ov->ov"})

#   // 1/2 [[Vovov,T1],T1]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Vovov, T1), T1), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("4 t^{o2}_{v0} t^{o0}_{v2} v^{o1,v2}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Vovov,T1],T1] (15)"})
# }

# test_return_t test_r2_16_17() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto T2 = make_operator("t", {"oo->vv"})

#   auto Fov = make_operator("f", {"v->o"})

#   // [[Fov,T1],T2]
#   auto sum = wdt.contract_sum(1, commutator(commutator(Fov, T1), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum("2 f^{v2}_{o2} t^{o0}_{v2} t^{o1,o2}_{v0,v1}") +
#                   string_to_sum("2 f^{v2}_{o2} t^{o2}_{v0} t^{o0,o1}_{v1,v2}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[Fov,T1],T2] (16-17)"})
# }

# test_return_t test_r2_18_21_22() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Vooov = make_operator("v", {"ov->oo"})

#   // [[Vooov,T1],T2]
#   auto sum = wdt.contract_sum(1, commutator(commutator(Vooov, T1), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test =
#       string_to_sum("2 t^{o2}_{v2} t^{o0,o3}_{v0,v1} v^{o1,v2}_{o2,o3}") +
#       string_to_sum("-1 t^{o0}_{v2} t^{o2,o3}_{v0,v1} v^{o1,v2}_{o2,o3}") +
#       string_to_sum("4 t^{o2}_{v0} t^{o0,o3}_{v1,v2} v^{o1,v2}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[Vooov,T1],T2] (18,21,22)"})
# }

# test_return_t test_r2_19_20_23() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Vovvv = make_operator("v", {"vv->ov"})

#   // [[Vovvv,T1],T2]
#   auto sum = wdt.contract_sum(1, commutator(commutator(Vovvv, T1), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test =
#       string_to_sum("2 t^{o2}_{v2} t^{o0,o1}_{v0,v3} v^{v2,v3}_{o2,v1}") +
#       string_to_sum("4 t^{o0}_{v2} t^{o1,o2}_{v0,v3} v^{v2,v3}_{o2,v1}") +
#       string_to_sum("-1 t^{o2}_{v0} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[Vovvv,T1],T2] (19,20,23)"})
# }

# test_return_t test_r2_24() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Vovvv = make_operator("v", {"vv->ov"})

#   // 1/6 [[[Vovvv,T1],T1],T1]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 6), commutator(commutator(commutator(Vovvv, T1), T1), T1), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test =
#       string_to_sum("-2 t^{o2}_{v0} t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/6 [[[Vovvv,T1],T1],T1] (24)"})
# }

# test_return_t test_r2_25() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Vooov = make_operator("v", {"ov->oo"})

#   // 1/6 [[[Vooov,T1],T1],T1]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 6), commutator(commutator(commutator(Vooov, T1), T1), T1), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test =
#       string_to_sum("-2 t^{o2}_{v0} t^{o3}_{v1} t^{o0}_{v2} v^{o1,v2}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/6 [[[Vooov,T1],T1],T1] (25)"})
# }

# test_return_t test_r2_26_30() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto T2 = make_operator("t", {"oo->vv"})
#   auto Voovv = make_operator("v", {"vv->oo"})

#   // [[[Voovv,T1],T1],T2]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 2), commutator(commutator(commutator(Voovv, T1), T1), T2), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test =
#       string_to_sum(
#           "-2 t^{o0}_{v2} t^{o2}_{v3} t^{o1,o3}_{v0,v1} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "-2 t^{o2}_{v0} t^{o3}_{v2} t^{o0,o1}_{v1,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "1/2 t^{o0}_{v2} t^{o1}_{v3} t^{o2,o3}_{v0,v1} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "1/2 t^{o2}_{v0} t^{o3}_{v1} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_sum(
#           "-4 t^{o2}_{v0} t^{o0}_{v2} t^{o1,o3}_{v1,v3} v^{v2,v3}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[[Voovv,T1],T1],T2] (26)"})
# }

# test_return_t test_r2_31() {
#   auto T1 = make_operator("t", {"o->v"})
#   auto Voovv = make_operator("v", {"vv->oo"})

#   // 1/24 [[[[Voovv,T1],T1],T1],T1]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 24),
#       commutator(commutator(commutator(commutator(Voovv, T1), T1), T1), T1), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val_test = string_to_sum(
#       "t^{o2}_{v0} t^{o3}_{v1} t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl)
#   bool pass = (val == val_test)

#   return make_return_t(
#       TestPass, pass, {"CCSD T2 Residual 1/24 [[[[Voovv,T1],T1],T1],T1] (31)"})
# }
