import pytest
import wicked as w


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
    test_expression_normal_ordering1()
