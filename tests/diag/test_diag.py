import wicked as w


def test_diag():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])

    T1 = w.op("t", ["v+ o"])
    assert str(T1) == "+ t { v+ o }"

    T2 = w.op("t", ["v+ v+ o o"])
    assert str(T2) == "+ t { v+ v+ o o }"

    O2 = w.op("O", ["o+ v+ v o"])
    assert str(O2) == "+ O { o+ v+ v o }"

    O2 = w.op("O", ["o+ v+ o v"])
    assert str(O2) == "+ O { o+ v+ v o }"

    O2 = w.op("O", ["v+ o+ v o"])
    assert str(O2) == "+ O { o+ v+ v o }"

    O2 = w.op("O", ["v+ o+ o v"])
    assert str(O2) == "+ O { o+ v+ v o }"

    H1 = w.op("h", ["o+ o", "v+ o", "o+ v", "v+ v"])
    assert (
        str(H1)
        == """+ h { v+ v }
+ h { v+ o }
+ h { o+ v }
+ h { o+ o }"""
    )


if __name__ == "__main__":
    test_diag()
