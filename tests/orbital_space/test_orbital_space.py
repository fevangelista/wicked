import wicked as w
import pytest


def test_orbital_space():
    w.reset_space()
    w.add_space("c", "fermion", "occupied", ["m", "n"])
    w.add_space("a", "fermion", "general", ["u", "v", "w", "x", "y", "z"])
    w.add_space("v", "fermion", "unoccupied", ["e", "f"])
    assert w.num_spaces() == 3


def test_orbital_space_exceptions():
    w.reset_space()
    w.add_space("c", "fermion", "occupied", ["m", "n"])
    with pytest.raises(Exception):
        w.add_space("c", "fermion", "occupied", ["m", "n"])
    with pytest.raises(Exception):
        w.add_space("v", "fermion", "occupied", ["m", "n"])


if __name__ == "__main__":
    test_orbital_space()
    test_orbital_space_exceptions()
