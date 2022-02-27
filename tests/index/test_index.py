import wicked as w
from wicked import Index, index


def test_index():
    """Test Index class"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "occupied", ["a", "b", "c"])

    i = Index(0, 0)
    assert i.space() == 0
    assert i.pos() == 0
    assert str(i) == "o0"
    assert i.latex() == "i"

    j = index("o_1")
    assert j.space() == 0
    assert j.pos() == 1
    assert str(j) == "o1"
    assert j.latex() == "j"

    k = index("o_2")
    assert k.space() == 0
    assert k.pos() == 2
    assert str(k) == "o2"
    assert k.latex() == "k"

    u = Index(1, 1)
    assert u.space() == 1
    assert u.pos() == 1
    assert str(u) == "a1"
    assert u.latex() == "v"

    v = index("a_10")
    assert v.space() == 1
    assert v.pos() == 10
    assert str(v) == "a10"
    assert v.latex() == "a_{10}"

    a = Index(2, 2)
    assert a.space() == 2
    assert a.pos() == 2
    assert str(a) == "v2"
    assert a.latex() == "c"

    b = index("v_0")
    assert b.space() == 2
    assert b.pos() == 0
    assert str(b) == "v0"
    assert b.latex() == "a"


def test_index2():
    """Test Index class"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k"])
    w.add_space("a", "fermion", "general", ["u", "v"])
    w.add_space("v", "fermion", "occupied", ["a", "b", "c"])
    assert index("o_0") < index("o_1")
    assert index("o_0") < index("a_0")
    assert index("o_0") == index("o_0")
    indices = w.indices("v_0,v_1,a_2,a_0,a_1,o_1,o_0")
    sign, indices = w.canonicalize_indices(indices, False)
    test_indices = w.indices("o_0,o_1,a_0,a_1,a_2,v_0,v_1")
    assert indices == test_indices
    assert sign == w.rational(-1, 1)


if __name__ == "__main__":
    test_index()
    test_index2()
