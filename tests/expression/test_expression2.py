import wicked as w

def test_expression1():
    """Test the vacuum_normal_ordered method of the Expression class on pairs of operators"""
    w.reset_space()
    w.add_space('g','fermion','unoccupied',list('abcdefghijklmnopqrstuvwxyz'))

    # test same-type ordered
    e = w.expression("a+(g_0) a+(g_1)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a+(g0) a+(g1)")    
    assert str(expr) == "a+(g0) a+(g1)"
    assert expr == test_expr

    # test same-type unordered
    e = w.expression("a+(g_1) a+(g_0)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(g0) a+(g1)")
    assert expr == test_expr

    # test same-type repeated
    e = w.expression("a+(g_1) a+(g_1)")
    expr = e.vacuum_normal_ordered()
    assert str(expr) == ""

    # test same-type repeated
    e = w.expression("a+(g_1) a+(g_1) a+(g_1)")
    expr = e.vacuum_normal_ordered()
    assert str(expr) == ""    

    # test same-type ordered
    e = w.expression("a-(g_1) a-(g_0)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a-(g1) a-(g0)")
    assert expr == test_expr

    # test same-type unordered
    e = w.expression("a-(g_0) a-(g_1)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a-(g1) a-(g0)")    
    assert str(expr) == "-a-(g1) a-(g0)"
    assert expr == test_expr

    # test same-type repeated
    e = w.expression("a-(g0) a-(g_1) a-(g0) a-(g_1)")
    expr = e.vacuum_normal_ordered()
    assert str(expr) == ""    


def test_expression2():
    """Test the vacuum_normal_ordered method of the Expression class on pairs of operators"""
    w.reset_space()
    w.add_space('g','fermion','unoccupied',list('abcdefghijklmnopqrstuvwxyz'))

    # test different-type ordered
    e = w.expression("a+(g_0) a-(g_1)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a+(g0) a-(g1)")    
    assert str(expr) == "a+(g0) a-(g1)"
    assert expr == test_expr

    # test different-type ordered ignoring contractions of different indices
    e = w.expression("a+(g_0) a-(g_1)")
    expr = e.vacuum_normal_ordered(True)
    test_expr = w.expression("a+(g0) a-(g1)")    
    assert str(expr) == "a+(g0) a-(g1)"
    assert expr == test_expr

    # test different-type unordered
    e = w.expression("a-(g_0) a+(g_1)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(g1) a-(g0)") + w.expression("+delta^{g1}_{g0}")
    assert expr == test_expr

    # test different-type unordered ignoring contractions of different indices
    e = w.expression("a-(g_0) a+(g_1)")
    expr = e.vacuum_normal_ordered(True)
    test_expr = w.expression("-a+(g1) a-(g0)")
    assert expr == test_expr    

    # test different-type unordered
    e = w.expression("a-(g_0) a+(g_0)")
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(g0) a-(g0)") + w.expression("1")
    assert expr == test_expr

    # test different-type unordered ignoring contractions of different indices
    e = w.expression("a-(g_0) a+(g_0)")
    expr = e.vacuum_normal_ordered(True)
    test_expr = w.expression("-a+(g0) a-(g0)") + w.expression("1")
    assert expr == test_expr

def test_expression3():
    """Test the vacuum_normal_ordered method of the Expression class on commutators of operators"""
    w.reset_space()
    w.add_space('g','fermion','unoccupied',list('abcdefghijklmnopqrstuvwxyz'))

    o = w.expression("a+(g_1) a-(g_2)")

    k = w.expression("t^{}_{} a+(g_1) a-(g_3)")
    k -= k.adjoint()

    c1 = (o @ k) - (k @ o)
    c1 = c1.vacuum_normal_ordered(True)
    test_c1 = w.expression("t^{}_{} a+(g_3) a-(g_2)")
    assert c1 == test_c1

    c2 = (c1 @ k) - (k @ c1)
    c2 = c2.vacuum_normal_ordered(True)
    test_c2 = w.expression("-t^{}_{} t^{}_{} a+(g1) a-(g2)")
    assert c2 == test_c2

if __name__ == "__main__":
    test_expression1()
    test_expression2()
    test_expression3()    
