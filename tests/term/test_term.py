import wicked as w


def test_term():
    """Test the Tensor class"""
    w.reset_space()
    w.add_space("o", "occupied", ["i", "j"])
    w.add_space("a", "general", ["u", "v"])
    w.add_space("v", "unoccupied", ["a", "b", "c"])

    # Create an empty term
    term = w.Term()
    assert str(term) == "1"

    # Create a term from a symbolic term
    term = w.SymbolicTerm()
    term.set_normal_ordered(True)
    term.add([w.Fcre("v_0"), w.Fann("o_0")])
    term = w.Term(term)
    assert str(term) == "{ a+(v0) a-(o0) }"

    # Create a term from scratch
    term = w.Term()
    term.set_normal_ordered(True)
    term.set(w.rational(1, 2))
    term.add([w.Fcre("v_0"), w.Fann("o_0")])
    term.add(w.tensor("T", ["v_0"], ["o_0"], w.sym.anti))
    assert str(term) == "1/2 T^{o0}_{v0} { a+(v0) a-(o0) }"
    assert term.latex() == "+\\frac{1}{2} T^{i}_{a} \\{ \\hat{a}^{a} \\hat{a}_{i} \\}"


if __name__ == "__main__":
    test_term()
