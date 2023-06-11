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


def test_odd_operators_1():
    """Test <o+ o>"""
    initialize()
    A = w.op("a", ["o+"])
    B = w.op("b", ["o"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), A @ B, 0, 0)
    val2 = w.expression("a^{}_{o_0} b^{o_0}_{}")
    print_comparison(val, val2)
    assert val == val2


def test_odd_operators_2():
    """Test <o o+>"""
    initialize()
    A = w.op("a", ["o"])
    B = w.op("b", ["o+"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), A @ B, 0, 0)
    val2 = w.expression("")
    print_comparison(val, val2)
    assert val == val2


def test_odd_operators_3():
    """Test <o+ v o>"""
    initialize()
    A = w.op("a", ["o+ v"])
    B = w.op("b", ["o"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), A @ B, 0, 1)
    val2 = w.expression("-a^{v0}_{o0} b^{o0}_{} a-(v0)")
    print_comparison(val, val2)
    assert val == val2


def test_odd_operators_4():
    """Test <o+ o+ o o>, <o+ o+ o o>, and <o o+ o+ o>"""
    initialize()
    A = w.op("a", ["o+ o+"])
    B = w.op("b", ["o"])
    C = w.op("c", ["o"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), A @ B @ C, 0, 0)
    val2 = w.expression("- a^{}_{o0,o1} b^{o0}_{} c^{o1}_{}")
    print_comparison(val, val2)
    assert val == val2

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), A @ C @ B, 0, 0)
    val2 = w.expression("a^{}_{o0,o1} b^{o0}_{} c^{o1}_{}")
    print_comparison(val, val2)
    assert val == val2

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), C @ A @ B, 0, 0)
    val2 = w.expression("")
    print_comparison(val, val2)
    assert val == val2


if __name__ == "__main__":
    test_odd_operators_1()
    test_odd_operators_2()
    test_odd_operators_3()
    test_odd_operators_4()
