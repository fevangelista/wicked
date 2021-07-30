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


if __name__ == "__main__":
    test_expression()
