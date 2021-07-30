import wicked as w


def test_sqoperator():
    """Test the SQOperator class"""
    w.reset_space()
    w.add_space("o", "occupied", ["i", "j"])
    w.add_space("a", "general", ["u", "v"])
    w.add_space("v", "unoccupied", ["a", "b", "c"])

    cop = w.sqoperator("o_0", w.type.cre, w.stat.fermion)
    assert str(cop) == "a+(o0)"
    cop = w.sqoperator("a_1", w.type.ann, w.stat.fermion)
    assert str(cop) == "a-(a1)"
    cop = w.sqoperator("o_0", w.type.cre, w.stat.boson)
    assert str(cop) == "b+(o0)"
    cop = w.sqoperator("a_1", w.type.ann, w.stat.boson)
    assert str(cop) == "b-(a1)"

    cop = w.Fcre("o_0")
    assert str(cop) == "a+(o0)"
    aop = w.Fann("v_1")
    assert str(aop) == "a-(v1)"


if __name__ == "__main__":
    test_sqoperator()
