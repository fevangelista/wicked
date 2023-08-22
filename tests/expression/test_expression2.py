import wicked as w
w.add_space('g','fermion','unoccupied',list('abcdefghijklmnopqrstuvwxyz'))

def test_expression1():
    """Test the vacuum_normal_ordered method of the Expression class on pairs of operators"""
    # test same-type ordered
    e = w.expression("a+(g_0) a+(g_1)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a+(g0) a+(g1)",False)    
    assert str(expr) == "a+(g0) a+(g1)"
    assert expr == test_expr

    # test same-type unordered
    e = w.expression("a+(g_1) a+(g_0)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(g0) a+(g1)",False)
    assert expr == test_expr

    # test same-type repeated
    e = w.expression("a+(g_1) a+(g_1)",False)
    expr = e.vacuum_normal_ordered()
    assert str(expr) == ""

    # test same-type repeated
    e = w.expression("a+(g_1) a+(g_1) a+(g_1)",False)
    expr = e.vacuum_normal_ordered()
    assert str(expr) == ""    

    # test same-type ordered
    e = w.expression("a-(g_1) a-(g_0)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a-(g1) a-(g0)",False)
    assert expr == test_expr

    # test same-type unordered
    e = w.expression("a-(g_0) a-(g_1)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a-(g1) a-(g0)",False)    
    assert str(expr) == "-a-(g1) a-(g0)"
    assert expr == test_expr

    # test same-type repeated
    e = w.expression("a-(g0) a-(g_1) a-(g0) a-(g_1)",False)
    expr = e.vacuum_normal_ordered()
    assert str(expr) == ""    


def test_expression2():
    """Test the vacuum_normal_ordered method of the Expression class on pairs of operators"""
    # test different-type ordered
    e = w.expression("a+(g_0) a-(g_1)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("a+(g0) a-(g1)",False)    
    assert str(expr) == "a+(g0) a-(g1)"
    assert expr == test_expr

    # test different-type ordered ignoring contractions of different indices
    e = w.expression("a+(g_0) a-(g_1)",False)
    expr = e.vacuum_normal_ordered(True)
    test_expr = w.expression("a+(g0) a-(g1)",False)    
    assert str(expr) == "a+(g0) a-(g1)"
    assert expr == test_expr

    # test different-type unordered
    e = w.expression("a-(g_0) a+(g_1)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(g1) a-(g0)",False) + w.expression("+delta^{g1}_{g0}",False)
    assert expr == test_expr

    # test different-type unordered ignoring contractions of different indices
    e = w.expression("a-(g_0) a+(g_1)",False)
    expr = e.vacuum_normal_ordered(True)
    test_expr = w.expression("-a+(g1) a-(g0)",False)
    assert expr == test_expr    

    # test different-type unordered
    e = w.expression("a-(g_0) a+(g_0)",False)
    expr = e.vacuum_normal_ordered()
    test_expr = w.expression("-a+(g0) a-(g0)",False) + w.expression("1",False)
    assert expr == test_expr

    # test different-type unordered ignoring contractions of different indices
    e = w.expression("a-(g_0) a+(g_0)",False)
    expr = e.vacuum_normal_ordered(True)
    test_expr = w.expression("-a+(g0) a-(g0)",False) + w.expression("1",False)
    assert expr == test_expr

if __name__ == "__main__":
    test_expression1()
    test_expression2()
