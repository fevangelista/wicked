import wicked as w


def print_comparison(val, val2):
    print(f"Result: {val}")
    print(f"Test:   {val2}")


def initialize():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])


def test_energy1():
    """CCSD Energy <F T1> (1)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Fov = w.op("f", ["o+ v"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), Fov @ T1, 0, 0)
    val2 = w.string_to_expr("f^{v_0}_{o_0} t^{o_0}_{v_0}")
    print_comparison(val, val2)
    assert val == val2


def test_energy2():
    """CCSD Energy <V T2> (2)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Voovv = w.op("v", ["o+ o+ v v"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), Voovv @ T2, 0, 0)
    val2 = w.string_to_expr("1/4 t^{o_0,o_1}_{v_0,v_1} v^{v_0,v_1}_{o_0,o_1}")
    print_comparison(val, val2)
    assert val == val2


def test_energy3():
    """CCSD Energy 1/2 <V T1 T1> (3)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Voovv = w.op("v", ["o+ o+ v v"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1, 2), Voovv @ T1 @ T1, 0, 0)
    val2 = w.string_to_expr("1/2 t^{o0}_{v0} t^{o1}_{v1} v^{v0,v1}_{o0,o1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_1():
    """CCSD T1 Residual Fov (1)"""
    initialize()
    Fvo = w.op("f", ["v+ o"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Fvo, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("f^{o0}_{v0}").canonicalize()
    print_comparison(val, val2)
    assert val == val2


def test_r1_2():
    """CCSD T1 Residual [Fvv,T1] (2)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Fvv = w.op("f", ["v+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Fvv @ T1, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("f^{v1}_{v0} t^{o0}_{v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_3():
    """CCSD T1 Residual [Foo,T1] (3)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Foo = w.op("f", ["o+ o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Foo @ T1, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1 f^{o0}_{o1} t^{o1}_{v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_4():
    """CCSD T1 Residual [Vovov,T1] (4)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Vovov = w.op("v", ["o+ v+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vovov @ T1, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1 t^{o1}_{v1} v^{o0,v1}_{o1,v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_5():
    """CCSD T1 Residual [Fvo,T2] (5)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Fov = w.op("f", ["o+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Fov @ T2, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("1 f^{v1}_{o1} t^{o0,o1}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_6():
    """CCSD T1 Residual [Vovvv,T2] (6)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Vovvv = w.op("v", ["o+ v+ v v"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vovvv @ T2, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1/2 t^{o0,o1}_{v1,v2} v^{v1,v2}_{o1,v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_7():
    """CCSD T1 Residual [Vooov,T2] (7)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Vooov = w.op("v", ["o+ o+ v o"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vooov @ T2, 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1/2 t^{o1,o2}_{v0,v1} v^{o0,v1}_{o1,o2}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_8():
    """CCSD T1 Residual 1/2 [[Fov,T1],T1] (8)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Fov = w.op("f", ["o+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Fov, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1 f^{v1}_{o1} t^{o1}_{v0} t^{o0}_{v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_9():
    """CCSD T1 Residual 1/2 [[Vooov,T1],T1] (9)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Vooov = w.op("v", ["o+ o+ v o"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vooov, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1 t^{o1}_{v0} t^{o2}_{v1} v^{o0,v1}_{o1,o2}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_10():
    """CCSD T1 Residual 1/2 [[Vovvv,T1],T1] (10)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Vovvv = w.op("v", ["o+ v+ v v"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vovvv, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1 t^{o0}_{v1} t^{o1}_{v2} v^{v1,v2}_{o1,v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_11():
    """CCSD T1 Residual 1/6 [[[Voovv,T1],T1],T1] (11)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Voovv = w.op("v", ["o+ o+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(
        w.rational(1, 6),
        w.commutator(w.commutator(w.commutator(Voovv, T1), T1), T1),
        2,
        2,
    )
    val = sum.to_manybody_equation("r")[2]
    val2 = w.string_to_expr("-1 t^{o1}_{v0} t^{o0}_{v1} t^{o2}_{v2} v^{v1,v2}_{o1,o2}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_12_14():
    """CCSD T1 Residual [[Voovv,T1],T2] (12-14)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    T2 = w.op("t", ["v+ v+ o o"])
    Voovv = w.op("v", ["o+ o+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(
        w.rational(1),
        w.commutator(w.commutator(Voovv, T1), T2),
        2,
        2,
    )
    val = sum.to_manybody_equation("r")[2]
    val2 = (
        w.string_to_expr("1 t^{o1}_{v1} t^{o0,o2}_{v0,v2} v^{v1,v2}_{o1,o2}")
        + w.string_to_expr("-1/2 t^{o0}_{v1} t^{o1,o2}_{v0,v2} v^{v1,v2}_{o1,o2}")
        + w.string_to_expr("-1/2 t^{o1}_{v0} t^{o0,o2}_{v1,v2} v^{v1,v2}_{o1,o2}")
    )
    print_comparison(val, val2)
    assert val == val2


def test_r2_1():
    Vvvoo = w.op("v", ["v+ v+ o o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vvvoo, 4, 4)
    val = sum.to_manybody_equation("r")[4]
    val2 = w.string_to_expr("1 v^{o0,o1}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


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
    test_r2_1()

# test_return_t test_r2_2() {
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Fvv = make_operator("f", {"v+ v"})

#   // [Fvv,T2]
#   auto sum = wdt.contract_sum(1, Fvv @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("-2 f^{v2}_{v0} t^{o0,o1}_{v1,v2}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Fvv,T2] (2)"})
# }

# test_return_t test_r2_3() {
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Foo = make_operator("f", {"o+ o"})

#   // [Foo,T2]
#   auto sum = wdt.contract_sum(1, Foo @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("2 f^{o0}_{o2} t^{o1,o2}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Foo,T2] (3)"})
# }

# test_return_t test_r2_4() {
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Voooo = make_operator("v", {"oo->oo"})

#   // [Voooo,T2]
#   auto sum = wdt.contract_sum(1, Voooo @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("1/2 t^{o2,o3}_{v0,v1} v^{o0,o1}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Voooo,T2] (4)"})
# }

# test_return_t test_r2_5() {
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Vvvvv = make_operator("v", {"vv->vv"})

#   // [Vvvvv,T2]
#   auto sum = wdt.contract_sum(1, Vvvvv @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("1/2 t^{o0,o1}_{v2,v3} v^{v2,v3}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vvvvv,T2] (5)"})
# }

# test_return_t test_r2_6() {
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Vovov = make_operator("v", {"o+ v+ v o"})

#   // [Vovov,T2]
#   auto sum = wdt.contract_sum(1, Vovov @ T2, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("-4 t^{o0,o2}_{v0,v2} v^{o1,v2}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vovov,T2] (6)"})
# }

# test_return_t test_r2_7() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Vvvov = make_operator("v", {"ov->vv"})

#   // [Vvvov,T2]
#   auto sum = wdt.contract_sum(1, Vvvov @ T1, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("-2 t^{o0}_{v2} v^{o1,v2}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vvvov,T2] (7)"})
# }

# test_return_t test_r2_8() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Vovoo = make_operator("v", {"oo->ov"})

#   // [Vovoo,T2]
#   auto sum = wdt.contract_sum(1, Vovoo @ T1, 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("-2 t^{o2}_{v0} v^{o0,o1}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass, {"CCSD T2 Residual [Vovoo,T2] (8)"})
# }

# test_return_t test_r2_9_12() {
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Voovv = make_operator("v", {"o+ o+ v v"})

#   // 1/2 [[Voovv,T2],T2]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Voovv, T2), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 =
#       string_to_expr("2 t^{o0,o2}_{v0,v2} t^{o1,o3}_{v1,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "-1 t^{o0,o1}_{v0,v2} t^{o2,o3}_{v1,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "-1 t^{o0,o2}_{v0,v1} t^{o1,o3}_{v2,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "1/4 t^{o2,o3}_{v0,v1} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Voovv,T2],T2] (9-12)"})
# }

# test_return_t test_r2_13() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Voooo = make_operator("v", {"oo->oo"})

#   // 1/2 [[Voooo,T1],T1]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Voooo, T1), T1), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("1 t^{o2}_{v0} t^{o3}_{v1} v^{o0,o1}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Voooo,T1],T1] (13)"})
# }

# test_return_t test_r2_14() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Vvvvv = make_operator("v", {"vv->vv"})

#   // 1/2 [[Vvvvv,T1],T1]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Vvvvv, T1), T1), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{v0,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Vvvvv,T1],T1] (14)"})
# }

# test_return_t test_r2_15() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Vovov = make_operator("v", {"o+ v+ v o"})

#   // 1/2 [[Vovov,T1],T1]
#   auto sum = wdt.contract_sum(scalar_t(1, 2),
#                               commutator(commutator(Vovov, T1), T1), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("4 t^{o2}_{v0} t^{o0}_{v2} v^{o1,v2}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/2 [[Vovov,T1],T1] (15)"})
# }

# test_return_t test_r2_16_17() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto T2 = make_operator("t", {"v+ v+ o o"})

#   auto Fov = make_operator("f", {"o+ v"})

#   // [[Fov,T1],T2]
#   auto sum = wdt.contract_sum(1, commutator(commutator(Fov, T1), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr("2 f^{v2}_{o2} t^{o0}_{v2} t^{o1,o2}_{v0,v1}") +
#                   string_to_expr("2 f^{v2}_{o2} t^{o2}_{v0} t^{o0,o1}_{v1,v2}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[Fov,T1],T2] (16-17)"})
# }

# test_return_t test_r2_18_21_22() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Vooov = make_operator("v", {"v+ v+ v o"})

#   // [[Vooov,T1],T2]
#   auto sum = wdt.contract_sum(1, commutator(commutator(Vooov, T1), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 =
#       string_to_expr("2 t^{o2}_{v2} t^{o0,o3}_{v0,v1} v^{o1,v2}_{o2,o3}") +
#       string_to_expr("-1 t^{o0}_{v2} t^{o2,o3}_{v0,v1} v^{o1,v2}_{o2,o3}") +
#       string_to_expr("4 t^{o2}_{v0} t^{o0,o3}_{v1,v2} v^{o1,v2}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[Vooov,T1],T2] (18,21,22)"})
# }

# test_return_t test_r2_19_20_23() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Vovvv = make_operator("v", {"o+ v+ v v"})

#   // [[Vovvv,T1],T2]
#   auto sum = wdt.contract_sum(1, commutator(commutator(Vovvv, T1), T2), 4, 4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 =
#       string_to_expr("2 t^{o2}_{v2} t^{o0,o1}_{v0,v3} v^{v2,v3}_{o2,v1}") +
#       string_to_expr("4 t^{o0}_{v2} t^{o1,o2}_{v0,v3} v^{v2,v3}_{o2,v1}") +
#       string_to_expr("-1 t^{o2}_{v0} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[Vovvv,T1],T2] (19,20,23)"})
# }

# test_return_t test_r2_24() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Vovvv = make_operator("v", {"o+ v+ v v"})

#   // 1/6 [[[Vovvv,T1],T1],T1]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 6), commutator(commutator(commutator(Vovvv, T1), T1), T1), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 =
#       string_to_expr("-2 t^{o2}_{v0} t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{o2,v1}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/6 [[[Vovvv,T1],T1],T1] (24)"})
# }

# test_return_t test_r2_25() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Vooov = make_operator("v", {"v+ v+ v o"})

#   // 1/6 [[[Vooov,T1],T1],T1]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 6), commutator(commutator(commutator(Vooov, T1), T1), T1), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 =
#       string_to_expr("-2 t^{o2}_{v0} t^{o3}_{v1} t^{o0}_{v2} v^{o1,v2}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual 1/6 [[[Vooov,T1],T1],T1] (25)"})
# }

# test_return_t test_r2_26_30() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto T2 = make_operator("t", {"v+ v+ o o"})
#   auto Voovv = make_operator("v", {"o+ o+ v v"})

#   // [[[Voovv,T1],T1],T2]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 2), commutator(commutator(commutator(Voovv, T1), T1), T2), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 =
#       string_to_expr(
#           "-2 t^{o0}_{v2} t^{o2}_{v3} t^{o1,o3}_{v0,v1} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "-2 t^{o2}_{v0} t^{o3}_{v2} t^{o0,o1}_{v1,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "1/2 t^{o0}_{v2} t^{o1}_{v3} t^{o2,o3}_{v0,v1} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "1/2 t^{o2}_{v0} t^{o3}_{v1} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,o3}") +
#       string_to_expr(
#           "-4 t^{o2}_{v0} t^{o0}_{v2} t^{o1,o3}_{v1,v3} v^{v2,v3}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(TestPass, pass,
#                        {"CCSD T2 Residual [[[Voovv,T1],T1],T2] (26)"})
# }

# test_return_t test_r2_31() {
#   auto T1 = make_operator("t", {"v+ o"})
#   auto Voovv = make_operator("v", {"o+ o+ v v"})

#   // 1/24 [[[[Voovv,T1],T1],T1],T1]
#   auto sum = wdt.contract_sum(
#       scalar_t(1, 24),
#       commutator(commutator(commutator(commutator(Voovv, T1), T1), T1), T1), 4,
#       4)
#   Expression val
#   for (const auto &eq : sum.to_manybody_equation("r")) {
#     val.add(eq.rhs(), eq.rhs_factor())
#   }
#   auto val2 = string_to_expr(
#       "t^{o2}_{v0} t^{o3}_{v1} t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{o2,o3}")
#   TEST_DEBUG_PRINT(cout << "Result: " << val << endl)
#   TEST_DEBUG_PRINT(cout << "Test:   " << val2 << endl)
#   bool pass = (val == val2)

#   return make_return_t(
#       TestPass, pass, {"CCSD T2 Residual 1/24 [[[[Voovv,T1],T1],T1],T1] (31)"})
# }
