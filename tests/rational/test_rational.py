import wicked as w


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

    assert repr(w.make_rational("1/2")) == "1/2"
    assert repr(w.make_rational("-12")) == "-12"
    assert repr(w.make_rational("12")) == "12"
    assert repr(w.make_rational("-/2")) == "-1/2"
    assert repr(w.make_rational("-0/2")) == "0"
    assert w.make_rational("10/1").str(False) == "10"
    assert w.rational(-1, -1) == w.rational(1, 1)
    assert w.rational(-12, -4) == w.rational(3, 1)
    assert w.rational(4, 24) == w.rational(1, 6)
    # test to_double
    assert math.isclose(float(w.rational(1, 6)), 0.1666666667)


if __name__ == "__main__":
    test_rational()
