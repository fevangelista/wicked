import wicked as w


def print_comparison(val, val2):
    print(f"Result: {val}")
    print(f"Test:   {val2}")


def compare_expressions(test, ref):
    test_expr = w.Expression()
    ref_expr = w.Expression()
    for s in ref:
        ref_expr += w.string_to_expr(s)
    for eq in test:
        test_expr += eq.rhs_expression()
    print_comparison(test_expr, ref_expr)
    assert test_expr == ref_expr


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
    val2 = w.expression("f^{v_0}_{o_0} t^{o_0}_{v_0}")
    print_comparison(val, val2)
    assert val == val2


def test_energy2():
    """CCSD Energy <V T2> (2)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Voovv = w.op("v", ["o+ o+ v v"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), Voovv @ T2, 0, 0)
    val2 = w.expression("1/4 t^{o_0,o_1}_{v_0,v_1} v^{v_0,v_1}_{o_0,o_1}")
    print_comparison(val, val2)
    assert val == val2


def test_energy3():
    """CCSD Energy 1/2 <V T1 T1> (3)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Voovv = w.op("v", ["o+ o+ v v"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1, 2), Voovv @ T1 @ T1, 0, 0)
    val2 = w.expression("1/2 t^{o0}_{v0} t^{o1}_{v1} v^{v0,v1}_{o0,o1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_1():
    """CCSD T1 Residual Fov (1)"""
    initialize()
    Fvo = w.op("f", ["v+ o"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Fvo, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("f^{o0}_{v0}").canonicalize()
    # print(val[0].rhs_term())
    print_comparison(val, val2)
    assert val == val2


def test_r1_2():
    """CCSD T1 Residual [Fvv,T1] (2)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Fvv = w.op("f", ["v+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Fvv @ T1, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("f^{v1}_{v0} t^{o0}_{v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_3():
    """CCSD T1 Residual [Foo,T1] (3)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Foo = w.op("f", ["o+ o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Foo @ T1, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1 f^{o0}_{o1} t^{o1}_{v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_4():
    """CCSD T1 Residual [Vovov,T1] (4)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Vovov = w.op("v", ["o+ v+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vovov @ T1, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1 t^{o1}_{v1} v^{o0,v1}_{o1,v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_5():
    """CCSD T1 Residual [Fvo,T2] (5)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Fov = w.op("f", ["o+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Fov @ T2, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("1 f^{v1}_{o1} t^{o0,o1}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_6():
    """CCSD T1 Residual [Vovvv,T2] (6)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Vovvv = w.op("v", ["o+ v+ v v"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vovvv @ T2, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1/2 t^{o0,o1}_{v1,v2} v^{v1,v2}_{o1,v0}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_7():
    """CCSD T1 Residual [Vooov,T2] (7)"""
    initialize()
    T2 = w.op("t", ["v+ v+ o o"])
    Vooov = w.op("v", ["o+ o+ v o"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vooov @ T2, 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1/2 t^{o1,o2}_{v0,v1} v^{o0,v1}_{o1,o2}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_8():
    """CCSD T1 Residual 1/2 [[Fov,T1],T1] (8)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Fov = w.op("f", ["o+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Fov, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1 f^{v1}_{o1} t^{o1}_{v0} t^{o0}_{v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_9():
    """CCSD T1 Residual 1/2 [[Vooov,T1],T1] (9)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Vooov = w.op("v", ["o+ o+ v o"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vooov, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1 t^{o1}_{v0} t^{o2}_{v1} v^{o0,v1}_{o1,o2}")
    print_comparison(val, val2)
    assert val == val2


def test_r1_10():
    """CCSD T1 Residual 1/2 [[Vovvv,T1],T1] (10)"""
    initialize()
    T1 = w.op("t", ["v+ o"])
    Vovvv = w.op("v", ["o+ v+ v v"])

    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vovvv, T1), T1), 2, 2)
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1 t^{o0}_{v1} t^{o1}_{v2} v^{v1,v2}_{o1,v0}")
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
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val2 = w.expression("-1 t^{o1}_{v0} t^{o0}_{v1} t^{o2}_{v2} v^{v1,v2}_{o1,o2}")
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
    val = sum.to_manybody_equation("r")["o|v"][0].rhs_expression()
    val += sum.to_manybody_equation("r")["o|v"][1].rhs_expression()
    val += sum.to_manybody_equation("r")["o|v"][2].rhs_expression()
    val2 = (
        w.expression("1 t^{o1}_{v1} t^{o0,o2}_{v0,v2} v^{v1,v2}_{o1,o2}")
        + w.expression("-1/2 t^{o0}_{v1} t^{o1,o2}_{v0,v2} v^{v1,v2}_{o1,o2}")
        + w.expression("-1/2 t^{o1}_{v0} t^{o0,o2}_{v1,v2} v^{v1,v2}_{o1,o2}")
    )
    print_comparison(val, val2)
    assert val == val2


def test_r2_1():
    """CCSD T2 Residual Vvvoo (1)"""
    Vvvoo = w.op("v", ["v+ v+ o o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), Vvvoo, 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("1/4 v^{o0,o1}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_2():
    """CCSD T2 Residual [Fvv,T2] (2)"""
    T2 = w.op("t", ["v+ v+ o o"])
    Fvv = w.op("f", ["v+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Fvv, T2), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("-1/2 f^{v2}_{v0} t^{o0,o1}_{v1,v2}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_3():
    """CCSD T2 Residual [Foo,T2] (3)"""
    T2 = w.op("t", ["v+ v+ o o"])
    Foo = w.op("f", ["o+ o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Foo, T2), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("1/2 f^{o0}_{o2} t^{o1,o2}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_4():
    """CCSD T2 Residual [Voooo,T2] (4)"""
    T2 = w.op("t", ["v+ v+ o o"])
    Voooo = w.op("v", ["o+ o+ o o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Voooo, T2), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("1/8 t^{o2,o3}_{v0,v1} v^{o0,o1}_{o2,o3}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_5():
    """CCSD T2 Residual [Vvvvv,T2] (5)"""
    T2 = w.op("t", ["v+ v+ o o"])
    Vvvvv = w.op("v", ["v+ v+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Vvvvv, T2), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("1/8 t^{o0,o1}_{v2,v3} v^{v2,v3}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_6():
    """CCSD T2 Residual [Vovov,T2] (6)"""
    T2 = w.op("t", ["v+ v+ o o"])
    Vovov = w.op("v", ["o+ v+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Vovov, T2), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("- t^{o0,o2}_{v0,v2} v^{o1,v2}_{o2,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_7():
    """CCSD T2 Residual [Vvvov,T1] (7)"""
    T1 = w.op("t", ["v+ o"])
    Vvvov = w.op("v", ["v+ v+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Vvvov, T1), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("-1/2 t^{o0}_{v2} v^{o1,v2}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_8():
    """CCSD T2 Residual [Vovoo,T1] (8)"""
    T1 = w.op("t", ["v+ o"])
    Vovoo = w.op("v", ["o+ v+ o o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1), w.commutator(Vovoo, T1), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("-1/2 t^{o2}_{v0} v^{o0,o1}_{o2,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_9_12():
    """CCSD T2 Residual 1/2 [[Voovv,T2],T2] (9-12)"""
    T2 = w.op("t", ["v+ v+ o o"])
    Voovv = w.op("v", ["o+ o+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Voovv, T2), T2), 4, 4)
    compare_expressions(
        sum.to_manybody_equation("r")["oo|vv"],
        [
            "1/2 t^{o0,o2}_{v0,v2} t^{o1,o3}_{v1,v3} v^{v2,v3}_{o2,o3}",
            "-1/4 t^{o0,o1}_{v0,v2} t^{o2,o3}_{v1,v3} v^{v2,v3}_{o2,o3}",
            "1/16 t^{o2,o3}_{v0,v1} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,o3}",
            "-1/4 t^{o0,o2}_{v0,v1} t^{o1,o3}_{v2,v3} v^{v2,v3}_{o2,o3}",
        ],
    )


def test_r2_13():
    """CCSD T2 Residual 1/2 [[Voooo,T1],T1] (13)"""
    T1 = w.op("t", ["v+ o"])
    Voooo = w.op("v", ["o+ o+ o o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Voooo, T1), T1), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("1/4 t^{o2}_{v0} t^{o3}_{v1} v^{o0,o1}_{o2,o3}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_14():
    """CCSD T2 Residual 1/2 [[Vvvvv,T1],T1] (14)"""
    T1 = w.op("t", ["v+ o"])
    Vvvvv = w.op("v", ["v+ v+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vvvvv, T1), T1), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("1/4 t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{v0,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_15():
    """CCSD T2 Residual 1/2 [[Vovov,T1],T1] (15)"""
    T1 = w.op("t", ["v+ o"])
    Vovov = w.op("v", ["o+ v+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 2), w.commutator(w.commutator(Vovov, T1), T1), 4, 4)
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("t^{o2}_{v0} t^{o0}_{v2} v^{o1,v2}_{o2,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_16_17():
    """CCSD T2 Residual [[Fov,T1],T2] (16-17)"""
    T1 = w.op("t", ["v+ o"])
    T2 = w.op("t", ["v+ v+ o o"])
    Fov = w.op("f", ["o+ v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 1), w.commutator(w.commutator(Fov, T1), T2), 4, 4)
    compare_expressions(
        sum.to_manybody_equation("r")["oo|vv"],
        [
            "1/2 f^{v2}_{o2} t^{o0}_{v2} t^{o1,o2}_{v0,v1}",
            "1/2 f^{v2}_{o2} t^{o2}_{v0} t^{o0,o1}_{v1,v2}",
        ],
    )


def test_r2_18_21_22():
    """CCSD T2 Residual [[Vooov,T1],T2] (18,21,22)"""
    T1 = w.op("t", ["v+ o"])
    T2 = w.op("t", ["v+ v+ o o"])
    Vooov = w.op("v", ["o+ o+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 1), w.commutator(w.commutator(Vooov, T1), T2), 4, 4)
    compare_expressions(
        sum.to_manybody_equation("r")["oo|vv"],
        [
            "1/2 t^{o2}_{v2} t^{o0,o3}_{v0,v1} v^{o1,v2}_{o2,o3}",
            "-1/4 t^{o0}_{v2} t^{o2,o3}_{v0,v1} v^{o1,v2}_{o2,o3}",
            "t^{o2}_{v0} t^{o0,o3}_{v1,v2} v^{o1,v2}_{o2,o3}",
        ],
    )


def test_r2_19_20_23():
    """CCSD T2 Residual [[Vovvv,T1],T2] (19,20,23)"""
    T1 = w.op("t", ["v+ o"])
    T2 = w.op("t", ["v+ v+ o o"])
    Vovvv = w.op("v", ["o+ v+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(w.rational(1, 1), w.commutator(w.commutator(Vovvv, T1), T2), 4, 4)
    compare_expressions(
        sum.to_manybody_equation("r")["oo|vv"],
        [
            "1/2 t^{o2}_{v2} t^{o0,o1}_{v0,v3} v^{v2,v3}_{o2,v1}",
            "t^{o0}_{v2} t^{o1,o2}_{v0,v3} v^{v2,v3}_{o2,v1}",
            "-1/4 t^{o2}_{v0} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,v1}",
        ],
    )


def test_r2_24():
    """CCSD T2 Residual 1/6 [[[Vovvv,T1],T1],T1] (24)"""
    T1 = w.op("t", ["v+ o"])
    Vovvv = w.op("v", ["o+ v+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(
        w.rational(1, 6),
        w.commutator(w.commutator(w.commutator(Vovvv, T1), T1), T1),
        4,
        4,
    )
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("-1/2 t^{o2}_{v0} t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{o2,v1}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_25():
    """CCSD T2 Residual 1/6 [[[Vooov,T1],T1],T1] (25)"""
    T1 = w.op("t", ["v+ o"])
    Vooov = w.op("v", ["o+ o+ v o"])
    wt = w.WickTheorem()
    sum = wt.contract(
        w.rational(1, 6),
        w.commutator(w.commutator(w.commutator(Vooov, T1), T1), T1),
        4,
        4,
    )
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression("-1/2 t^{o2}_{v0} t^{o3}_{v1} t^{o0}_{v2} v^{o1,v2}_{o2,o3}")
    print_comparison(val, val2)
    assert val == val2


def test_r2_26_30():
    """CCSD T2 Residual [[[Voovv,T1],T1],T2] (26-30)"""
    T1 = w.op("t", ["v+ o"])
    T2 = w.op("t", ["v+ v+ o o"])
    Voovv = w.op("v", ["o+ o+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(
        w.rational(1, 2),
        w.commutator(w.commutator(w.commutator(Voovv, T1), T1), T2),
        4,
        4,
    )
    compare_expressions(
        sum.to_manybody_equation("r")["oo|vv"],
        [
            "-1/2 t^{o0}_{v2} t^{o2}_{v3} t^{o1,o3}_{v0,v1} v^{v2,v3}_{o2,o3}",
            "1/8 t^{o0}_{v2} t^{o1}_{v3} t^{o2,o3}_{v0,v1} v^{v2,v3}_{o2,o3}",
            "-1/2 t^{o2}_{v0} t^{o3}_{v2} t^{o0,o1}_{v1,v3} v^{v2,v3}_{o2,o3}",
            "-1 t^{o2}_{v0} t^{o0}_{v2} t^{o1,o3}_{v1,v3} v^{v2,v3}_{o2,o3}",
            "1/8 t^{o2}_{v0} t^{o3}_{v1} t^{o0,o1}_{v2,v3} v^{v2,v3}_{o2,o3}",
        ],
    )


def test_r2_31():
    """CCSD T2 Residual 1/24 [[[[Voovv,T1],T1],T1],T1] (31)"""
    T1 = w.op("t", ["v+ o"])
    Voovv = w.op("v", ["o+ o+ v v"])
    wt = w.WickTheorem()
    sum = wt.contract(
        w.rational(1, 24),
        w.commutator(w.commutator(w.commutator(w.commutator(Voovv, T1), T1), T1), T1),
        4,
        4,
    )
    val = sum.to_manybody_equation("r")["oo|vv"][0].rhs_expression()
    val2 = w.expression(
        "1/4 t^{o2}_{v0} t^{o3}_{v1} t^{o0}_{v2} t^{o1}_{v3} v^{v2,v3}_{o2,o3}"
    )
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
    test_r2_2()
    test_r2_3()
    test_r2_4()
    test_r2_5()
    test_r2_6()
    test_r2_7()
    test_r2_8()
    test_r2_9_12()
    test_r2_13()
    test_r2_14()
    test_r2_15()
    test_r2_16_17()
    test_r2_18_21_22()
    test_r2_19_20_23()
    test_r2_24()
    test_r2_25()
    test_r2_26_30()
    test_r2_31()
