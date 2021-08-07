import wicked as w


def test_diag():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])

    T1 = w.op("t", ["o->v"])
    assert str(T1) == "+ t { v+ o }"

    T2 = w.op("t", ["oo->vv"])
    assert str(T2) == "+ t { v+ v+ o o }"

    O2 = w.op("O", ["ov->ov"])
    assert str(O2) == "+ O { o+ v+ v o }"

    O2 = w.op("O", ["ov->vo"])
    assert str(O2) == "+ O { o+ v+ v o }"

    O2 = w.op("O", ["vo->vo"])
    assert str(O2) == "+ O { o+ v+ v o }"

    O2 = w.op("O", ["vo->ov"])
    assert str(O2) == "+ O { o+ v+ v o }"


if __name__ == "__main__":
    test_diag()
