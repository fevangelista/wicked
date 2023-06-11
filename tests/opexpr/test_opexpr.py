import wicked as w

def test_opexpr1():
    """Test the definition of an operator expression"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])

    T1 = w.op("t", ["v+ o"])
    T2 = w.op("t", ["v+ v+ o o"])
    T3 = T1 + T2
    T = w.op("t", ["v+ o", "v+ v+ o o"])
    assert T == T3

def test_opexpr2():
    """Test the canonicalization of an operator expression"""
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])

    A = w.op("a", ["v+ v"])
    B = w.op("b", ["o+ o"])
    C = w.op("c", ["o+ o"])

    prod = C @ B @ A
    assert str(prod) == "+ c { o+ o } b { o+ o } a { v+ v }"
    # if we canonicalize, we get a different ordering but preserve
    # the order of operators that do not commute
    prod.canonicalize()
    assert str(prod) == "+ a { v+ v } c { o+ o } b { o+ o }"

if __name__ == "__main__":
    test_opexpr1()
    test_opexpr2()
