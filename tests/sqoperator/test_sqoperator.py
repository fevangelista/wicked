import wicked as w


def test_sqoperator():
    """Test the SQOperator class"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c"])
    w.add_space("p", "boson", "unoccupied", ["u", "v"])

    cop = w.sqoperator("o_0", w.type.cre)
    assert str(cop) == "a+(o0)"
    cop = w.sqoperator("v_1", w.type.ann)
    assert str(cop) == "a-(v1)"
    cop = w.sqoperator("p_0", w.type.cre)
    assert str(cop) == "b+(p0)"
    cop = w.sqoperator("p_1", w.type.ann)
    assert str(cop) == "b-(p1)"

    cop = w.cre("o_0")
    assert str(cop) == "a+(o0)"
    aop = w.ann("v_1")
    assert str(aop) == "a-(v1)"


if __name__ == "__main__":
    test_sqoperator()
