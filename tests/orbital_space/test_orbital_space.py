import wicked as w


def test_orbital_space():
    w.reset_space()
    w.add_space("c", "fermion", "occupied", ["m", "n"])
    w.add_space("a", "fermion", "general", ["u", "v", "w", "x", "y", "z"])
    w.add_space("v", "fermion", "unoccupied", ["e", "f"])
    w.add_space("h", "fermion", "composite", ["i", "j", "k", "l"], ["c", "a"])
    w.add_space("p", "fermion", "composite", ["a", "b", "c", "d"], ["a", "v"])
    assert w.num_spaces() == 5


if __name__ == "__main__":
    test_orbital_space()
