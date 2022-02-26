import wicked as w


def print_comparison(val, val2):
    print(f"Result: {val}")
    print(f"Test:   {val2}")


def initialize():
    w.reset_space()
    w.add_space("o", "fermion", "occupied", ["i", "j", "k", "l", "m", "n"])
    w.add_space("v", "fermion", "unoccupied", ["a", "b", "c", "d", "e", "f"])


def test_energy():
    """MP1 amplitudes"""
    initialize()
    T2 = w.op("T2", ["v+ v+ o o"])
    F0 = w.op("f0", ["o+ o", "v+ v"])
    F1 = w.op("f1", ["o+ v", "v+ o"])
    V1 = w.op(
        "v",
        [
            "o+ o+ o o",
            "o+ o+ v o",
            "o+ o+ v v",
            "o+ v+ o o",
            "o+ v+ v o",
            "o+ v+ v v",
            "v+ v+ o o",
            "v+ v+ v o",
            "v+ v+ v v",
        ],
    )

    wt = w.WickTheorem()

    expr = wt.contract(w.rational(1), w.commutator(F0, T2), 0, 4)
    expr += wt.contract(w.rational(1), V1, 0, 4)
    mbeq = expr.to_manybody_equation("R")

    print(mbeq)
    for rank in mbeq:
        for eq in mbeq[rank]:
            print(eq)

    for eq in mbeq['oo|vv']:
        print(eq.compile("einsum"))
    # assert len(mbeq[0]) == 1
    # comp = mbeq[0][0].compile("einsum")
    # print(comp)
    # assert comp == 'R += 1.000000000 * np.einsum("ia,ai->",T1,f)'

    # assert len(mbeq[2]) == 4
    # comp = mbeq[2][0].compile("einsum")
    # print(comp)
    # assert comp == 'Rov += 1.000000000 * np.einsum("ib,ba->ia",T1,f)'

    # comp = mbeq[2][1].compile("einsum")
    # print(comp)
    # assert comp == 'Rov += -1.000000000 * np.einsum("ja,ij->ia",T1,f)'

    # comp = mbeq[2][2].compile("einsum")
    # print(comp)
    # assert comp == 'Roo += 1.000000000 * np.einsum("ia,aj->ij",T1,f)'

    # comp = mbeq[2][3].compile("einsum")
    # print(comp)
    # assert comp == 'Rvv += -1.000000000 * np.einsum("ia,bi->ba",T1,f)'

    # expr = wt.contract(w.rational(1), V @ T2, 0, 2)
    # mbeq = expr.to_manybody_equation("R")

    # assert len(mbeq[0]) == 1
    # comp = mbeq[0][0].compile("einsum")
    # print(comp)
    # assert comp == 'R += 0.250000000 * np.einsum("ijab,abij->",T2,v)'

    # print(mbeq[2])
    # expr += wt.contract(w.rational(1, 2), V @ T1 @ T1, 0, 0)
    # print(expr)


if __name__ == "__main__":
    test_energy()
