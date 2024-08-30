import pytest
import wicked as w


def test_expression_product1():
    """Test the product of two expressions"""
    w.reset_space()
    w.add_space("g", "fermion", "unoccupied", list("abcdefghijklmnopqrstuvwxyz"))

    # test same-type ordered
    e1 = w.expression("a+(g_0) a-(g_1)")
    e2 = w.expression("a+(g_2) a-(g_3)")
    e3 = e1 @ e2
    e3 = e3.vacuum_normal_ordered()
    test_e3 = w.expression("a+(g_0) a+(g_2) a-(g_3) a-(g_1)") + w.expression(
        "delta^{g2}_{g1} a+(g_0) a-(g_3)"
    )
    assert e3 == test_e3


# def test_expression2():
#     w.reset_space()
#     w.add_space("h", "fermion", "occupied", list("ijkl"))
#     w.add_space("p", "fermion", "unoccupied", list("abcd"))

#     e = w.expression("a+(h_1) a-(h_2)")
#     expr = e.normal_ordered()
#     test_expr = w.expression("-a-(h2) a+(h1)") + w.expression("delta^{h1}_{h2}")
#     assert expr == test_expr

#     e = w.expression("a-(p_2) a+(p_1)")
#     expr = e.normal_ordered()
#     test_expr = w.expression("-a+(p_1) a-(p_2)") + w.expression("delta^{p1}_{p2}")
#     assert expr == test_expr

#     e = w.expression("a-(p_2) a+(h_1) a-(h_2) a+(p_1)")
#     expr = e.normal_ordered()
#     test_expr = (
#         w.expression("a-(h2) a+(p1) a-(p2) a+(h1)")
#         + w.expression("-delta^{h1}_{h2} a+(p1) a-(p2)")
#         + w.expression("+delta^{h1}_{h2} delta^{p1}_{p2}")
#         + w.expression("-delta^{p1}_{p2} a-(h2) a+(h1)")
#     )
#     assert expr == test_expr


if __name__ == "__main__":
    test_expression_product1()
