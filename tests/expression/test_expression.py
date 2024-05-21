import wicked as w


def test_expression():
    """Test the Expression class"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c"])

    expr = w.Expression()

    term = w.Term()
    term.add([w.cre("v_0"), w.ann("o_0")])
    term.set_normal_ordered(True)
    expr.add(term)
    assert str(expr) == "{ a+(v0) a-(o0) }"

    term = w.SymbolicTerm()
    term.set_normal_ordered(True)
    term.add([w.cre("a_0")])
    expr.add(term, w.rational(1, 2))
    expr_str = """1/2 { a+(a0) }
+{ a+(v0) a-(o0) }"""
    assert str(expr) == expr_str

    expr2 = w.Expression()

    term = w.SymbolicTerm()
    term.set_normal_ordered(True)
    term.add([w.cre("a_0")])
    expr2.add(term)

    expr_str = """{ a+(a0) }"""
    assert str(expr2) == expr_str

    expr.add(expr2)
    expr_str = """3/2 { a+(a0) }
+{ a+(v0) a-(o0) }"""
    assert str(expr) == expr_str
    for idx, i in enumerate(expr):
        if (idx == 0): 
            assert str(i[0])=='{ a+(a0) }' 
            assert i[1]==w.rational(3,2)
        if (idx == 1):
            assert str(i[0])=='{ a+(v0) a-(o0) }'
            assert i[1]==w.rational(1,1)


def test_expression2():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c"])
    expr = w.operator_expr("T", ["v+ v+ o o"], True)
    assert str(expr) == "T^{o0,o1}_{v0,v1} { a+(v0) a+(v1) a-(o1) a-(o0) }"
    expr = w.operator_expr("T", ["v+ v+ v v"], True)
    assert str(expr) == "T^{v2,v3}_{v0,v1} { a+(v0) a+(v1) a-(v3) a-(v2) }"
    expr = w.operator_expr("T", ["v+ a+ a o"], True)
    assert str(expr) == "T^{o0,a1}_{v0,a0} { a+(v0) a+(a0) a-(a1) a-(o0) }"
    expr = w.operator_expr("T", ["v+ a+ o a"], True)
    assert str(expr) == "T^{a1,o0}_{v0,a0} { a+(v0) a+(a0) a-(o0) a-(a1) }"


def test_expression3():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c"])
    # empty string corresponds to no operator
    expr = w.expression("")
    assert str(expr) == ""

    # identity
    expr = w.expression("1")
    assert str(expr) == "1"


def test_expression4():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])
    C = w.op("C", ["o+ o"])
    D = w.op("D", ["v+ v"])
    CD_bch = w.bch_series(C, D, 2)
    CD_bch.canonicalize()
    assert CD_bch.size() == 1


def test_expression5():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])
    # empty string corresponds to no operator
    expr = w.expression("f^{o0}_{}")
    print(str(expr))
    assert str(expr) == "f^{o0}_{}"


if __name__ == "__main__":
    test_expression()
    test_expression2()
    test_expression3()
    test_expression4()
    test_expression5()
