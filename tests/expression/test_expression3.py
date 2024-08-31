import pytest
import wicked as w


def test_expression1():
    """Test the vacuum_normal_ordered method of the Expression class on pairs of operators"""
    w.reset_space()
    w.add_space("g", "fermion", "unoccupied", list("abcdefghijklmnopqrstuvwxyz"))

    # test same-type ordered
    e = w.expression("a+(g_0) a+(g_1)")
    e = w.expression("{a+(g_0) a+(g_1) }")
    e = w.expression("{a+(g_0) a+(g_1)}")
    e = w.expression("{a+(g_0) a+(g_1) }")

    # ensure that normal ordered expressions cannot be vacuum normal ordered
    with pytest.raises(
        RuntimeError, match=r".*can be applied only to non-normal ordered.*"
    ):
        e.vacuum_normal_ordered()


def test_expression2():
    w.reset_space()
    w.add_space("h", "fermion", "occupied", list("ijkl"))
    w.add_space("p", "fermion", "unoccupied", list("abcd"))

    e = w.expression("a+(h_1) a-(h_2)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a+(h_1) a-(h_2)")
    assert expr == test_expr

    e = w.expression("a-(p_2) a+(p_1)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(p_1) a-(p_2)") + w.expression("delta^{p1}_{p2}")
    assert expr == test_expr

    e = w.expression("a-(p_2) a+(h_1) a-(h_2) a+(p_1)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(h_1) a+(p_1) a-(p_2) a-(h_2)") + w.expression(
        "delta^{p1}_{p2} a+(h1) a-(h2)"
    )
    assert expr == test_expr


def test_expression3():
    """Test the vacuum_normal_ordered method for bosonic operators"""
    w.reset_space()
    w.add_space("v", "boson", "unoccupied", list("uv"))

    expr = w.expression("b-(v_1) b+(v_1)")
    expr = expr.vacuum_normal_ordered()
    test_expr = w.expression("b+(v1) b-(v1)") + w.expression("1")
    assert expr == test_expr

    expr = w.expression("b+(v_1) b+(v_1)")
    expr = expr.vacuum_normal_ordered()
    test_expr = w.expression("b+(v_1) b+(v_1)")
    assert expr == test_expr

    expr = w.expression("b+(v_1) b+(v_1) b+(v_1)")
    expr = expr.vacuum_normal_ordered()
    test_expr = w.expression("b+(v_1) b+(v_1) b+(v_1)")
    assert expr == test_expr

    expr = w.expression("b-(v_1) b-(v_1) b+(v_1) b+(v_1)")
    expr = expr.vacuum_normal_ordered()
    # need to use the correct test string here
    test_expr = (
        w.expression("2")
        + w.expression("4 b+(v_1) b-(v_1)")
        + w.expression("b+(v_1) b+(v_1) b-(v_1) b-(v_1)")
    )
    assert expr == test_expr

    expr = w.expression("b+(v_1) b+(v_2) b+(v_1)")
    expr = expr.vacuum_normal_ordered()
    test_expr = w.expression("b+(v_1) b+(v_1) b+(v_2)")
    assert expr == test_expr

    expr = w.expression("b+(v_1) b+(v_2) b-(v_1) b-(v_2)")
    expr = expr.vacuum_normal_ordered()
    test_expr = w.expression("b+(v_1) b+(v_2) b-(v_2) b-(v_1)")
    assert expr == test_expr


if __name__ == "__main__":
    test_expression1()
    test_expression2()
    test_expression3()
