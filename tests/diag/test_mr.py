import wicked as w


def print_comparison(val, val2):
    print(f"\n{'-'*30} Result {'-'*30}\n{val}")
    print(f"{'-'*31} Test {'-'*31}\n{val2}")


def initialize():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("a", "fermion", "general", ["u", "v", "w", "x", "y", "z"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])


def test_mr1():
    initialize()
    T1aa = w.op("t", ["a+ a"])
    Faa = w.op("f", ["a+ a"])

    wt = w.WickTheorem()
    val = wt.contract(w.rational(1), Faa @ T1aa, 0, 0)
    ref = w.utils.string_to_expr(
        """eta1^{a1}_{a0} f^{a0}_{a2} gamma1^{a2}_{a3} t^{a3}_{a1}"
f^{a1}_{a0} lambda2^{a0,a3}_{a1,a2} t^{a2}_{a3}"""
    )
    print_comparison(val, ref)
    assert val == ref


def test_mr2():
    initialize()
    T1aa = w.op("t", ["a+ a"])
    Faa = w.op("f", ["a+ a"])

    wt = w.WickTheorem()
    wt.set_print(w.PrintLevel.summary)
    val = wt.contract(w.rational(1), Faa @ T1aa @ T1aa, 0, 2)
    ref_expr = """eta1^{a1}_{a0} eta1^{a3}_{a2} f^{a2}_{a4} gamma1^{a4}_{a5} t^{a5}_{a1} t^{a0}_{a3}
+eta1^{a1}_{a0} f^{a3}_{a2} lambda2^{a2,a5}_{a3,a4} t^{a0}_{a5} t^{a4}_{a1}
-eta1^{a1}_{a0} f^{a0}_{a2} gamma1^{a4}_{a3} gamma1^{a2}_{a5} t^{a5}_{a4} t^{a3}_{a1}
+eta1^{a1}_{a0} f^{a0}_{a2} lambda2^{a2,a5}_{a3,a4} t^{a4}_{a5} t^{a3}_{a1}
-eta1^{a1}_{a0} f^{a0}_{a2} lambda2^{a2,a5}_{a3,a4} t^{a3}_{a5} t^{a4}_{a1}
-f^{a1}_{a0} gamma1^{a3}_{a2} lambda2^{a0,a5}_{a1,a4} t^{a2}_{a5} t^{a4}_{a3}
-f^{a1}_{a0} gamma1^{a0}_{a2} lambda2^{a4,a5}_{a1,a3} t^{a3}_{a5} t^{a2}_{a4}
+f^{a1}_{a0} gamma1^{a0}_{a2} lambda2^{a4,a5}_{a1,a3} t^{a3}_{a4} t^{a2}_{a5}
-f^{a1}_{a0} lambda3^{a0,a4,a5}_{a1,a2,a3} t^{a2}_{a5} t^{a3}_{a4}"""
    ref = w.utils.string_to_expr(ref_expr)
    # assert val == ref


def test_mr3():
    initialize()
    T2aaaa = w.op("t", ["a+ a+ a a"])
    Vaaaa = w.op("f", ["a+ a+ a a"])

    wt = w.WickTheorem()
    wt.set_print(w.PrintLevel.summary)
    val = wt.contract(w.rational(1), Vaaaa @ T2aaaa, 0, 0)
    ref_expr = """eta1^{a1}_{a0} eta1^{a3}_{a2} f^{a2}_{a4} gamma1^{a4}_{a5} t^{a5}_{a1} t^{a0}_{a3}
+eta1^{a1}_{a0} f^{a3}_{a2} lambda2^{a2,a5}_{a3,a4} t^{a0}_{a5} t^{a4}_{a1}
-eta1^{a1}_{a0} f^{a0}_{a2} gamma1^{a4}_{a3} gamma1^{a2}_{a5} t^{a5}_{a4} t^{a3}_{a1}
+eta1^{a1}_{a0} f^{a0}_{a2} lambda2^{a2,a5}_{a3,a4} t^{a4}_{a5} t^{a3}_{a1}
-eta1^{a1}_{a0} f^{a0}_{a2} lambda2^{a2,a5}_{a3,a4} t^{a3}_{a5} t^{a4}_{a1}
-f^{a1}_{a0} gamma1^{a3}_{a2} lambda2^{a0,a5}_{a1,a4} t^{a2}_{a5} t^{a4}_{a3}
-f^{a1}_{a0} gamma1^{a0}_{a2} lambda2^{a4,a5}_{a1,a3} t^{a3}_{a5} t^{a2}_{a4}
+f^{a1}_{a0} gamma1^{a0}_{a2} lambda2^{a4,a5}_{a1,a3} t^{a3}_{a4} t^{a2}_{a5}
-f^{a1}_{a0} lambda3^{a0,a4,a5}_{a1,a2,a3} t^{a2}_{a5} t^{a3}_{a4}"""
    ref = w.utils.string_to_expr(ref_expr)


if __name__ == "__main__":
    test_mr1()
    test_mr2()
    test_mr3()
