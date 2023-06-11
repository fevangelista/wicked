import wicked as w


def test_cancellation():
    """Test cancellation of equivalent terms in a specific example involving a general orbital space"""
    w.reset_space()
    w.add_space("a", "fermion", "general", ["u", "v", "w", "x", "y", "z"])

    T2 = w.op("t", ["a+ a+ a a"])
    V = w.op("v", ["a+ a+ a a"])

    wt = w.WickTheorem()
    wt.do_canonicalize_graph(True)
    val = wt.contract(w.rational(1), w.commutator(V, T2), 0, 0)
    ref = w.utils.string_to_expr(
        """1/4 eta1^{a1}_{a0} eta1^{a3}_{a2} gamma1^{a5}_{a4} gamma1^{a7}_{a6} t^{a4,a6}_{a1,a3} v^{a0,a2}_{a5,a7}
-1/4 eta1^{a1}_{a0} eta1^{a3}_{a2} gamma1^{a5}_{a4} gamma1^{a7}_{a6} t^{a0,a2}_{a5,a7} v^{a4,a6}_{a1,a3}
+1/8 eta1^{a1}_{a0} eta1^{a3}_{a2} lambda2^{a6,a7}_{a4,a5} t^{a4,a5}_{a1,a3} v^{a0,a2}_{a6,a7}
-1/8 eta1^{a1}_{a0} eta1^{a3}_{a2} lambda2^{a6,a7}_{a4,a5} t^{a0,a2}_{a6,a7} v^{a4,a5}_{a1,a3}
-eta1^{a1}_{a0} gamma1^{a3}_{a2} lambda2^{a6,a7}_{a4,a5} t^{a2,a4}_{a1,a7} v^{a0,a5}_{a3,a6}
+eta1^{a1}_{a0} gamma1^{a3}_{a2} lambda2^{a6,a7}_{a4,a5} t^{a0,a5}_{a3,a6} v^{a2,a4}_{a1,a7}
-1/4 eta1^{a1}_{a0} lambda3^{a5,a6,a7}_{a2,a3,a4} t^{a2,a3}_{a1,a7} v^{a0,a4}_{a5,a6}
+1/4 eta1^{a1}_{a0} lambda3^{a5,a6,a7}_{a2,a3,a4} t^{a0,a4}_{a5,a6} v^{a2,a3}_{a1,a7}
-1/8 gamma1^{a1}_{a0} gamma1^{a3}_{a2} lambda2^{a6,a7}_{a4,a5} t^{a4,a5}_{a1,a3} v^{a0,a2}_{a6,a7}
+1/8 gamma1^{a1}_{a0} gamma1^{a3}_{a2} lambda2^{a6,a7}_{a4,a5} t^{a0,a2}_{a6,a7} v^{a4,a5}_{a1,a3}
-1/4 gamma1^{a1}_{a0} lambda3^{a5,a6,a7}_{a2,a3,a4} t^{a3,a4}_{a1,a5} v^{a0,a2}_{a6,a7}
+1/4 gamma1^{a1}_{a0} lambda3^{a5,a6,a7}_{a2,a3,a4} t^{a0,a2}_{a6,a7} v^{a3,a4}_{a1,a5}"""
    )
    assert val == ref


if __name__ == "__main__":
    test_cancellation()
