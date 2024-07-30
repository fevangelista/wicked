import pytest
import wicked as w


def test_is_normal_ordered():
    """Test the normal_ordered method of the Expression class on pairs of operators"""
    w.reset_space()
    w.add_space("g", "fermion", "unoccupied", list("abcdefghijklmnopqrstuvwxyz"))

    e = w.expression("a-(g_0) a+(g_1)")
    assert not e.is_vacuum_normal_ordered()

    e = w.expression("a+(g_0) a-(g_1)")
    assert e.is_vacuum_normal_ordered()

    w.reset_space()
    w.add_space("a", "fermion", "unoccupied", list("abcdefghijkl"))
    w.add_space("b", "fermion", "unoccupied", list("mnopqrstuvwxyz"))

    e = w.expression("a-(a_0) a+(a_1)")
    assert not e.is_vacuum_normal_ordered()

    e = w.expression("a+(a_0) a+(b_0) a-(b_1) a-(a_1)")
    assert e.is_vacuum_normal_ordered()

    e = w.expression("a+(a_0) a+(b_0) a-(b_3) a-(a_1)")
    assert e.is_vacuum_normal_ordered()

    e = w.expression("a+(a_0) a+(b_0) a-(b_3) a-(a_7)")
    assert e.is_vacuum_normal_ordered()

    e = w.expression("a+(a_0) a+(b_0) a-(a_7) a-(b_3)")
    assert not e.is_vacuum_normal_ordered()

    e = w.expression("a+(a_0) a-(a_7) a+(b_0) a-(b_3)")
    assert not e.is_vacuum_normal_ordered()


def test_expression_normal_ordering1():
    """Test the normal_ordered method of the Expression class on pairs of operators"""
    w.reset_space()
    w.add_space("g", "fermion", "unoccupied", list("abcdefghijklmnopqrstuvwxyz"))

    e = w.expression("a-(g_0) a+(g_1)")
    e = e.normal_ordered()
    print(f"{e = }")

    e = w.expression("a+(g_0) a-(g_1)")
    e = e.normal_ordered()
    print(f"{e = }")


if __name__ == "__main__":
    test_is_normal_ordered()
    test_expression_normal_ordering1()
