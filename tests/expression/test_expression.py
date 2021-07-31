import wicked as w


def test_expression():
    """Test the Tensor class"""
    w.reset_space()
    w.add_space("o", "occupied", ["i", "j"])
    w.add_space("a", "general", ["u", "v"])
    w.add_space("v", "unoccupied", ["a", "b", "c"])

    expr = w.Expression()

    term = w.Term()
    term.add([w.Fcre("v_0"), w.Fann("o_0")])
    expr.add(term)

    assert str(expr) == "{ a+(v0) a-(o0) }"

    term = w.SymbolicTerm()
    term.add([w.Fcre("a_0")])
    expr.add(term, w.rational(1, 2))
    expr_str = """{ a+(v0) a-(o0) }
+1/2 { a+(a0) }"""
    assert str(expr) == expr_str

    expr2 = w.Expression()

    term = w.SymbolicTerm()
    term.add([w.Fcre("a_0")])
    expr2.add(term)

    expr_str = """{ a+(a0) }"""
    assert str(expr2) == expr_str

    expr.add(expr2)
    expr_str = """{ a+(v0) a-(o0) }
+3/2 { a+(a0) }"""
    assert str(expr) == expr_str


def test_expression2():
    w.reset_space()
    w.add_space("o", "occupied", ["i", "j"])
    w.add_space("a", "general", ["u", "v"])
    w.add_space("v", "unoccupied", ["a", "b", "c"])
    expr = w.operator("T", ["v+ v+ o o"])
    assert str(expr) == "T^{o0,o1}_{v0,v1} { a+(v0) a+(v1) a-(o1) a-(o0) }"
    expr = w.operator("T", ["v+ v+ v v"])
    assert str(expr) == "T^{v2,v3}_{v0,v1} { a+(v0) a+(v1) a-(v3) a-(v2) }"
    expr = w.operator("T", ["v+ a+ a o"])
    assert str(expr) == "T^{o0,a1}_{v0,a0} { a+(v0) a+(a0) a-(a1) a-(o0) }"
    expr = w.operator("T", ["v+ a+ o a"])


if __name__ == "__main__":
    test_expression()
    test_expression2()
