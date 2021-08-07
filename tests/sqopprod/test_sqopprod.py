import wicked as w
from wicked import index


def test_sqopprod():
    """Test the SQOpProd class"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "occupied", ["a", "b", "c"])

    opprod = w.sqopprod([], [])
    assert str(opprod) == ""

    opprod = w.sqopprod(["v_0"], [])
    assert str(opprod) == "a+(v0)"

    opprod = w.sqopprod([], ["o_0"])
    assert str(opprod) == "a-(o0)"

    opprod = w.sqopprod(["v_0"], ["o_0"])
    assert str(opprod) == "a+(v0) a-(o0)"
    assert opprod.latex() == "\hat{a}^\dagger_{a} \hat{a}_{i}"

    opprod = w.sqopprod(["v_0", "v_1"], ["o_0", "o_1"])
    assert str(opprod) == "a+(v0) a+(v1) a-(o1) a-(o0)"

    opprod1 = w.sqopprod(["v_0", "v_1"], ["o_0", "o_1"])
    opprod2 = w.sqopprod(["v_0", "v_1"], ["o_0", "o_1"])
    assert opprod1 == opprod2

    opprod1 = w.sqopprod(["v_0"], [])
    opprod2 = w.sqopprod(["v_0"], [])
    assert not (opprod1 < opprod2)

    opprod1 = w.sqopprod(["v_0"], [])
    opprod2 = w.sqopprod(["v_1"], [])
    assert opprod1 < opprod2

    # let's test a bunch of combinations

    opprod1 = w.sqopprod(["v_0"], ["o_0"])
    opprod2 = w.sqopprod(["v_0"], ["o_0"])
    assert opprod1 == opprod2
    assert not (opprod1 < opprod2)

    opprod1 = w.sqopprod(["v_0"], ["o_0"])
    opprod2 = w.sqopprod(["v_1"], ["o_0"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["v_0"], ["o_1"])
    opprod2 = w.sqopprod(["v_1"], ["o_0"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["v_1"], ["o_0"])
    opprod2 = w.sqopprod(["v_1"], ["o_1"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["v_1"], ["o_1"])
    opprod2 = w.sqopprod(["v_1"], ["o_0"])
    assert not (opprod1 < opprod2)

    opprod1 = w.sqopprod(["v_1"], ["o_2"])
    opprod2 = w.sqopprod(["v_1", "v_2"], ["o_0"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["v_2"], ["o_2"])
    opprod2 = w.sqopprod(["v_1", "v_2"], ["o_0"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["o_4"], ["o_2"])
    opprod2 = w.sqopprod(["v_1", "v_2"], ["o_0"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["o_4"], ["o_2"])
    opprod2 = w.sqopprod(["a_1", "o_2"], ["o_0"])
    assert opprod1 < opprod2

    opprod1 = w.sqopprod(["a_4"], ["o_2"])
    opprod2 = w.sqopprod(["a_4"], ["a_2"])
    assert opprod1 < opprod2


if __name__ == "__main__":
    test_sqopprod()
