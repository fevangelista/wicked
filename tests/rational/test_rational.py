import wicked as w
import pytest


def test_rational():
    """Test the rational number class"""
    import math

    assert w.make_rational("1/2") == w.rational(1, 2)
    assert w.make_rational("+1/2") == w.rational(1, 2)
    assert w.make_rational("+") == w.rational(1, 1)
    assert w.make_rational("") == w.rational(1, 1)
    assert w.make_rational("-") == w.rational(-1, 1)

    assert w.rational(1, 2) + w.rational(1, 2) == w.rational(1, 1)
    assert w.rational(3, 5) - w.rational(1, 2) == w.rational(1, 10)
    assert w.rational(3, 5) * w.rational(1, 2) == w.rational(3, 10)
    assert w.rational(3, 5) / w.rational(1, 2) == w.rational(6, 5)

    assert repr(w.make_rational("1/2")) == "rational(1,2)"
    assert repr(w.make_rational("-12")) == "rational(-12,1)"
    assert repr(w.make_rational("12")) == "rational(12,1)"
    assert repr(w.make_rational("-1/2")) == "rational(-1,2)"
    assert repr(w.make_rational("-0/2")) == "rational(0,1)"
    assert w.make_rational("10/1").str(False) == "10"
    assert w.rational(-1, -1) == w.rational(1, 1)
    assert w.rational(-12, -4) == w.rational(3, 1)
    assert w.rational(4, 24) == w.rational(1, 6)
    # test to_double
    assert math.isclose(float(w.rational(1, 6)), 0.1666666667)
    assert w.rational(1, 1).latex(True) == "+"
    assert w.rational(-1, 1).latex(True) == "-"
    assert w.rational(3, 1).latex(True) == "3"

    # test failure to convert
    with pytest.raises(Exception):
        w.make_rational("1a/1")
    with pytest.raises(Exception):
        w.make_rational("1/1/1")
    with pytest.raises(Exception):
        w.make_rational("/")
    with pytest.raises(Exception):
        w.make_rational("+/2")
    with pytest.raises(Exception):
        assert w.make_rational("-/2")


if __name__ == "__main__":
    test_rational()
