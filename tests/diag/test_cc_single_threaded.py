import wicked as w


def print_comparison(val, val2):
    print(f"Result: {val}")
    print(f"Test:   {val2}")


def compare_expressions(test, ref):
    test_expr = w.Expression()
    ref_expr = w.Expression()
    for s in ref:
        ref_expr += w.string_to_expr(s)
    for eq in test:
        test_expr += eq.rhs_expression()
    print_comparison(test_expr, ref_expr)
    assert test_expr == ref_expr


def initialize():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])


def make_T(n):
    components = [f"{'v+' * k} {'o' * k}" for k in range(1, n + 1)]
    return w.op("t", components)


def cc_equations(n):
    initialize()

    wt = w.WickTheorem()
    wt.set_single_threaded(True)

    E0 = w.op("E_0", [""])
    F = w.utils.gen_op("f", 1, "ov", "ov")
    V = w.utils.gen_op("v", 2, "ov", "ov")
    H = E0 + F + V

    T = make_T(n)
    Hbar = w.bch_series(H, T, 4)
    expr = wt.contract(w.rational(1), Hbar, 0, 2 * n)
    mbeq = expr.to_manybody_equation("r")

    equations = {}
    for r in range(0, n + 1):
        s = f"{'o' * r}|{'v' * r}"
        equations[r] = mbeq[s]

    return equations


def test_cc():
    """Test the CC equations"""
    cc_test_n = 4
    cc_n(cc_test_n)


def cc_n(max_n):
    """Evaluate the number of terms in the n-th order CC equations"""
    # The number of terms in the CC equations for n = 0, 1, 2, ..., 8
    diagrams_count_ref = [
        [1],
        [3, 8],
        [4, 14, 31],
        [4, 15, 37, 47],
        [4, 15, 38, 53, 74],
        [4, 15, 38, 54, 80, 99],
        [4, 15, 38, 54, 81, 105, 135],
        [4, 15, 38, 54, 81, 106, 141, 169],
        [4, 15, 38, 54, 81, 106, 142, 175, 215],
    ]

    width = 5 * (max_n + 1) + 2
    print(f'{"=" * width}')
    print(f" n     Exc. level ")
    print(f'  {" ".join([f"{k:4d}" for k in range(0,max_n + 1)])}')
    print(f'{"-" * width}')

    rows = []
    diagrams_count = []
    for n in range(0, max_n + 1):
        equations = cc_equations(n)
        diagrams_count.append([len(eqs) for _, eqs in equations.items()])
        count = " ".join([f"{len(eqs):4d}" for k, eqs in equations.items()])
        rows.append(f" {n}{count}")
        print(f" {n}{count}")

    print(f'{"=" * width}')

    print(diagrams_count)
    print(diagrams_count_ref[: max_n + 1])

    assert diagrams_count == diagrams_count_ref[: max_n + 1]


if __name__ == "__main__":
    test_cc()
